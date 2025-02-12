#include "JpegDecoder.h"
#include <immintrin.h>
#include <cassert>
#include <memory>
#include <nameof.hpp>
#include <print>
#include "debugging.h"
#include "../math/math.h"
#include "../image/color_model.h"
#include "../simd/simd.h"

using namespace jpeg;
using namespace jpeg::segments;

namespace
{
    using MCUBlock8x8 = std::array<int, 64>;

    struct ComponentInfo
    {
        ComponentID id;
        int horizontalSamplingFactor;
        int verticalSamplingFactor;
        int width;
        int height;
        std::vector<int> buffer;
    };

    class YCbCrComponents
    {
    public:
        YCbCrComponents(const SOF0& sof0)
        {
            sampleWidth = sof0.width;
            sampleHeight = sof0.height;

            auto componentSelector = [&](ComponentID id) -> ComponentInfo& {
                switch (id) {
                case ComponentID::Y:
                    return y;
                case ComponentID::Cb:
                    return cb;
                case ComponentID::Cr:
                    return cr;
                default:
                    assert(false);
                    return y; // dummy
                }
                };

            for (const auto& component : sof0.components) {
                assert(component.id == ComponentID::Y || component.id == ComponentID::Cb || component.id == ComponentID::Cr);
                auto& info = componentSelector(component.id);
                info.horizontalSamplingFactor = component.horizonalSamplingFactor;
                info.verticalSamplingFactor = component.verticalSamplingFactor;
            }

            for (const auto& component : sof0.components) {
                auto& info = componentSelector(component.id);
                const auto hMaxFactor = getMaxHorizontalSamplingFactor();
                const auto vMaxFactor = getMaxVerticalSamplingFactor();
                info.width = (sof0.width * info.horizontalSamplingFactor + hMaxFactor - 1) / hMaxFactor;
                info.height = (sof0.height * info.verticalSamplingFactor + vMaxFactor - 1) / vMaxFactor;
                info.buffer = std::vector<int>(info.width * info.height, 0);
            }
        }

        inline int getMaxHorizontalSamplingFactor() const
        {
            return std::max({ y.horizontalSamplingFactor, cb.horizontalSamplingFactor, cr.horizontalSamplingFactor });
        }

        inline int getMaxVerticalSamplingFactor() const
        {
            return std::max({ y.verticalSamplingFactor, cb.verticalSamplingFactor, cr.verticalSamplingFactor });
        }

        inline int getMCUWidth() const
        {
            return getMaxHorizontalSamplingFactor() * 8;
        }

        inline int getMCUHeight() const
        {
            return getMaxVerticalSamplingFactor() * 8;
        }

        inline int getMCUHorizontalCount() const
        {
            return (sampleWidth + getMCUWidth() - 1) / getMCUWidth();
        }

        inline int getMCUVerticalCount() const
        {
            return (sampleHeight + getMCUHeight() - 1) / getMCUHeight();
        }

        inline double getCbHorizontalSamplingFactor() const
        {
            return static_cast<double>(cb.horizontalSamplingFactor) / static_cast<double>(y.horizontalSamplingFactor);
        }

        inline double getCbVerticalSamplingFactor() const
        {
            return static_cast<double>(cb.verticalSamplingFactor) / static_cast<double>(y.verticalSamplingFactor);
        }

        inline double getCrHorizontalSamplingFactor() const
        {
            return static_cast<double>(cr.horizontalSamplingFactor) / static_cast<double>(y.horizontalSamplingFactor);
        }

        inline double getCrVerticalSamplingFactor() const
        {
            return static_cast<double>(cr.verticalSamplingFactor) / static_cast<double>(y.verticalSamplingFactor);
        }

        inline const ComponentInfo& getComponent(ComponentID id) const
        {
            switch (id) {
            case ComponentID::Y:
                return y;
            case ComponentID::Cb:
                return cb;
            case ComponentID::Cr:
                return cr;
            default:
                assert(false);
                return y; // dummy
            }
        }

        inline ComponentInfo& getComponent(ComponentID id)
        {
            switch (id) {
            case ComponentID::Y:
                return y;
            case ComponentID::Cb:
                return cb;
            case ComponentID::Cr:
                return cr;
            default:
                assert(false);
                return y; // dummy
            }
        }
    private:
        uint16_t sampleWidth = 0;
        uint16_t sampleHeight = 0;
        ComponentInfo y{};
        ComponentInfo cb{};
        ComponentInfo cr{};
    };

