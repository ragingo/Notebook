#pragma once
#include <cstdint>

namespace jpeg {

    enum class Marker : uint16_t
    {
        SOI = 0xFFD8,
        // JFIF
        APP0 = 0xFFE0,
        // EXIF
        APP1 = 0xFFE1,
        APP2 = 0xFFE2,
        APP3 = 0xFFE3,
        APP4 = 0xFFE4,
        APP5 = 0xFFE5,
        APP6 = 0xFFE6,
        APP7 = 0xFFE7,
        APP8 = 0xFFE8,
        APP9 = 0xFFE9,
        APP10 = 0xFFEA,
        APP11 = 0xFFEB,
        APP12 = 0xFFEC,
        APP13 = 0xFFED,
        APP14 = 0xFFEE,
        APP15 = 0xFFEF,
        DQT = 0xFFDB,
        DHT = 0xFFC4,
        // Baseline DCT
        SOF0 = 0xFFC0,
        // Extended sequential DCT, Huffman
        SOF1 = 0xFFC1,
        // Progressive DCT, Huffman
        SOF2 = 0xFFC2,
        // Lossless, Huffman
        SOF3 = 0xFFC3,
        // Differential sequential DCT, Huffman
        SOF5 = 0xFFC5,
        // Differential progressive DCT, Huffman
        SOF6 = 0xFFC6,
        // Differential lossless, Huffman
        SOF7 = 0xFFC7,
        // sequential DCT, arithmetic
        SOF9 = 0xFFC9,
        // Progressive DCT, arithmetic
        SOF10 = 0xFFCA,
        // Lossless, arithmetic
        SOF11 = 0xFFCB,
        // Differential sequential DCT, arithmetic
        SOF13 = 0xFFCD,
        // Differential progressive DCT, arithmetic
        SOF14 = 0xFFCE,
        // Differential lossless, arithmetic
        SOF15 = 0xFFCF,
        DNL = 0xFFDC,
        DRI = 0xFFDD,
        SOS = 0xFFDA,
        EOI = 0xFFD9,
        RST0 = 0xFFD0,
        RST1 = 0xFFD1,
        RST2 = 0xFFD2,
        RST3 = 0xFFD3,
        RST4 = 0xFFD4,
        RST5 = 0xFFD5,
        RST6 = 0xFFD6,
        RST7 = 0xFFD7,
    };

} // namespace jpeg
