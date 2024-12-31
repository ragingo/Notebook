#include <algorithm>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <vector>

#include "jpeg.h"
#include "JpegDecoder.h"
#include "debugging.h"

using namespace jpeg;

JpegDecoder::JpegDecoder(const char* fileName)
    : m_FileReader(fileName)
{
}

JpegDecoder::~JpegDecoder()
{
}

void JpegDecoder::decode()
{
    parse();

    dumpSummary();

    // TODO: デコード処理を書いていく
}

void JpegDecoder::parse()
{
    Marker marker;
    m_FileReader.ReadUInt16(marker);
    m_Markers.push_back(marker);

    if (marker != Marker::SOI) {
        throw std::runtime_error("Invalid JPEG file");
    }

    while (true) {
        switch (marker) {
        case Marker::SOI:
            parseSOI();
            break;
        case Marker::APP0:
            parseAPP0();
            break;
        case Marker::DQT:
            parseDQT();
            break;
        case Marker::SOF0:
            parseSOF0();
            break;
        case Marker::EOI:
            parseEOI();
            return;
        case Marker::DHT:
            parseDHT();
            break;
        case Marker::SOS:
            parseSOS();
            break;
        default:
            std::println("Unknown marker: 0x{:02X}", static_cast<uint16_t>(marker));
            return;
        }
        m_FileReader.ReadUInt16(marker);
        m_Markers.push_back(marker);
    }
}

void JpegDecoder::parseSOI()
{
    m_SOI.reserved = jpeg::marker_upper(jpeg::Marker::SOI);
    m_SOI.marker = jpeg::marker_lower(jpeg::Marker::SOI);
    std::cout << jpeg::debugging::to_string(m_SOI) << std::endl;
}

void JpegDecoder::parseAPP0()
{
    m_APP0.reserved = jpeg::marker_upper(jpeg::Marker::APP0);
    m_APP0.marker = jpeg::marker_lower(jpeg::Marker::APP0);
    m_FileReader.ReadUInt16(m_APP0.length);
    int remain = m_APP0.length - sizeof(m_APP0.length);
    if (remain >= sizeof(m_APP0.identifier)) {
        m_FileReader.ReadBytes(m_APP0.identifier);
        remain -= sizeof(m_APP0.identifier);
    }
    if (remain >= sizeof(m_APP0.version)) {
        m_FileReader.ReadUInt16(m_APP0.version);
        remain -= sizeof(m_APP0.version);
    }
    if (remain >= sizeof(m_APP0.units)) {
        m_FileReader.ReadUInt8(m_APP0.units);
        remain -= sizeof(m_APP0.units);
    }
    if (remain >= sizeof(m_APP0.xDensity) + sizeof(m_APP0.yDensity)) {
        m_FileReader.ReadUInt16(m_APP0.xDensity);
        m_FileReader.ReadUInt16(m_APP0.yDensity);
        remain -= sizeof(m_APP0.xDensity) + sizeof(m_APP0.yDensity);
    }
    if (remain >= sizeof(m_APP0.thumbnailWidth) + sizeof(m_APP0.thumbnailHeight)) {
        m_FileReader.ReadUInt8(m_APP0.thumbnailWidth);
        m_FileReader.ReadUInt8(m_APP0.thumbnailHeight);
        remain -= sizeof(m_APP0.thumbnailWidth) + sizeof(m_APP0.thumbnailHeight);
    }

    std::cout << jpeg::debugging::to_string(m_APP0) << std::endl;
}

void JpegDecoder::parseDQT()
{
    m_DQT.reserved = jpeg::marker_upper(jpeg::Marker::DQT);
    m_DQT.marker = jpeg::marker_lower(jpeg::Marker::DQT);
    m_FileReader.ReadUInt16(m_DQT.length);
    int remain = m_DQT.length - sizeof(m_DQT.length);
    if (remain >= 1) {
        uint8_t value;
        m_FileReader.ReadUInt8(value);
        m_DQT.precision = static_cast<segments::DQT::Precision>(value >> 4);
        m_DQT.tableID = value & 0x0F;
        remain--;
    }

    switch (m_DQT.precision) {
    case segments::DQT::Precision::BITS_8:
        if (int size = sizeof(std::get<segments::DQT::Bits8Table>(m_DQT.table)); remain >= size)
        {
            m_FileReader.ReadBytes(std::get<segments::DQT::Bits8Table>(m_DQT.table));
            remain -= size;
        }

        break;
    case segments::DQT::Precision::BITS_16:
        if (int size = sizeof(std::get<segments::DQT::Bits16Table>(m_DQT.table)); remain >= size) {
            m_FileReader.ReadBytes(std::get<segments::DQT::Bits16Table>(m_DQT.table));
            remain -= size;
        }
        break;
    default:
        break;
    }

    std::cout << jpeg::debugging::to_string(m_DQT) << std::endl;
}

