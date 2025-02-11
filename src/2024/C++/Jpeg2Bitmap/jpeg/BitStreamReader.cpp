#include "BitStreamReader.h"
#include <print>
#include "segments.h"

namespace jpeg {

    // Figure F.17 - Procedure for RECEIVE(SSSS)
    int BitStreamReader::receive(int ssss)
    {
        int v = 0;
        for (int i = 0; i < ssss; ++i) {
            v = (v << 1) + nextBit();
        }
        return v;
    }

    // Figure F.18 – Procedure for fetching the next bit of compressed data
    uint8_t BitStreamReader::nextBit()
    {
        if (m_ReadBitCount == 0) {
            if (m_DataIndex + 1 >= m_Stream.size()) {
                return 0;
            }
            m_CurrentByte = m_Stream[++m_DataIndex];
            if (m_CurrentByte == 0xFF) {
                uint8_t byte = m_Stream[++m_DataIndex];
                // DNL マーカーの暫定対応
                // あまり使用されないらしい。とりあえず2バイトスキップしておく。
                if (byte == 0xDC) {
                    std::println("* DNL");
                    nextByte();
                    nextByte();
                }
                // RST マーカー
                // デコーダー側で処理するから、ここでは何もしない。
                // デコーダー側で処理されなかったら、ここでログが記録される。
                else if (byte >= 0xD0 && byte <= 0xD7) {
                    std::println("* RST{}", byte - 0xD0);
                }
                // マーカーの後に 0x00 が続く場合はスキップ
                else if (byte == 0x00) {
                    // 何もしない
                }
                else {
                    std::println("Unknown marker: 0xFF{:02X}", byte);
                }
            }
            m_ReadBitCount = 8;
        }

        uint8_t bit = m_CurrentByte >> 7;
        m_ReadBitCount--;
        m_CurrentByte <<= 1;

        return bit;
    }

    uint8_t BitStreamReader::nextByte()
    {
        m_ReadBitCount = 0;

        if (m_DataIndex + 1 < m_Stream.size()) {
            m_CurrentByte = m_Stream[++m_DataIndex];
        }
        else {
            m_CurrentByte = 0;
        }

        return m_CurrentByte;
    }

} // namespace jpeg
