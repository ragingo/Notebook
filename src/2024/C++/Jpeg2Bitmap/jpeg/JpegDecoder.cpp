#include "JpegDecoder.h"
#include <cassert>
#include <mdspan>
#include <memory>
#include <nameof.hpp>
#include <print>
#include <unordered_map>
#include "debugging.h"
#include "../math/math.h"

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

    constexpr void levelShift(MCUBlock8x8& zz)
    {
        for (int i = 0; i < zz.size(); ++i) {
            zz[i] = std::clamp(zz[i] + 128, 0, 255);
        }
    }

    constexpr std::tuple<uint8_t, uint8_t, uint8_t> yuvToRGB(int y, int cb, int cr)
    {
        cb -= 128;
        cr -= 128;
        double r = y + 1.402 * cr;
        double g = y - 0.344136 * cb - 0.714136 * cr;
        double b = y + 1.772 * cb;
        r = std::clamp(r, 0.0, 255.0);
        g = std::clamp(g, 0.0, 255.0);
        b = std::clamp(b, 0.0, 255.0);
        return { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b) };
    }

    void convertRGB(
        std::vector<uint8_t>& pixels,
        int width,
        int height,
        const ComponentInfo& componentY,
        const ComponentInfo& componentCb,
        const ComponentInfo& componentCr
    )
    {
        double hY = static_cast<double>(componentY.horizontalSamplingFactor);
        double vY = static_cast<double>(componentY.verticalSamplingFactor);
        double hCb = static_cast<double>(componentCb.horizontalSamplingFactor);
        double vCb = static_cast<double>(componentCb.verticalSamplingFactor);
        double hCr = static_cast<double>(componentCr.horizontalSamplingFactor);
        double vCr = static_cast<double>(componentCr.verticalSamplingFactor);

        double cbSampleFactorH = hCb / hY;
        double cbSampleFactorV = vCb / vY;
        double crSampleFactorH = hCr / hY;
        double crSampleFactorV = vCr / vY;

        for (int row = 0; row < height; ++row) {
            int yOffset = row * width;

            int cbRow = static_cast<int>(row * cbSampleFactorV);
            int crRow = static_cast<int>(row * crSampleFactorV);

            for (int col = 0; col < width; ++col) {
                int cbCol = static_cast<int>(col * cbSampleFactorH);
                int crCol = static_cast<int>(col * crSampleFactorH);

                int cbOffset = cbRow * componentCb.width + cbCol;
                int crOffset = crRow * componentCr.width + crCol;

                int y = componentY.buffer[yOffset + col];
                int cb = componentCb.buffer[cbOffset];
                int cr = componentCr.buffer[crOffset];

                auto [r, g, b] = yuvToRGB(y, cb, cr);

                pixels[(yOffset + col) * 3 + 0] = b;
                pixels[(yOffset + col) * 3 + 1] = g;
                pixels[(yOffset + col) * 3 + 2] = r;
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

    if (auto colorSpace = getColorSpace(*sof0); colorSpace != ColorSpace::YCbCr) {
        std::println("Unsupported color space: {}", NAMEOF_ENUM(colorSpace));
        return;
    }

    if (!isInterleaved(*sof0)) {
        std::println("Unsupported non-interleaved MCU");
        return;
    }

    if (auto format = getYUVFormat(*sof0); format != YUVFormat::YUV420) {
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

    const auto [hMaxFactor, vMaxFactor] = getMaxSamplingFactor(*sof0);

    ComponentInfo componentY{};
    ComponentInfo componentCb{};
    ComponentInfo componentCr{};
    auto componentSelector = [&](ComponentID id) -> ComponentInfo& {
        switch (id) {
        case ComponentID::Y:
            return componentY;
        case ComponentID::Cb:
            return componentCb;
        case ComponentID::Cr:
            return componentCr;
        default:
            assert(false);
            return componentY; // dummy
        }
    };

    for (const auto& component : sof0->components) {
        auto& info = componentSelector(component.id);
        // 各コンポーネントのサンプリング係数を取得
        info.horizontalSamplingFactor = component.horizonalSamplingFactor;
        info.verticalSamplingFactor = component.verticalSamplingFactor;

        // コンポーネントの幅と高さを計算
        info.width = (sof0->width * info.horizontalSamplingFactor + hMaxFactor - 1) / hMaxFactor;
        info.height = (sof0->height * info.verticalSamplingFactor + vMaxFactor - 1) / vMaxFactor;

        // バッファの初期化
        int size = info.width * info.height;
        info.buffer = std::vector<int>(size, 0);
    }

    const int mcuWidth = hMaxFactor * 8;
    const int mcuHeight = vMaxFactor * 8;
    const int mcuHorizCount = (sof0->width + mcuWidth - 1) / mcuWidth;
    const int mcuVertCount = (sof0->height + mcuHeight - 1) / mcuHeight;

    assert(mcuHorizCount * mcuWidth >= sof0->width);
    assert(mcuVertCount * mcuHeight >= sof0->height);

    // ファイル全体を通して更新し続ける。
    // ただし、リスタートマーカーがある場合は、この値をリセットする。
    std::array<int, 3> dcPred = { 0, 0, 0 };

    for (int mcuRow = 0; mcuRow < mcuVertCount; ++mcuRow) {
        for (int mcuCol = 0; mcuCol < mcuHorizCount; ++mcuCol) {
            for (auto&& component : sof0->components) {
                auto componentIndex = std::distance(sof0->components.data(), &component);
                auto [dcTable, dcDHT] = dcTables[std::to_underlying(component.tableID)];
                auto [acTable, acDHT] = acTables[std::to_underlying(component.tableID)];
                auto dqt = dqts[std::to_underlying(component.tableID)];

                // 1 MCU Y 16x16 Cb 8x8 Cr 8x8 を、 8x8 のブロックに分割して処理
                for (int blockRow = 0; blockRow < component.verticalSamplingFactor; ++blockRow) {
                    for (int blockCol = 0; blockCol < component.horizonalSamplingFactor; ++blockCol) {
                        //std::println("MCU: ({}, {})", mcuCol, mcuRow);
                        MCUBlock8x8 block{};
                        decodeBlock(dcTable, dcDHT, acTable, acDHT, dqt, block, dcPred[componentIndex]);
                        std::mdspan<int, std::extents<int, 8, 8>> blockView(block.data(), 8, 8);

                        // MCU内のブロック処理ループ内
                        for (int y = 0; y < 8; ++y) {
                            for (int x = 0; x < 8; ++x) {
                                int cx = ((mcuCol * component.horizonalSamplingFactor + blockCol) * 8) + x;
                                int cy = ((mcuRow * component.verticalSamplingFactor + blockRow) * 8) + y;

                                int width = componentSelector(component.id).width;
                                int height = componentSelector(component.id).height;
                                if (cx < width && cy < height) {
                                    int index = cy * width + cx;
                                    componentSelector(component.id).buffer[index] = blockView[y, x];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    std::vector<uint8_t> pixels(sof0->width * sof0->height * sof0->numComponents, 0);

    convertRGB(
        pixels,
        sof0->width,
        sof0->height,
        componentY,
        componentCb,
        componentCr
    );

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
        code = (code << 1) + m_BitStreamReader->getNextBit();
        if (code <= table.maxCode[i]) {
            break;
        }
    }

    int j = table.valPtr[i];
    j = j + code - table.minCode[i];
    int value = symbols[j];
    return value;
}

int JpegDecoder::decodeZZ(int ssss)
{
    int value = m_BitStreamReader->receive(ssss);
    return extend(value, ssss);
}

std::array<int, 64> JpegDecoder::decodeACCoeffs(HuffmanTable& table, const std::vector<uint8_t>& symbols)
{
    std::array<int, 64> zz{};
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
                break; // 範囲外アクセス防止
            }
            int coeff = decodeZZ(ssss);
            zz[k] = coeff;
            k++;
        }
    }

    return zz;
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

    auto zz = decodeACCoeffs(acTable, acDHT->symbols);
    for (int j = 1; j < 64; ++j) {
        block[j] = zz[j];
    }
    //debugging::dumpBlock(" ac coeff", block);

    dequantize(block, *dqt);
    //debugging::dumpBlock(" dequantize", block);

    reorder(block);
    //debugging::dumpBlock(" reorder", block);

    MCUBlock8x8 temp = block;
    math::idct<int, 8, 8>(block, temp);
    block = temp;
    //debugging::dumpBlock(" idct", block);

    levelShift(block);
    //debugging::dumpBlock(" level shift", block);
}
