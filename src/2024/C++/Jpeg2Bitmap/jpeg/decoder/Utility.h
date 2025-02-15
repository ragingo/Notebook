#pragma once
#include <concepts>
#include <cstdint>
#include <memory>
#include <ranges>
#include <tuple>
#include <utility>
#include <vector>
#include "jpeg/syntax/Segment.h"

namespace jpeg
{
    template<typename T>
        requires std::derived_from<T, segments::Segment>
    inline T* segment_cast(segments::Segment* segment) noexcept
    {
        return dynamic_cast<T*>(segment);
    }

    template<typename T>
        requires std::derived_from<T, segments::Segment>
    inline std::shared_ptr<T> segment_cast(std::shared_ptr<segments::Segment> segment) noexcept
    {
        return std::dynamic_pointer_cast<T>(segment);
    }

    template <typename T>
        requires std::derived_from<T, segments::Segment>
    inline std::vector<std::shared_ptr<T>> findSegments(std::vector<std::shared_ptr<segments::Segment>> segments)
    {
        return segments
            | std::views::transform([](const auto& segment) { return segment_cast<T>(segment); })
            | std::views::filter([](const auto& segment) { return segment != nullptr; })
            | std::ranges::to<std::vector>();
    }

    template <typename T>
        requires std::derived_from<T, segments::Segment>
    inline std::shared_ptr<T> findFirstSegment(std::vector<std::shared_ptr<segments::Segment>> segments)
    {
        auto view = segments
            | std::views::transform([](const auto& segment) { return segment_cast<T>(segment); })
            | std::views::filter([](const auto& segment) { return segment != nullptr; })
            | std::views::take(1);

        return std::ranges::empty(view) ? nullptr : *std::ranges::begin(view);
    }

    inline segments::ColorSpace getColorSpace(const segments::SOF0& sof0)
    {
        auto ids = sof0.components
            | std::ranges::views::transform([](const auto& c) { return c.id; })
            | std::ranges::to<std::vector>();

        if (ids == std::vector{ segments::ComponentID::Y, segments::ComponentID::Cb, segments::ComponentID::Cr }) {
            return segments::ColorSpace::YCbCr;
        }
        if (ids == std::vector{ segments::ComponentID::Y, segments::ComponentID::I, segments::ComponentID::Q }) {
            return segments::ColorSpace::YIQ;
        }
        if (ids.size() == 4) {
            return segments::ColorSpace::CMYK;
        }
        if (ids.size() == 1) {
            return segments::ColorSpace::GRAYSCALE;
        }
        return segments::ColorSpace::UNKNOWN;
    }

    inline std::tuple<uint8_t, uint8_t> getMaxSamplingFactor(const segments::SOF0& sof0)
    {
        uint8_t h = 0;
        uint8_t v = 0;
        for (const auto& component : sof0.components) {
            h = std::max(h, component.horizonalSamplingFactor);
            v = std::max(v, component.verticalSamplingFactor);
        }
        return { h, v };
    }

    inline std::tuple<uint8_t, uint8_t> getSamplingFactor(const segments::SOF0& sof0, segments::ComponentID id)
    {
        for (const auto& component : sof0.components) {
            if (component.id == id) {
                return { component.horizonalSamplingFactor, component.verticalSamplingFactor };
            }
        }
        return { 0ui8, 0ui8 };
    }

    inline bool isInterleaved(const segments::SOF0& sof0)
    {
        uint8_t prev_h = 0;
        uint8_t prev_v = 0;
        for (auto i = 0; i < sof0.components.size(); ++i) {
            auto& component = sof0.components[i];
            if (i == 0) {
                prev_h = component.horizonalSamplingFactor;
                prev_v = component.verticalSamplingFactor;
                continue;
            }
            if (prev_h != component.horizonalSamplingFactor || prev_v != component.verticalSamplingFactor) {
                return true;
            }
        }
        return false;
    }

    inline segments::YUVFormat getYUVFormat(const segments::SOF0& sof0)
    {
        switch (sof0.numComponents) {
        case 1:
            return segments::YUVFormat::YUV400;
        case 3:
            if (sof0.components[0].horizonalSamplingFactor == 1 && sof0.components[0].verticalSamplingFactor == 1 &&
                sof0.components[1].horizonalSamplingFactor == 1 && sof0.components[1].verticalSamplingFactor == 1 &&
                sof0.components[2].horizonalSamplingFactor == 1 && sof0.components[2].verticalSamplingFactor == 1) {
                return segments::YUVFormat::YUV444;
            }
            if (sof0.components[0].horizonalSamplingFactor == 2 && sof0.components[0].verticalSamplingFactor == 1 &&
                sof0.components[1].horizonalSamplingFactor == 1 && sof0.components[1].verticalSamplingFactor == 1 &&
                sof0.components[2].horizonalSamplingFactor == 1 && sof0.components[2].verticalSamplingFactor == 1) {
                return segments::YUVFormat::YUV422;
            }
            if (sof0.components[0].horizonalSamplingFactor == 2 && sof0.components[0].verticalSamplingFactor == 2 &&
                sof0.components[1].horizonalSamplingFactor == 1 && sof0.components[1].verticalSamplingFactor == 1 &&
                sof0.components[2].horizonalSamplingFactor == 1 && sof0.components[2].verticalSamplingFactor == 1) {
                return segments::YUVFormat::YUV420;
            }
            if (sof0.components[0].horizonalSamplingFactor == 4 && sof0.components[0].verticalSamplingFactor == 1 &&
                sof0.components[1].horizonalSamplingFactor == 1 && sof0.components[1].verticalSamplingFactor == 1 &&
                sof0.components[2].horizonalSamplingFactor == 1 && sof0.components[2].verticalSamplingFactor == 1) {
                return segments::YUVFormat::YUV411;
            }
            if (sof0.components[0].horizonalSamplingFactor == 4 && sof0.components[0].verticalSamplingFactor == 4 &&
                sof0.components[1].horizonalSamplingFactor == 1 && sof0.components[1].verticalSamplingFactor == 1 &&
                sof0.components[2].horizonalSamplingFactor == 1 && sof0.components[2].verticalSamplingFactor == 1) {
                return segments::YUVFormat::YUV410;
            }
            return segments::YUVFormat::UNKNOWN;
        default:
            return segments::YUVFormat::UNKNOWN;
        }
    }

} // namespace jpeg
