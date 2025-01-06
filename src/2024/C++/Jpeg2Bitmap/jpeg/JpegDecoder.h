#pragma once
#include <cstdint>
#include <memory>
#include <vector>

#include "segments.h"
#include "BitStreamReader.h"
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

private:
    BinaryFileReader m_FileReader;
    std::vector<Marker> m_Markers;
    std::vector<std::shared_ptr<segments::Segment>> m_Segments;
    std::vector<uint8_t> m_ECS = {};
    std::unique_ptr<BitStreamReader> m_BitStreamReader;
};

} // namespace jpg
