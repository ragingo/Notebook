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
    enum class Marker : uint16_t
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

    inline uint8_t marker_lower(Marker marker)
    {
        return static_cast<uint16_t>(marker) & 0xFF;
    }

    inline uint8_t marker_upper(Marker marker)
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

        struct SOF0
        {
            uint8_t reserved;
            uint8_t marker;
            uint16_t length;
            uint8_t precision;
            uint16_t height;
            uint16_t width;
            uint8_t numComponents;

            struct Component
            {
                uint8_t id;
                uint8_t samplingFactor;
                uint8_t quantizationTableID;
            } components[3];
        };

        struct DHT
        {
            uint8_t reserved;
            uint8_t marker;
            uint16_t length;
            uint8_t tableID;
            uint8_t counts[16];
            std::vector<uint8_t> symbols;
        };

        struct SOS
        {
            uint8_t reserved;
            uint8_t marker;
            uint16_t length;
            uint8_t numComponents;

            struct Component
            {
                uint8_t id;
                uint8_t huffmanTable;
            } components[3];

            uint8_t spectralSelectionStart;
            uint8_t spectralSelectionEnd;
            uint8_t successiveApproximation;
        };

        struct EOI
        {
            uint8_t reserved;
            uint8_t marker;
        };
    }

    class JpegDecoder final
    {
    public:
        JpegDecoder(const std::string& fileName);
        ~JpegDecoder();

        void decode();

    private:
        void parse();
        void parseSOI();
        void parseAPP0();
        void parseDQT();
        void parseSOF0();
        void parseEOI();
        void parseDHT();
        void parseSOS();
        void parseECS();

    private:
        std::string m_FileName;
        std::ifstream m_Stream;
        segments::SOI m_SOI = {};
        segments::APP0 m_APP0 = {};
        segments::DQT m_DQT = {};
        segments::SOF0 m_SOF0 = {};
        segments::EOI m_EOI = {};
        segments::DHT m_DHT = {};
        segments::SOS m_SOS = {};
        std::vector<uint8_t> m_ECS = {};
    };
}

namespace jpg {
    std::string to_string(const segments::SOI& soi);
    std::string to_string(const segments::APP0& app0);
    std::string to_string(const segments::DQT& dqt);
    std::string to_string(const segments::SOF0& sof0);
    std::string to_string(const segments::DHT& dht);
    std::string to_string(const segments::SOS& sos);
    std::string to_string(const std::vector<uint8_t>& ecs);
    std::string to_string(const segments::EOI& eoi);
}
