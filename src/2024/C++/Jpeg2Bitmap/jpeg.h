#pragma once
#include <cstdint>
#include <format>
#include <string>

// 参考資料
// https://www.w3.org/Graphics/JPEG/jfif3.pdf
// https://www.w3.org/Graphics/JPEG/itu-t81.pdf
// https://github.com/corkami/formats/blob/master/image/jpeg.md

namespace jpg
{
    enum class Markers : uint16_t
    {
        SOI = 0xFFD8,
        APP0 = 0xFFE0,
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
        SOF0 = 0xFFC0,
        SOF1 = 0xFFC1,
        SOF2 = 0xFFC2,
        SOF3 = 0xFFC3,
        DRI = 0xFFDD,
        SOS = 0xFFDA,
        EOI = 0xFFD9,
    };

    inline uint8_t marker_lower(Markers marker)
    {
        return static_cast<uint16_t>(marker) & 0xFF;
    }

    inline uint8_t marker_upper(Markers marker)
    {
        return static_cast<uint16_t>(marker) >> 8;
    }
}

namespace jpg
{
    namespace segments
    {
        struct SOI
        {
            uint8_t reserved;
            uint8_t marker;
        };

        struct APP0
        {
            uint8_t reserved;
            uint8_t marker;
            uint16_t length;
            char identifier[5];
            uint16_t version;
            uint8_t units;
            uint16_t xDensity;
            uint16_t yDensity;
            uint8_t thumbnailWidth;
            uint8_t thumbnailHeight;
        };

        struct DQT
        {
            uint8_t reserved;
            uint8_t marker;
            uint16_t length;
            uint8_t precision : 4;
            uint8_t tableID : 4;
            uint8_t table[64];
        };
    }
}

namespace jpg {
    template<typename T>
    std::string to_string(const T& value) {}

    template<>
    std::string to_string<segments::SOI>(const segments::SOI& soi)
    {
        std::string result;
        result += "SOI\n";
        result += std::format("  reserved: 0x{:02X}\n", soi.reserved);
        result += std::format("  marker: 0x{:02X}\n", soi.marker);
        return result;
    }

    template<>
    std::string to_string<segments::APP0>(const segments::APP0& app0)
    {
        std::string result;
        result += "APP0\n";
        result += std::format("  reserved: 0x{:02X}\n", app0.reserved);
        result += std::format("  marker: 0x{:02X}\n", app0.marker);
        result += std::format("  length: 0x{:04X}\n", app0.length);
        result += std::format("  identifier: {}\n", app0.identifier);
        result += std::format("  version: 0x{:02X}\n", app0.version);
        result += std::format("  units: 0x{:02X}\n", app0.units);
        result += std::format("  xDensity: 0x{:04X}\n", app0.xDensity);
        result += std::format("  yDensity: 0x{:04X}\n", app0.yDensity);
        result += std::format("  thumbnailWidth: 0x{:02X}\n", app0.thumbnailWidth);
        result += std::format("  thumbnailHeight: 0x{:02X}\n", app0.thumbnailHeight);
        return result;
    }

    template<>
    std::string to_string<segments::DQT>(const segments::DQT& dqt)
    {
        std::string result;
        result += "DQT\n";
        result += std::format("  reserved: 0x{:02X}\n", dqt.reserved);
        result += std::format("  marker: 0x{:02X}\n", dqt.marker);
        result += std::format("  length: 0x{:04X}\n", dqt.length);
        result += std::format("  precision: 0x{:01X}\n", dqt.precision);
        result += std::format("  tableID: 0x{:01X}\n", dqt.tableID);
        result += "  table:\n";
        for (int i = 0; i < 64; ++i) {
            result += std::format("    0x{:02X}", dqt.table[i]);
            if (i % 8 == 7) {
                result += '\n';
            }
        }
        return result;
    }
}
