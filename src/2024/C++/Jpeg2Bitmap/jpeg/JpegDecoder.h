#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <ranges>

#include "segments.h"
#include "../BinaryFileReader.h"

namespace jpeg {

class JpegDecoder final
{
public:
    JpegDecoder(const char* fileName);
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

    template <typename T>
        requires std::derived_from<T, segments::Segment>
    std::vector<std::shared_ptr<T>> findSegments() const {
        return m_Segments
            | std::views::transform([](const auto& segment) { return segments::segment_cast<T>(segment); })
            | std::views::filter([](const auto& segment) { return segment != nullptr; })
            | std::ranges::to<std::vector>();
    }

    template <typename T>
        requires std::derived_from<T, segments::Segment>
    std::shared_ptr<T> findFirstSegment() const
    {
        auto view = m_Segments
            | std::views::transform([](const auto& segment) { return segments::segment_cast<T>(segment); })
            | std::views::filter([](const auto& segment) { return segment != nullptr; })
            | std::views::take(1);

        return std::ranges::empty(view) ? nullptr : *std::ranges::begin(view);
    }

private:
    BinaryFileReader m_FileReader;
    std::vector<Marker> m_Markers;
    std::vector<std::shared_ptr<segments::Segment>> m_Segments;
    std::vector<uint8_t> m_ECS = {};
};

} // namespace jpg
