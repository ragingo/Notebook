#include "JpegDecoder.h"
#include <immintrin.h>
#include <cassert>
#include <memory>
#include <nameof.hpp>
#include <print>
#include "jpeg/debugging/debugging.h"
#include "math/math.h"
#include "image/Color.h"
#include "simd/simd.h"
#include "Common.h"
#include "Utility.h"
#include "YCbCrComponents.h"

using namespace jpeg;
using namespace jpeg::segments;

namespace
{
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

            // uint8_t[] to int16_t[]
            std::array<int16_t, 64> temp{};
            for (size_t i = 0; i < 64; ++i) {
                temp[i] = table[i];
            }

            for (size_t i = 0; i < block.size(); i += 16) {
                __m256i b = _mm256_load_si256(reinterpret_cast<const __m256i*>(&block[i]));
                __m256i t = _mm256_load_si256(reinterpret_cast<const __m256i*>(&temp[i]));
                b = _mm256_mullo_epi16(b, t);
                _mm256_store_si256(reinterpret_cast<__m256i*>(&block[i]), b);
            }
        }
        else if (dqt.precision == DQT::Precision::BITS_16) {
            auto table = std::get<DQT::Bits16Table>(dqt.table);
            for (size_t i = 0; i < block.size(); ++i) {
                block[i] *= table[i];
            }
        }
    }

    inline void reorder(MCUBlock8x8& block)
    {
        MCUBlock8x8 temp{};
        for (size_t i = 0; i < block.size(); ++i) {
            temp[i] = block[ZIGZAG[i]];
        }
        block.swap(temp);
    }

    inline void levelShift(MCUBlock8x8& block)
    {
        for (int i = 0; i < block.size(); i += 16) {
            __m256i v = _mm256_load_si256(reinterpret_cast<const __m256i*>(&block[i]));
            v = _mm256_add_epi16(v, _mm256_set1_epi16(128));
            _mm256_store_si256(reinterpret_cast<__m256i*>(&block[i]), v);
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
    using TableInfo = std::tuple<HuffmanTable, std::shared_ptr<DHT>>;
    std::array<TableInfo, 4> dcTables{};
    std::array<TableInfo, 4> acTables{};

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

    //const int totalMCUCount = ycc.getMCUHorizontalCount() * ycc.getMCUVerticalCount();
    int mcuCount = 0;

    for (int mcuRow = 0; mcuRow < ycc.getMCUVerticalCount(); ++mcuRow) {
        for (int mcuCol = 0; mcuCol < ycc.getMCUHorizontalCount(); ++mcuCol) {
            ++mcuCount;

            // 最後の MCU を処理すると、ECS を読み切ったのに読み込みが何度も発生してしまう。
            // とりあえず、最後の MCU はスキップする。
            //if (dri && mcuCount == totalMCUCount) {
            //    break;
            //}

            if (!m_BitStreamReader->hasMore()) {
                break;
            }

            // DRI で指定された間隔でリスタートマーカーがある場合、 dcPred をリセット
            if (dri && mcuCount % dri->restartInterval == 0) {
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
                auto& [dcTable, dcDHT] = dcTables[std::to_underlying(component.tableID)];
                auto& [acTable, acDHT] = acTables[std::to_underlying(component.tableID)];
                auto dqt = dqts[std::to_underlying(component.tableID)];
                auto& buf = ycc.getComponent(component.id).buffer;
                int width = ycc.getComponent(component.id).width;

                // 4:4:4 の場合、1 MCU Y  8 x  8, Cb 8 x 8, Cr 8 x 8 で処理
                // 4:2:0 の場合、1 MCU Y 16 x 16, Cb 8 x 8, Cr 8 x 8 となるため、Y は 2 ブロック分の処理が必要

                for (int blockRow = 0; blockRow < component.verticalSamplingFactor; ++blockRow) {
                    for (int blockCol = 0; blockCol < component.horizonalSamplingFactor; ++blockCol) {
                        MCUBlock8x8 block{};
                        decodeBlock(dcTable, dcDHT, acTable, acDHT, dqt, block, dcPred[componentIndex]);

                        // MCU内のブロック
                        for (int y = 0; y < 8; ++y) {
                            size_t blockStride = y * 8;

                            for (int x = 0; x < 8; ++x) {
                                int cx = ((mcuCol * component.horizonalSamplingFactor + blockCol) * 8) + x;
                                int cy = ((mcuRow * component.verticalSamplingFactor + blockRow) * 8) + y;

                                int height = ycc.getComponent(component.id).height;
                                if (cx < width && cy < height) {
                                    int index = cy * width + cx;
                                    buf[index] = block[blockStride + x];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    std::vector<uint8_t> pixels(sof0->width * sof0->height * 4, 0);

    if (getYUVFormat(*sof0) == YUVFormat::YUV420) {
        image::convertYCbCrToBGRA32<image::PixelFormat::YCBCR420_UINT>(
            pixels, sof0->width, sof0->height,
            ycc.getComponent(ComponentID::Y).buffer,
            ycc.getComponent(ComponentID::Cb).buffer,
            ycc.getComponent(ComponentID::Cr).buffer
        );
    }
    else if (getYUVFormat(*sof0) == YUVFormat::YUV444) {
        image::convertYCbCrToBGRA32<image::PixelFormat::YCBCR444_UINT>(
            pixels, sof0->width, sof0->height,
            ycc.getComponent(ComponentID::Y).buffer,
            ycc.getComponent(ComponentID::Cb).buffer,
            ycc.getComponent(ComponentID::Cr).buffer
        );
    }

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

void JpegDecoder::decodeACCoefs(
    HuffmanTable& table,
    const std::vector<uint8_t>& symbols,
    MCUBlock8x8& block
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
            // decodeZZ や receive 時点で int16_t にしてもいいかもしれない
            block[k++] = static_cast<int16_t>(decodeZZ(ssss));
        }
    }
}

int JpegDecoder::decodeDCCoef(HuffmanTable& table, const std::vector<uint8_t>& symbols, int& pred)
{
    int symbol = decodeHuffmanSymbol(table, symbols);
    int diff = (symbol == 0) ? 0 : extend(m_BitStreamReader->receive(symbol), symbol);
    int dcCoef = pred + diff;
    pred = dcCoef;
    return dcCoef;
}

void JpegDecoder::decodeBlock(
    HuffmanTable& dcTable,
    std::shared_ptr<DHT> dcDHT,
    HuffmanTable& acTable,
    std::shared_ptr<DHT> acDHT,
    std::shared_ptr<DQT> dqt,
    MCUBlock8x8& block,
    int& dcPred
)
{
    // TODO: すごく雑に int -> int16_t にしているので、全体的にどうするか考える
    block[0] = static_cast<int16_t>(decodeDCCoef(dcTable, dcDHT->symbols, dcPred));
    decodeACCoefs(acTable, acDHT->symbols, block);
    dequantize(block, *dqt);
    reorder(block);
    math::idct(block);
    levelShift(block);
}