    // Figure A.6 – Zig-zag sequence of quantized DCT coefficients
    constexpr MCUBlock8x8 ZIGZAG = {
         0,  1,  5,  6, 14, 15, 27, 28,
         2,  4,  7, 13, 16, 26, 29, 42,
         3,  8, 12, 17, 25, 30, 41, 43,
         9, 11, 18, 24, 31, 40, 44, 53,
        10, 19, 23, 32, 39, 45, 52, 54,
        20, 22, 33, 38, 46, 51, 55, 60,
        21, 34, 37, 47, 50, 56, 59, 61,
        35, 36, 48, 49, 57, 58, 62, 63
    };

    // Figure F.12 – Extending the sign bit of a decoded value in V
    constexpr int extend(int v, int t)
    {
        int vt = 1 << (t - 1);
        if (v < vt) {
            vt = (-1 << t) + 1;
            v += vt;
        }
        return v;
    }

    inline void dequantize(MCUBlock8x8& block, const DQT& dqt)
    {
        if (dqt.precision == DQT::Precision::BITS_8) {
            auto table = std::get<DQT::Bits8Table>(dqt.table);
            for (int i = 0; i < block.size(); ++i) {
                block[i] *= table[i];
            }
        }
        else if (dqt.precision == DQT::Precision::BITS_16) {
            auto table = std::get<DQT::Bits16Table>(dqt.table);
            for (int i = 0; i < block.size(); ++i) {
                block[i] *= table[i];
            }
        }
    }

    constexpr void reorder(MCUBlock8x8& block)
    {
        MCUBlock8x8 temp{};
        for (int i = 0; i < block.size(); ++i) {
            temp[i] = block[ZIGZAG[i]];
        }
        block.swap(temp);
    }

    inline void levelShift(MCUBlock8x8& zz)
    {
        static __m256i _128 = _mm256_set1_epi32(128);
        static __m256i _0 = _mm256_setzero_si256();
        static __m256i _255 = _mm256_set1_epi32(255);

        for (int i = 0; i < zz.size(); i += 8) {
            __m256i v = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&zz[i]));
            v = _mm256_add_epi32(v, _128);
            simd::clamp256_i32(v, _0, _255);
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(&zz[i]), v);
        }
    }

    void convertColorData(
        std::vector<uint8_t>& pixels,
        int width,
        int height,
        const YCbCrComponents& ycc
    )
    {
        const auto componentY = ycc.getComponent(ComponentID::Y);
        const auto componentCb = ycc.getComponent(ComponentID::Cb);
        const auto componentCr = ycc.getComponent(ComponentID::Cr);

        for (int row = 0; row < height; ++row) {
            int yRow = row * width;
            int cbRow = static_cast<int>(row * ycc.getCbVerticalSamplingFactor()) * componentCb.width;
            int crRow = static_cast<int>(row * ycc.getCrVerticalSamplingFactor()) * componentCr.width;

            for (int col = 0; col < width; ++col) {
                int yOffset = yRow + col;
                int cbCol = static_cast<int>(col * ycc.getCbHorizontalSamplingFactor());
                int crCol = static_cast<int>(col * ycc.getCrHorizontalSamplingFactor());

                int y = componentY.buffer[yOffset];
                int cb = componentCb.buffer[cbRow + cbCol];
                int cr = componentCr.buffer[crRow + crCol];

                auto [r, g, b] = convertYCbCrToRGB(y, cb, cr);

                pixels[yOffset * 3 + 0] = b;
                pixels[yOffset * 3 + 1] = g;
                pixels[yOffset * 3 + 2] = r;
            }
        }
    }
}

JpegDecoder::JpegDecoder(const char* fileName)
    : m_Parser(fileName)
{
}

JpegDecoder::~JpegDecoder()
{
}

