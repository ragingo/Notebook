#pragma once
#include <cstdint>
#include <memory>
#include <vector>

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
    std::vector<std::shared_ptr<T>> findSegments(Marker marker) const {
        std::vector<std::shared_ptr<T>> result;
        for (const auto& segment : m_Segments) {
            if (segment->marker == marker) {
                // TODO: T と marker の指定を一致させないといけないから、marker 指定が無くせるようにしたい
                result.emplace_back(std::static_pointer_cast<T>(segment));
            }
        }
        return result;
    }

private:
    BinaryFileReader m_FileReader;
    std::vector<Marker> m_Markers;
    std::vector<std::shared_ptr<segments::Segment>> m_Segments;
    std::vector<uint8_t> m_ECS = {};
};

} // namespace jpg
