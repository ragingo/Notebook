﻿#include "JpegDecoder.h"
#include <cassert>
#include <mdspan>
#include <memory>
#include <nameof.hpp>
#include <print>
#include "debugging.h"
#include "../math/math.h"
#include "../bitmap.h"

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
}

namespace
{
    void writeBitmap(const std::string& filename, int width, int height, const std::vector<uint8_t>& pixels)
    {
        BitmapFileHeader fileHeader{};
        BitmapInfoHeader infoHeader{};

        fileHeader.bfType = 0x4D42;
        fileHeader.bfSize = static_cast<uint32_t>(sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + pixels.size());
        fileHeader.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

        infoHeader.biSize = sizeof(BitmapInfoHeader);
        infoHeader.biWidth = width;
        infoHeader.biHeight = -height;
        infoHeader.biPlanes = 1;
        infoHeader.biBitCount = 24;
        infoHeader.biSizeImage = static_cast<uint32_t>(pixels.size());

        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            return;
        }

        file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
        file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
        file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    }
}

JpegDecoder::JpegDecoder(const char* fileName)
    : m_Parser(fileName)
{
}

JpegDecoder::~JpegDecoder()
{
}

void JpegDecoder::decode()
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

    auto getDCTable = [&](int componentIndex) -> std::tuple<HuffmanTable, std::shared_ptr<DHT>>& {
        auto component = sof0->components[componentIndex];
        return dcTables[std::to_underlying(component.tableID)];
    };

    auto getACTable = [&](int componentIndex) -> std::tuple<HuffmanTable, std::shared_ptr<DHT>>& {
        auto component = sof0->components[componentIndex];
        return acTables[std::to_underlying(component.tableID)];
    };

    const auto width = sof0->width;
    const auto height = sof0->height;
    const auto numComponents = sof0->numComponents;
    std::vector<uint8_t> pixels(width * height * numComponents);

    // TODO: pixels の更新

    writeBitmap("d:\\temp\\images\\x.bmp", width, height, pixels);
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
