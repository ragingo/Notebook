#pragma once
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <format>
#include <ranges>

class BinaryFileReader final
{
public:
    enum class SeekOrigin
    {
        Begin,
        Current,
        End
    };

public:
    BinaryFileReader(const char* filename)
    {
        if (!std::filesystem::exists(filename)) {
            throw std::runtime_error(std::format("File not found: {}", filename));
        }

        m_Size = std::filesystem::file_size(filename);

        m_Stream.open(filename, std::ios::binary);

        if (!m_Stream.is_open()) {
            throw std::runtime_error(std::format("Failed to open file: {}", filename));
        }
    }

    ~BinaryFileReader()
    {
        m_Stream.close();
    }

    template <typename T, size_t N>
    void ReadBytes(std::array<T, N>& buffer)
    {
        m_Stream.read(reinterpret_cast<char*>(buffer.data()), sizeof(T) * N);
    }

    template <std::ranges::random_access_range T>
    void ReadBytes(T& buffer)
    {
        m_Stream.read(
            reinterpret_cast<char*>(std::data(buffer)),
            std::size(buffer) * sizeof(std::ranges::range_value_t<T>)
        );
    }

    template <typename T>
        requires (std::is_same_v<T, uint8_t> || (std::is_enum_v<T> && std::is_same_v<std::underlying_type_t<T>, uint8_t>))
    void ReadUInt8(T& buffer)
    {
        m_Stream.read(reinterpret_cast<char*>(&buffer), sizeof(T));
    }

    template <typename T>
        requires (std::is_same_v<T, uint16_t> || (std::is_enum_v<T> && std::is_same_v<std::underlying_type_t<T>, uint16_t>))
    void ReadUInt16(T& buffer)
    {
        m_Stream.read(reinterpret_cast<char*>(&buffer), sizeof(T));
        if constexpr (std::is_enum_v<T>) {
            buffer = static_cast<T>(std::byteswap(static_cast<std::underlying_type_t<T>>(buffer)));
        }
        else {
            buffer = std::byteswap(buffer);
        }
    }

    size_t GetSize() const
    {
        return m_Size;
    }

    void Seek(std::streamoff pos, SeekOrigin origin = SeekOrigin::Begin)
    {
        switch (origin) {
        using enum SeekOrigin;
        case Begin:
            m_Stream.seekg(pos, std::ios::beg);
            break;
        case Current:
            m_Stream.seekg(pos, std::ios::cur);
            break;
        case End:
            m_Stream.seekg(pos, std::ios::end);
            break;
        }
    }

    std::streamoff GetCurrentPosition()
    {
        return m_Stream.tellg();
    }

private:
    std::ifstream m_Stream;
    size_t m_Size;
};
