﻿#include "JpegDecoder.h"
#include <cassert>
#include <mdspan>
#include <memory>
#include <nameof.hpp>
#include <print>
#include "debugging.h"
#include "../math/math.h"

using namespace jpeg;
using namespace jpeg::segments;

namespace
{
    using MCUBlock8x8 = std::array<int, 64>;

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

    void convertRGB(const std::vector<int>& ys, const std::vector<int>& cbs, const std::vector<int>& crs, std::vector<uint8_t>& pixels, int width, int height)
    {
        for (int row = 0; row < height; ++row) {
            for (int col = 0; col < width; ++col) {
                int offset = row * width + col;
                int y = ys[offset];
                int cb = cbs[(row / 2) * (width / 2) + (col / 2)];
                int cr = crs[(row / 2) * (width / 2) + (col / 2)];
                cb -= 128;
                cr -= 128;
                double r = y + 1.402 * cr;
                double g = y - 0.344136 * cb - 0.714136 * cr;
                double b = y + 1.772 * cb;
                r = std::clamp(r, 0.0, 255.0);
                g = std::clamp(g, 0.0, 255.0);
                b = std::clamp(b, 0.0, 255.0);
                pixels[offset * 3 + 0] = static_cast<uint8_t>(r);
                pixels[offset * 3 + 1] = static_cast<uint8_t>(g);
                pixels[offset * 3 + 2] = static_cast<uint8_t>(b);
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

    const auto width = sof0->width;
    const auto height = sof0->height;
    const auto numComponents = sof0->numComponents;
    std::vector<uint8_t> pixels(width * height * numComponents, 0);
    std::vector<int> ys(width * height, 0);
    std::vector<int> crs((width / 2) * (height / 2), 0);
    std::vector<int> cbs((width / 2) * (height / 2), 0);

    const auto [horizFactor, vertFactor] = getMaxSamplingFactor(*sof0);
    const int mcuWidth = horizFactor * 8;
    const int mcuHeight = vertFactor * 8;
    const int mcuHorizCount = (width + mcuWidth - 1) / mcuWidth;
    const int mcuVertCount = (height + mcuHeight - 1) / mcuHeight;

    assert(mcuHorizCount * mcuWidth >= width);
    assert(mcuVertCount * mcuHeight >= height);

    // 全体を通して更新し続ける。
    // ただし、リスタートマーカーがある場合は、この値をリセットする。
    std::array<int, 3> dcPred = { 0, 0, 0 };

    for (int mcuRow = 0; mcuRow < mcuVertCount; ++mcuRow) {
        for (int mcuCol = 0; mcuCol < mcuHorizCount; ++mcuCol) {
            for (int componentIndex = 0; auto&& component : sof0->components) {
                auto [dcTable, dcDHT] = dcTables[std::to_underlying(component.tableID)];
                auto [acTable, acDHT] = acTables[std::to_underlying(component.tableID)];
                auto dqt = dqts[std::to_underlying(component.tableID)];

                // 1 MCU Y 16x16 Cb 8x8 Cr 8x8 を、 8x8 のブロックに分割して処理
                for (int blockRow = 0; blockRow < component.samplingFactorVerticalRatio; ++blockRow) {
                    for (int blockCol = 0; blockCol < component.samplingFactorHorizontalRatio; ++blockCol) {
                        //std::println("MCU: ({}, {})", mcuCol, mcuRow);
                        MCUBlock8x8 block{};
                        decodeBlock(dcTable, dcDHT, acTable, acDHT, dqt, block, dcPred[componentIndex]);
                        std::mdspan<int, std::extents<int, 8, 8>> blockView(block.data(), 8, 8);

                        for (int y = 0; y < 8; ++y) {
                            for (int x = 0; x < 8; ++x) {
                                int px = mcuCol * mcuWidth + blockCol * 8 + x;
                                int py = mcuRow * mcuHeight + blockRow * 8 + y;

                                switch (component.id) {
                                case ComponentID::Y:
                                    if (px < width && py < height) {
                                        ys[py * width + px] = blockView[y, x];
                                    }
                                    break;
                                case ComponentID::Cb:
                                case ComponentID::Cr:
                                    {
                                        int cp_x = (px / 2);
                                        int cp_y = (py / 2);
                                        if (cp_x < (width / 2) && cp_y < (height / 2)) {
                                            auto& target = (component.id == ComponentID::Cb) ? cbs : crs;
                                            target[cp_y * (width / 2) + cp_x] = blockView[y, x];
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    convertRGB(ys, cbs, crs, pixels, width, height);

    result = {
        .width = width,
        .height = height,
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
    int k = 0;

    while (k < 64) {
        int symbol = decodeHuffmanSymbol(table, symbols);
        if (symbol == 0) {
            break;
        }

        int ssss = symbol % 16;
        int rrrr = symbol >> 4;
        int r = rrrr;

        if (ssss == 0) {
            if (r == 15) {
                k += 16;
                continue;
            }
            break;
        }
        else {
            k += r;
            if (k > 63) {
                break;
            }
            zz[k] = decodeZZ(ssss);
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
