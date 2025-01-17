﻿#include "BitStreamReader.h"
#include <stdexcept>

namespace jpeg {

    // Figure F.17 - Procedure for RECEIVE(SSSS)
    uint8_t BitStreamReader::receive(int ssss)
    {
        uint8_t v = 0;
        for (int i = 0; i < ssss; ++i) {
            v = (v << 1) + getNextBit();
        }
        return v;
    }

    // Figure F.18 – Procedure for fetching the next bit of compressed data
    uint8_t BitStreamReader::getNextBit()
    {
        uint8_t b = 0;

        if (m_ReadBitCount == 0) {
            b = m_Stream[++m_DataIndex];
            m_ReadBitCount = 8;
            if (b == 0xFF) {
                uint8_t b2 = m_Stream[++m_DataIndex];
                if (b2 != 0x00) {
                    // TODO: DNL マーカー対応
                    throw std::runtime_error("Invalid JPEG file");
                }
            }
        }

        uint8_t bit = b >> 7;
        m_ReadBitCount--;
        b <<= 1;

        return bit;
    }

} // namespace jpeg
