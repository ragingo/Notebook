#pragma once
#include <cstdint>
#include <vector>

namespace jpeg {

    class BitStreamReader
    {
    public:
        BitStreamReader(const std::vector<uint8_t>& stream)
            : m_Stream(stream)
        {
        }

        ~BitStreamReader() = default;

        int receive(int ssss);
        uint8_t getNextBit();

    private:
        std::vector<uint8_t> m_Stream;
        uint8_t m_CurrentByte = 0;
        int m_DataIndex = 0;
        int m_ReadBitCount = 0;
    };

} // namespace jpeg