void JpegDecoder::parseSOF0()
{
    m_SOF0.reserved = jpeg::marker_upper(jpeg::Marker::SOF0);
    m_SOF0.marker = jpeg::marker_lower(jpeg::Marker::SOF0);
    m_FileReader.ReadUInt16(m_SOF0.length);
    int remain = m_SOF0.length - sizeof(m_SOF0.length);
    if (remain >= 1) {
        m_FileReader.ReadUInt8(m_SOF0.precision);
        remain--;
    }
    if (remain >= sizeof(m_SOF0.height) + sizeof(m_SOF0.width)) {
        m_FileReader.ReadUInt16(m_SOF0.height);
        m_FileReader.ReadUInt16(m_SOF0.width);
        remain -= sizeof(m_SOF0.height) + sizeof(m_SOF0.width);
    }
    if (remain >= 1) {
        m_FileReader.ReadUInt8(m_SOF0.numComponents);
        remain--;
    }
    m_SOF0.components.resize(m_SOF0.numComponents);
    if (remain >= sizeof(m_SOF0.components[0]) * m_SOF0.numComponents) {
        m_FileReader.ReadBytes(m_SOF0.components);
        remain -= sizeof(m_SOF0.components[0]) * m_SOF0.numComponents;
    }

    std::cout << jpeg::debugging::to_string(m_SOF0) << std::endl;
}

void JpegDecoder::parseDHT()
{
    m_DHT.reserved = jpeg::marker_upper(jpeg::Marker::DHT);
    m_DHT.marker = jpeg::marker_lower(jpeg::Marker::DHT);
    m_FileReader.ReadUInt16(m_DHT.length);
    int remain = m_DHT.length - sizeof(m_DHT.length);
    if (remain >= 1) {
        uint8_t value;
        m_FileReader.ReadUInt8(value);
        m_DHT.tableClass = static_cast<segments::DHT::TableClass>(value >> 4);
        m_DHT.tableID = value & 0xFF;
        remain--;
    }
    if (remain >= sizeof(m_DHT.counts)) {
        m_FileReader.ReadBytes(m_DHT.counts);
        remain -= sizeof(m_DHT.counts);
    }
    if (remain > 0) {
        m_DHT.symbols.resize(remain);
        m_FileReader.ReadBytes(m_DHT.symbols);
        remain = 0;
    }

    std::cout << jpeg::debugging::to_string(m_DHT) << std::endl;
}

void JpegDecoder::parseSOS()
{
    m_SOS.reserved = jpeg::marker_upper(jpeg::Marker::SOS);
    m_SOS.marker = jpeg::marker_lower(jpeg::Marker::SOS);
    m_FileReader.ReadUInt16(m_SOS.length);
    int remain = m_SOS.length - sizeof(m_SOS.length);
    if (remain >= 1) {
        m_FileReader.ReadUInt8(m_SOS.numComponents);
        remain--;
    }
    m_SOS.components.resize(m_SOS.numComponents);
    if (remain >= sizeof(m_SOS.components[0]) * m_SOS.numComponents) {
        m_FileReader.ReadBytes(m_SOS.components);
        remain -= sizeof(m_SOS.components[0]) * m_SOS.numComponents;
    }
    if (remain >= 3) {
        m_FileReader.ReadUInt8(m_SOS.spectralSelectionStart);
        m_FileReader.ReadUInt8(m_SOS.spectralSelectionEnd);
        m_FileReader.ReadUInt8(m_SOS.successiveApproximation);
        remain -= 3;
    }

    std::cout << jpeg::debugging::to_string(m_SOS) << std::endl;

    parseECS();
}

void JpegDecoder::parseECS()
{
    auto current_pos = m_FileReader.GetCurrentPosition();
    m_FileReader.Seek(-2, BinaryFileReader::SeekOrigin::End);
    auto end_pos = m_FileReader.GetCurrentPosition();

    Marker marker;
    m_FileReader.ReadUInt16(marker);

    if (marker != Marker::EOI) {
        std::cerr << "Invalid JPEG file" << std::endl;
        return;
    }

    m_FileReader.Seek(current_pos);

    m_ECS.resize(end_pos - current_pos);
    m_FileReader.ReadBytes(m_ECS);

    std::cout << jpeg::debugging::to_string(m_ECS) << std::endl;
}

void JpegDecoder::parseEOI()
{
    m_EOI.reserved = jpeg::marker_upper(jpeg::Marker::EOI);
    m_EOI.marker = jpeg::marker_lower(jpeg::Marker::EOI);
    std::cout << jpeg::debugging::to_string(m_EOI) << std::endl;
}

void JpegDecoder::dumpSummary()
{
    std::string result;

    result += "==================================================\n";
    result += std::format("File Size: {} bytes\n", m_FileReader.GetSize());

    // dimensions
    if (std::ranges::contains(m_Markers, Marker::SOF0)) {
        result += std::format("Dimensions: {}x{}\n", m_SOF0.width, m_SOF0.height);
    }

    if (std::ranges::contains(m_Markers, Marker::SOF0)) {
        result += "Frame Type: Baseline\n";
    }

    result += "==================================================";

    std::cout << result << std::endl;
}