void JpegDecoder::decode(DecodeResult& result)
{
    m_Parser.parse();

    debugging::dumpSummary(m_Parser.getMarkers(), m_Parser.getSegments());

    auto sos = findFirstSegment<SOS>(m_Parser.getSegments());
    assert(sos);
    auto dhts = findSegments<DHT>(m_Parser.getSegments());
    assert(!dhts.empty());
    auto sof0 = findFirstSegment<SOF0>(m_Parser.getSegments());
    assert(sof0);
    auto dqts = findSegments<DQT>(m_Parser.getSegments());
    assert(!dqts.empty());
    auto dri = findFirstSegment<DRI>(m_Parser.getSegments());

    if (auto colorSpace = getColorSpace(*sof0); colorSpace != ColorSpace::YCbCr) {
        std::println("Unsupported color space: {}", NAMEOF_ENUM(colorSpace));
        return;
    }

    if (auto format = getYUVFormat(*sof0); format != YUVFormat::YUV420 && format != YUVFormat::YUV444) {
        std::println("Unsupported YUV format: {}", NAMEOF_ENUM(format));
        return;
    }

    m_BitStreamReader = std::make_unique<BitStreamReader>(m_Parser.getECS());
    std::vector<std::tuple<HuffmanTable, std::shared_ptr<DHT>>> dcTables(4);
    std::vector<std::tuple<HuffmanTable, std::shared_ptr<DHT>>> acTables(4);

    for (const auto& dht : dhts) {
        auto huffmanTable = createHuffmanTable(dht->counts);

        if (dht->tableClass == segments::DHT::TableClass::DC_OR_LOSSLESS) {
            dcTables[std::to_underlying(dht->tableID)] = { huffmanTable, dht };
        }
        else {
            acTables[std::to_underlying(dht->tableID)] = { huffmanTable, dht };
        }
    }

    YCbCrComponents ycc(*sof0);
    std::println("MCU: {}x{}", ycc.getMCUHorizontalCount(), ycc.getMCUVerticalCount());

    // ファイル全体を通して更新し続ける
    std::array<int, 3> dcPred = { 0, 0, 0 };

    const int totalMCUCount = ycc.getMCUHorizontalCount() * ycc.getMCUVerticalCount();
    int mcuCount = 0;

    for (int mcuRow = 0; mcuRow < ycc.getMCUVerticalCount(); ++mcuRow) {
        for (int mcuCol = 0; mcuCol < ycc.getMCUHorizontalCount(); ++mcuCol) {
            ++mcuCount;

            // 最後の MCU を処理すると、ECS を読み切ったのに読み込みが何度も発生してしまう。
            // とりあえず、最後の MCU はスキップする。
            if (dri && mcuCount == totalMCUCount) {
                break;
            }

            // DRI で指定された間隔でリスタートマーカーがある場合、 dcPred をリセット
            if (dri && mcuCount % dri->restartInterval == 0 && mcuCount != totalMCUCount) {
                if (m_BitStreamReader->nextByte() != 0xFF) {
                    std::println("Restart marker not found");
                    return;
                }
                uint8_t marker = m_BitStreamReader->nextByte();
                if (marker < 0xD0 || marker > 0xD7) {
                    std::println("Invalid restart marker: 0xFF{:02X}", marker);
                    return;
                }
                dcPred = { 0, 0, 0 };
            }

            for (auto&& component : sof0->components) {
                auto componentIndex = std::distance(sof0->components.data(), &component);
                auto [dcTable, dcDHT] = dcTables[std::to_underlying(component.tableID)];
                auto [acTable, acDHT] = acTables[std::to_underlying(component.tableID)];
                auto dqt = dqts[std::to_underlying(component.tableID)];

                // 4:4:4 の場合、1 MCU Y 8x8 Cb 8x8 Cr 8x8 で処理
                if (getYUVFormat(*sof0) == YUVFormat::YUV444) {
                    MCUBlock8x8 block{};
                    decodeBlock(dcTable, dcDHT, acTable, acDHT, dqt, block, dcPred[componentIndex]);
                    for (int y = 0; y < 8; ++y) {
                        for (int x = 0; x < 8; ++x) {
                            int cx = (mcuCol * 8) + x;
                            int cy = (mcuRow * 8) + y;
                            int width = ycc.getComponent(component.id).width;
                            int index = cy * width + cx;
                            ycc.getComponent(component.id).buffer[index] = block[y * 8 + x];
                        }
                    }
                }

                // 4:2:0 の場合、1 MCU Y 16x16 Cb 8x8 Cr 8x8 を、 8x8 のブロックに分割して処理
                if (getYUVFormat(*sof0) == YUVFormat::YUV420) {
                    for (int blockRow = 0; blockRow < component.verticalSamplingFactor; ++blockRow) {
                        for (int blockCol = 0; blockCol < component.horizonalSamplingFactor; ++blockCol) {
                            MCUBlock8x8 block{};
                            decodeBlock(dcTable, dcDHT, acTable, acDHT, dqt, block, dcPred[componentIndex]);

                            // MCU内のブロック処理ループ内
                            for (int y = 0; y < 8; ++y) {
                                for (int x = 0; x < 8; ++x) {
                                    int cx = ((mcuCol * component.horizonalSamplingFactor + blockCol) * 8) + x;
                                    int cy = ((mcuRow * component.verticalSamplingFactor + blockRow) * 8) + y;

                                    int width = ycc.getComponent(component.id).width;
                                    int height = ycc.getComponent(component.id).height;
                                    if (cx < width && cy < height) {
                                        int index = cy * width + cx;
                                        ycc.getComponent(component.id).buffer[index] = block[y * 8 + x];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    std::vector<uint8_t> pixels(sof0->width * sof0->height * sof0->numComponents, 0);

    convertColorData(pixels, sof0->width, sof0->height, ycc);

    result = {
        .width = sof0->width,
        .height = sof0->height,
        .pixels = pixels
    };
}

std::vector<int> JpegDecoder::createHuffSize(const std::array<uint8_t, 16>& counts)
{
    std::vector<int> huffSize;
    for (int bits = 1; bits <= counts.size(); ++bits) {
        for (int count = 0; count < counts[bits - 1]; ++count) {
            huffSize.push_back(bits);
        }
    }
    return huffSize;
}

std::vector<int> JpegDecoder::createHuffCode(const std::vector<int>& huffSize)
{
    assert(!huffSize.empty());

    std::vector<int> huffCode(huffSize.size());
    int code = 0;
    int si = huffSize[0];

    for (int i = 0; i < huffSize.size(); ++i) {
        int bits = huffSize[i];

        if (bits == 0) {
            break;
        }

        if (bits != si) {
            code <<= (bits - si);
            si = bits;
        }

        huffCode[i] = code;
        code++;
    }
    return huffCode;
}

JpegDecoder::HuffmanTable JpegDecoder::createHuffmanTable(const std::array<uint8_t, 16>& counts)
{
    auto huffSize = createHuffSize(counts);
    auto huffCode = createHuffCode(huffSize);

    HuffmanTable table{};
    table.minCode.resize(16, 0);
    table.maxCode.resize(16, -1);
    table.valPtr.resize(16, 0);

    int j = 0;
    for (int i = 0; i < 16; ++i) {
        auto bit = counts[i];
        if (bit == 0) {
            continue;
        }
        table.valPtr[i] = j;
        table.minCode[i] = huffCode[j];
        j = j + bit - 1;
        table.maxCode[i] = huffCode[j];
        j++;
    }

    return table;
}

int JpegDecoder::decodeHuffmanSymbol(HuffmanTable& table, const std::vector<uint8_t>& symbols)
{
    int code = 0;
    int i = 0;

    for (; i < 16; ++i) {
        code = (code << 1) | m_BitStreamReader->nextBit();
        if (code <= table.maxCode[i]) {
            break;
        }
    }

    int j = table.valPtr[i];
    j = j + code - table.minCode[i];
    int value = symbols[j];
    return value;
}

inline int JpegDecoder::decodeZZ(int ssss)
{
    int value = m_BitStreamReader->receive(ssss);
    return extend(value, ssss);
}

void JpegDecoder::decodeACCoeffs(
    HuffmanTable& table,
    const std::vector<uint8_t>& symbols,
    std::array<int, 64>& block
)
{
    int k = 1; // DC係数は既にデコード済みなので、kを1から開始

    while (k < 64) {
        int symbol = decodeHuffmanSymbol(table, symbols);
        if (symbol == 0) {
            break; // EOBマーカー
        }

        int rrrr = symbol >> 4;   // ランレングス（連続するゼロの個数）
        int ssss = symbol & 0x0F; // 非ゼロ係数のビット長

        if (ssss == 0) {
            if (rrrr == 15) {
                k += 16; // ZRL（16個のゼロ）
            } else {
                break; // 他に非ゼロ係数はない
            }
        } else {
            k += rrrr; // ランレングス分インデックスを進める
            if (k >= 64) {
                break;
            }
            block[k++] = decodeZZ(ssss);
        }
    }
}

int JpegDecoder::decodeDCCoeff(HuffmanTable& table, const std::vector<uint8_t>& symbols, int& pred)
{
    int symbol = decodeHuffmanSymbol(table, symbols);
    int diff = (symbol == 0) ? 0 : extend(m_BitStreamReader->receive(symbol), symbol);
    int dcCoeff = pred + diff;
    pred = dcCoeff;
    return dcCoeff;
}

void JpegDecoder::decodeBlock(
    HuffmanTable& dcTable,
    std::shared_ptr<DHT> dcDHT,
    HuffmanTable& acTable,
    std::shared_ptr<DHT> acDHT,
    std::shared_ptr<DQT> dqt,
    std::array<int, 64>& block,
    int& dcPred
)
{
    block[0] = decodeDCCoeff(dcTable, dcDHT->symbols, dcPred);
    //debugging::dumpBlock(" dc coeff", block);

    decodeACCoeffs(acTable, acDHT->symbols, block);
    //debugging::dumpBlock(" ac coeff", block);

    dequantize(block, *dqt);
    //debugging::dumpBlock(" dequantize", block);

    reorder(block);
    //debugging::dumpBlock(" reorder", block);

    math::idct(block);
    //debugging::dumpBlock(" idct", block);

    levelShift(block);
    //debugging::dumpBlock(" level shift", block);
}
