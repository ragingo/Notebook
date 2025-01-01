#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <ranges>
#include <variant>
#include <vector>
#include "markers.h"

namespace jpeg { namespace segments {

    enum class QuantizationTableID : uint8_t
    {
        LUMINANCE = 0x0,
        CHROMINANCE = 0x1,
        RESERVED_2 = 0x2,
        RESERVED_3 = 0x3,
    };

    enum class HuffmanTableID : uint8_t
    {
        LUMINANCE = 0x0,
        CHROMINANCE = 0x1,
        RESERVED_2 = 0x2,
        RESERVED_3 = 0x3,
    };

    enum class ComponentID : uint8_t
    {
        Y = 1,
        Cb = 2,
        Cr = 3,
        I = 4,
        Q = 5,
    };


    struct Segment
    {
        Marker marker;
        virtual ~Segment() = default;
    };

    struct SOI : public Segment
    {
    };

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
        QuantizationTableID tableID : 4;

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
            ComponentID id;
            uint8_t samplingFactorHorizontalRatio : 4;
            uint8_t samplingFactorVerticalRatio : 4;
            QuantizationTableID tableID;
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
        HuffmanTableID tableID : 4;
        std::array<uint8_t, 16> counts;
        std::vector<uint8_t> symbols;
    };

    struct SOS : public Segment
    {
        uint16_t length;
        uint8_t numComponents;

        struct Component
        {
            ComponentID componentSelector;
            HuffmanTableID dcSelector : 4;
            HuffmanTableID acSelector : 4;
        };
        std::vector<Component> components;

        uint8_t spectralSelectionStart;
        uint8_t spectralSelectionEnd;
        uint8_t successiveApproximation;
    };

    struct EOI : public Segment {};


    template<typename T>
        requires std::derived_from<T, Segment>
    inline T* segment_cast(Segment* segment) noexcept
    {
        return dynamic_cast<T*>(segment);
    }

    template<typename T>
        requires std::derived_from<T, Segment>
    inline std::shared_ptr<T> segment_cast(std::shared_ptr<Segment> segment) noexcept
    {
        return std::dynamic_pointer_cast<T>(segment);
    }

    template <typename T>
        requires std::derived_from<T, segments::Segment>
    std::vector<std::shared_ptr<T>> findSegments(std::vector<std::shared_ptr<segments::Segment>> segments)
    {
        return segments
            | std::views::transform([](const auto& segment) { return segments::segment_cast<T>(segment); })
            | std::views::filter([](const auto& segment) { return segment != nullptr; })
            | std::ranges::to<std::vector>();
    }

    template <typename T>
        requires std::derived_from<T, segments::Segment>
    std::shared_ptr<T> findFirstSegment(std::vector<std::shared_ptr<segments::Segment>> segments)
    {
        auto view = segments
            | std::views::transform([](const auto& segment) { return segments::segment_cast<T>(segment); })
            | std::views::filter([](const auto& segment) { return segment != nullptr; })
            | std::views::take(1);

        return std::ranges::empty(view) ? nullptr : *std::ranges::begin(view);
    }

} } // namespace jpg::segments
