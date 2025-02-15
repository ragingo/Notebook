#pragma once
#include <cstdint>
#include <vector>

namespace jpeg {

    class BitStreamReader
    {
    public:
        BitStreamReader(const std::vector<uint8_t>& stream)
            : m_Stream(stream)
            , m_StreamSize(stream.size())
            , m_StreamLastIndex(stream.size() - 1)
        {
        }

        ~BitStreamReader() = default;

        int receive(int ssss);
        uint8_t nextBit();
        uint8_t nextByte();

        inline bool hasMore() const noexcept
        {
            return m_DataIndex < m_StreamLastIndex;
        }

    private:
        std::vector<uint8_t> m_Stream;
        uint8_t m_CurrentByte = 0;
        int m_DataIndex = 0;
        int m_ReadBitCount = 0;
        const size_t m_StreamSize;
        const size_t m_StreamLastIndex;
    };

} // namespace jpeg
