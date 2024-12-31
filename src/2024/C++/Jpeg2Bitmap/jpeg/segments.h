#pragma once
#include <array>
#include <cstdint>
#include <variant>
#include <vector>

namespace jpeg { namespace segments {

    struct Segment
    {
        uint8_t reserved;
        uint8_t marker;
    };

    struct SOI : public Segment {};

    struct APP0 : public Segment
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

        uint16_t length;
        std::array<char, 5> identifier;
        Version version;
        Units units;
        uint16_t xDensity;
        uint16_t yDensity;
        uint8_t thumbnailWidth;
        uint8_t thumbnailHeight;
    };

    struct DQT : public Segment
    {
        enum class Precision : uint8_t
        {
            BITS_8 = 0,
            BITS_16 = 1,
        };

        uint16_t length;
        Precision precision : 4;
        uint8_t tableID : 4;

        using Bits8Table = std::array<uint8_t, 64>;
        using Bits16Table = std::array<uint16_t, 64>;

        std::variant<Bits8Table, Bits16Table> table;
    };

    struct SOF0 : public Segment
    {
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

    struct DHT : public Segment
    {
        enum class TableClass : uint8_t
        {
            DC_OR_LOSSLESS = 0,
            AC = 1,
        };
        uint16_t length;
        TableClass tableClass : 4;
        uint8_t tableID : 4;
        std::array<uint8_t, 16> counts;
        std::vector<uint8_t> symbols;
    };

    struct SOS : public Segment
    {
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

    struct EOI : public Segment {};

} } // namespace jpg::segments
