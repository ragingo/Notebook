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
        DRI = 0xFFDD,
        SOS = 0xFFDA,
        EOI = 0xFFD9,
    };

    inline uint8_t marker_lower(Marker marker)
    {
        return static_cast<uint16_t>(marker) & 0xFF;
    }

    inline uint8_t marker_upper(Marker marker)
    {
        return static_cast<uint16_t>(marker) >> 8;
    }

} // namespace jpeg
