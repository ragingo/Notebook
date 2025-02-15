#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "jpeg/syntax/Marker.h"
#include "jpeg/syntax/Segment.h"
#include "BinaryFileReader.h"

namespace jpeg {

class JpegParser final
{
public:
    JpegParser(const char* fileName);
    ~JpegParser();

    void parse();

    inline auto getMarkers() const noexcept { return m_Markers; }
    inline auto getSegments() const noexcept { return m_Segments; }
    inline auto getECS() const noexcept { return m_ECS; }

private:
    void parseSOI();
    void parseAPP0();
    void parseDQT();
    void parseSOF0();
    void parseEOI();
    void parseDHT();
    void parseSOS();
    void parseECS();
    void parseDRI();

    void parseAPP1();
    void parseAPP13();
    void parseAPP14();

private:
    BinaryFileReader m_FileReader;
    std::vector<Marker> m_Markers;
    std::vector<std::shared_ptr<segments::Segment>> m_Segments;
    std::vector<uint8_t> m_ECS = {};
};

} // namespace jpg
