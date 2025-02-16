#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <vector>
#include "jpeg/BitStreamReader.h"
#include "jpeg/parser/JpegParser.h"
#include "jpeg/decoder/Common.h"

namespace jpeg {

struct DecodeResult
{
    int width;
    int height;
    std::vector<uint8_t> pixels;
};

class JpegDecoder final
{
public:
    JpegDecoder(const char* fileName);
    ~JpegDecoder();

    void decode(DecodeResult& result);

private:
    struct HuffmanTable
    {
        std::array<int, 16> minCode{};
        std::array<int, 16> maxCode{};
        std::array<int, 16> valPtr{};

        HuffmanTable()
        {
            maxCode.fill(-1);
        }
    };

    // Figure C.1 – Generation of table of Huffman code sizes
    // counts (BITS) には、各ビット長(1～16)のハフマン符号の数が格納されている。
    // 例えば、 counts[] = { 0, 5, 1, 1 } の場合、
    // ハフマン符号のビット長は 1ビットが5つ、2ビットが1つ、3ビットが1つとなる。
    // input: counts[0] = 0, counts[1] = 5, counts[2] = 1, counts[3] = 1
    // output: { 1, 1, 1, 1, 1, 2, 3 }
    std::vector<int> createHuffSize(const std::array<uint8_t, 16>& counts);

    // Figure C.2 – Generation of table of Huffman codes
    // ハフマン符号のビット長からハフマン符号を生成する。
    // 例えば、 huffSize[] = { 2, 2, 3, 3, 3, 5 } の場合、
    // ハフマン符号は { 00, 01, 100, 101, 110, 11111 } となる。
    std::vector<int> createHuffCode(const std::vector<int>& huffSize);

    HuffmanTable createHuffmanTable(const std::array<uint8_t, 16>& counts);

    // Figure F.16 – Procedure for DECODE
    int decodeHuffmanSymbol(HuffmanTable& table, const std::vector<uint8_t>& symbols);

    // Figure F.14 – Decoding a non-zero AC coefficient
    inline int decodeZZ(int ssss);

    // Figure F.13 – Huffman decoding procedure for AC coefficients
    void decodeACCoefs(
        HuffmanTable& table,
        const std::vector<uint8_t>& symbols,
        MCUBlock8x8& block
    );

    // F.2.2.1 Huffman decoding of DC coefficients
    int decodeDCCoef(HuffmanTable& table, const std::vector<uint8_t>& symbols, int& pred);

    void decodeBlock(
        HuffmanTable& dcTable,
        std::shared_ptr<segments::DHT> dcDHT,
        HuffmanTable& acTable,
        std::shared_ptr<segments::DHT> acDHT,
        std::shared_ptr<segments::DQT> dqt,
        MCUBlock8x8& block,
        int& dcPred
    );
private:
    JpegParser m_Parser;
    std::unique_ptr<BitStreamReader> m_BitStreamReader;
};

} // namespace jpeg
