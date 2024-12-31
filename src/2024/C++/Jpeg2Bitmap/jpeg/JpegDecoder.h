#pragma once
#include <cstdint>
#include <string>
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

private:
    BinaryFileReader m_FileReader;
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

} // namespace jpg
