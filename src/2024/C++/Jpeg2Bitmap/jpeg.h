#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

// 参考資料
// https://www.w3.org/Graphics/JPEG/jfif3.pdf
// https://www.w3.org/Graphics/JPEG/itu-t81.pdf
// https://github.com/corkami/formats/blob/master/image/jpeg.md

namespace jpg
{
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
            enum Version : uint16_t
            {
                UNKNOWN = 0,
                V1_1 = 0x101,
                V1_2 = 0x102,
            };

            enum class Units : uint8_t
            {
                NONE = 0,
                PPI = 1,
                PPCM = 2,
            };

            uint8_t reserved;
            uint8_t marker;
            uint16_t length;
            char identifier[5];
            Version version;
            Units units;
            uint16_t xDensity;
            uint16_t yDensity;
            uint8_t thumbnailWidth;
            uint8_t thumbnailHeight;
        };

        struct DQT
        {
            enum class Precision : uint8_t
            {
                BITS_8 = 0,
                BITS_16 = 1,
            };

            uint8_t reserved;
            uint8_t marker;
            uint16_t length;
            Precision precision : 4;
            uint8_t tableID : 4;

            union
            {
                uint8_t table8[64];
                uint16_t table16[64];
            };
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
                enum class ID : uint8_t
                {
                    Y = 1,
                    Cb = 2,
                    Cr = 3,
                    I = 4,
                    Q = 5,
                };
                ID id;
                uint8_t samplingFactorHorizontalRatio : 4;
                uint8_t samplingFactorVerticalRatio : 4;
                uint8_t quantizationTableID;
            };
            std::vector<Component> components;
        };

        struct DHT
        {
            enum class TableClass : uint8_t
            {
                DC_OR_LOSSLESS = 0,
                AC = 1,
            };
            uint8_t reserved;
            uint8_t marker;
            uint16_t length;
            TableClass tableClass : 4;
            uint8_t tableID : 4;
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
                uint8_t componentSelector;
                uint8_t dcSelector : 4;
                uint8_t acSelector : 4;
            };
            std::vector<Component> components;

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
        void dumpSummary();

    private:
        std::string m_FileName;
        std::ifstream m_Stream;
        std::vector<Marker> m_Markers;

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
