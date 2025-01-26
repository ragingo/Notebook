#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <ranges>
#include <variant>
#include <vector>
#include "markers.h"

namespace jpeg::segments {

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

    enum class ColorSpace
    {
        GRAYSCALE,
        YCbCr,
        YIQ,
        CMYK,
        UNKNOWN,
    };

    enum class YUVFormat
    {
        YUV444,
        YUV422,
        YUV420,
        YUV411,
        YUV410,
        YUV400,
        UNKNOWN,
    };

    struct Segment
    {
        Marker marker;
        virtual ~Segment() = default;
    };

    // Start of Image
    // SOI マーカーは、JPEG ファイルの開始を示す。
    // MotionJPEG では、連続する JPEG 画像の区切りを示すために使用される。
    struct SOI : public Segment
    {
    };

    // JFIF Extension APP0 Marker Segment
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
            DPI = 1,
            DPCM = 2,
        };

        // length
        uint16_t length;
        // identifier
        std::array<char, 5> identifier;
        // version
        Version version;
        // units
        Units units;
        // Xdensity
        uint16_t xDensity;
        // Ydensity
        uint16_t yDensity;
        // Xthumbnail
        uint8_t thumbnailWidth;
        // Ythumbnail
        uint8_t thumbnailHeight;

        // TODO: thumbnail (RGB 24bit, Xthumbnail * Ythumbnail * 3 pixels)
    };

    // B.2.4.1 Quantization table-specification syntax
    struct DQT : public Segment
    {
        enum class Precision : uint8_t
        {
            BITS_8 = 0,
            BITS_16 = 1,
        };

        // Lq: Quantization table definition length
        uint16_t length;
        // Pq: Quantization table element precision
        Precision precision : 4;
        // Tq: Quantization table destination identifier
        QuantizationTableID tableID : 4;

        using Bits8Table = std::array<uint8_t, 64>;
        using Bits16Table = std::array<uint16_t, 64>;
        // Qk: Quantization table element
        std::variant<Bits8Table, Bits16Table> table;
    };

    // B.2.2 Frame header syntax
    // Baseline DCT
    struct SOF0 : public Segment
    {
        // Lf: Frame header length
        uint16_t length;
        // P: Sample precision
        uint8_t precision;
        // Y: Number of lines
        uint16_t height;
        // X: Number of samples per line
        uint16_t width;
        // Nf: Number of image components in frame
        uint8_t numComponents;

        struct Component
        {
            // Ci: Component identifier
            ComponentID id;
            // Hi: Horizontal sampling factor
            uint8_t samplingFactorHorizontalRatio : 4;
            // Vi: Vertical sampling factor
            uint8_t samplingFactorVerticalRatio : 4;
            // Tqi: Quantization table destination selector
            QuantizationTableID tableID;
        };
        // Component-specification parameters
        std::vector<Component> components;
    };

    // B.2.4.2 Huffman table-specification syntax
    struct DHT : public Segment
    {
        enum class TableClass : uint8_t
        {
            DC_OR_LOSSLESS = 0,
            AC = 1,
        };

        // Lh: Huffman table definition length
        uint16_t length;
        // Tc: Table class
        TableClass tableClass : 4;
        // Th: Huffman table destination identifier
        HuffmanTableID tableID : 4;
        // Li: Number of Huffman codes of length i
        // BITS の要素が Li
        std::array<uint8_t, 16> counts;
        // Vi,j: Value associated with each Huffman code
        // HUFFVAL の要素が Vi,j
        std::vector<uint8_t> symbols;
    };

    // B.2.3 Scan header syntax
    struct SOS : public Segment
    {
        // Ls: Scan header length
        uint16_t length;
        // Ns: Number of image components in scan
        uint8_t numComponents;

        struct Component
        {
            // Csj: Scan component selector
            ComponentID componentSelector;
            // Tdj: DC entropy coding table destination selector
            HuffmanTableID dcSelector : 4;
            // Taj: AC entropy coding table destination selector
            HuffmanTableID acSelector : 4;
        };
        // Scan component-specification parameters
        std::vector<Component> components;

        // Ss: Start of spectral or predictor selection
        uint8_t spectralSelectionStart;
        // Se: End of spectral selection
        uint8_t spectralSelectionEnd;
        // Ah: Successive approximation bit position high
        uint8_t successiveApproximation;
    };

    // End of Image
    // EOI マーカーは、JPEG ファイルの終了を示す。
    // MotionJPEG では、連続する JPEG 画像の区切りを示すために使用される。
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

    constexpr inline ColorSpace getColorSpace(const SOF0& sof0)
    {
        auto ids = sof0.components
            | std::ranges::views::transform([](const auto& c) { return c.id; })
            | std::ranges::to<std::vector>();
        
        if (ids == std::vector{ ComponentID::Y, ComponentID::Cb, ComponentID::Cr }) {
            return ColorSpace::YCbCr;
        }
        if (ids == std::vector{ ComponentID::Y, ComponentID::I, ComponentID::Q }) {
            return ColorSpace::YIQ;
        }
        if (ids.size() == 4) {
            return ColorSpace::CMYK;
        }
        if (ids.size() == 1) {
            return ColorSpace::GRAYSCALE;
        }
        return ColorSpace::UNKNOWN;
    }

    inline std::tuple<uint8_t, uint8_t> getMaxSamplingFactor(const SOF0& sof0)
    {
        uint8_t h = 0;
        uint8_t v = 0;
        for (const auto& component : sof0.components) {
            h = std::max(h, component.samplingFactorHorizontalRatio);
            v = std::max(v, component.samplingFactorVerticalRatio);
        }
        return { h, v };
    }

    inline std::tuple<uint8_t, uint8_t> getSamplingFactor(const SOF0& sof0, ComponentID id)
    {
        for (const auto& component : sof0.components) {
            if (component.id == id) {
                return { component.samplingFactorHorizontalRatio, component.samplingFactorVerticalRatio };
            }
        }
        return { 0, 0 };
    }

    inline bool isInterleaved(const SOF0& sof0)
    {
        uint8_t prev_h = 0;
        uint8_t prev_v = 0;
        for (auto i = 0; i < sof0.components.size(); ++i) {
            auto& component = sof0.components[i];
            if (i == 0) {
                prev_h = component.samplingFactorHorizontalRatio;
                prev_v = component.samplingFactorVerticalRatio;
                continue;
            }
            if (prev_h != component.samplingFactorHorizontalRatio || prev_v != component.samplingFactorVerticalRatio) {
                return true;
            }
        }
        return false;
    }

    inline YUVFormat getYUVFormat(const SOF0& sof0)
    {
        switch (sof0.numComponents)
        {
        case 1:
            return YUVFormat::YUV400;
        case 3:
            if (sof0.components[0].samplingFactorHorizontalRatio == 1 && sof0.components[0].samplingFactorVerticalRatio == 1 &&
                sof0.components[1].samplingFactorHorizontalRatio == 1 && sof0.components[1].samplingFactorVerticalRatio == 1 &&
                sof0.components[2].samplingFactorHorizontalRatio == 1 && sof0.components[2].samplingFactorVerticalRatio == 1) {
                return YUVFormat::YUV444;
            }
            if (sof0.components[0].samplingFactorHorizontalRatio == 2 && sof0.components[0].samplingFactorVerticalRatio == 1 &&
                sof0.components[1].samplingFactorHorizontalRatio == 1 && sof0.components[1].samplingFactorVerticalRatio == 1 &&
                sof0.components[2].samplingFactorHorizontalRatio == 1 && sof0.components[2].samplingFactorVerticalRatio == 1) {
                return YUVFormat::YUV422;
            }
            if (sof0.components[0].samplingFactorHorizontalRatio == 2 && sof0.components[0].samplingFactorVerticalRatio == 2 &&
                sof0.components[1].samplingFactorHorizontalRatio == 1 && sof0.components[1].samplingFactorVerticalRatio == 1 &&
                sof0.components[2].samplingFactorHorizontalRatio == 1 && sof0.components[2].samplingFactorVerticalRatio == 1) {
                return YUVFormat::YUV420;
            }
            if (sof0.components[0].samplingFactorHorizontalRatio == 4 && sof0.components[0].samplingFactorVerticalRatio == 1 &&
                sof0.components[1].samplingFactorHorizontalRatio == 1 && sof0.components[1].samplingFactorVerticalRatio == 1 &&
                sof0.components[2].samplingFactorHorizontalRatio == 1 && sof0.components[2].samplingFactorVerticalRatio == 1) {
                return YUVFormat::YUV411;
            }
            if (sof0.components[0].samplingFactorHorizontalRatio == 4 && sof0.components[0].samplingFactorVerticalRatio == 4 &&
                sof0.components[1].samplingFactorHorizontalRatio == 1 && sof0.components[1].samplingFactorVerticalRatio == 1 &&
                sof0.components[2].samplingFactorHorizontalRatio == 1 && sof0.components[2].samplingFactorVerticalRatio == 1) {
                return YUVFormat::YUV410;
            }
            return YUVFormat::UNKNOWN;
        default:
            return YUVFormat::UNKNOWN;
        }
    }

} // namespace jpg::segments
