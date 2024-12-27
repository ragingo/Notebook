#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <format>
#include <print>
#include "jpeg.h"

using namespace jpg;

namespace
{
    std::vector<uint8_t> readBytes(std::ifstream& stream, int count)
    {
        std::vector<uint8_t> bytes(count);
        stream.read(reinterpret_cast<char*>(bytes.data()), count);
        return bytes;
    }

    jpg::Marker readMarker(std::ifstream& stream)
    {
        uint16_t marker;
        stream.read(reinterpret_cast<char*>(&marker), sizeof(marker));
        marker = std::byteswap(marker);
        return static_cast<jpg::Marker>(marker);
    }
}

JpegParser::JpegParser(const std::string& fileName)
    : m_FileName(fileName)
    , m_Stream(fileName, std::ios::binary)
{
    if (!m_Stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }
}

JpegParser::~JpegParser()
{
    m_Stream.close();
}

void JpegParser::parse()
{
    auto marker = readMarker(m_Stream);

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
        marker = readMarker(m_Stream);
    }
}

void JpegParser::parseSOI()
{
    auto soi = jpg::segments::SOI{};
    soi.reserved = jpg::marker_upper(jpg::Marker::SOI);
    soi.marker = jpg::marker_lower(jpg::Marker::SOI);
    std::cout << jpg::to_string(soi) << std::endl;
}

void JpegParser::parseAPP0()
{
    auto app0 = jpg::segments::APP0{};
    app0.reserved = jpg::marker_upper(jpg::Marker::APP0);
    app0.marker = jpg::marker_lower(jpg::Marker::APP0);
    m_Stream.read(reinterpret_cast<char*>(&app0.length), sizeof(app0.length));
    app0.length = std::byteswap(app0.length);
    int remain = app0.length - sizeof(app0.length);
    if (remain >= sizeof(app0.identifier)) {
        m_Stream.read(reinterpret_cast<char*>(&app0.identifier), sizeof(app0.identifier));
        remain -= sizeof(app0.identifier);
    }
    if (remain >= sizeof(app0.version)) {
        m_Stream.read(reinterpret_cast<char*>(&app0.version), sizeof(app0.version));
        app0.version = std::byteswap(app0.version);
        remain -= sizeof(app0.version);
    }
    if (remain >= sizeof(app0.units)) {
        m_Stream.read(reinterpret_cast<char*>(&app0.units), sizeof(app0.units));
        remain -= sizeof(app0.units);
    }
    if (remain >= sizeof(app0.xDensity) + sizeof(app0.yDensity)) {
        m_Stream.read(reinterpret_cast<char*>(&app0.xDensity), sizeof(app0.xDensity));
        m_Stream.read(reinterpret_cast<char*>(&app0.yDensity), sizeof(app0.yDensity));
        app0.xDensity = std::byteswap(app0.xDensity);
        app0.yDensity = std::byteswap(app0.yDensity);
        remain -= sizeof(app0.xDensity) + sizeof(app0.yDensity);
    }
    if (remain >= sizeof(app0.thumbnailWidth) + sizeof(app0.thumbnailHeight)) {
        m_Stream.read(reinterpret_cast<char*>(&app0.thumbnailWidth), sizeof(app0.thumbnailWidth));
        m_Stream.read(reinterpret_cast<char*>(&app0.thumbnailHeight), sizeof(app0.thumbnailHeight));
        remain -= sizeof(app0.thumbnailWidth) + sizeof(app0.thumbnailHeight);
    }

    std::cout << jpg::to_string(app0) << std::endl;
}

void JpegParser::parseDQT()
{
    auto dqt = jpg::segments::DQT{};
    dqt.reserved = jpg::marker_upper(jpg::Marker::DQT);
    dqt.marker = jpg::marker_lower(jpg::Marker::DQT);
    m_Stream.read(reinterpret_cast<char*>(&dqt.length), sizeof(dqt.length));
    dqt.length = std::byteswap(dqt.length);
    int remain = dqt.length - sizeof(dqt.length);
    if (remain >= 1) {
        uint8_t value;
        m_Stream.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
        dqt.precision = value >> 4;
        dqt.tableID = value & 0x0F;
        remain--;
    }
    if (remain >= sizeof(dqt.table)) {
        m_Stream.read(reinterpret_cast<char*>(&dqt.table), sizeof(dqt.table));
        remain -= sizeof(dqt.table);
    }

    std::cout << jpg::to_string(dqt) << std::endl;
}

void JpegParser::parseSOF0()
{
    auto sof0 = jpg::segments::SOF0{};
    sof0.reserved = jpg::marker_upper(jpg::Marker::SOF0);
    sof0.marker = jpg::marker_lower(jpg::Marker::SOF0);
    m_Stream.read(reinterpret_cast<char*>(&sof0.length), sizeof(sof0.length));
    sof0.length = std::byteswap(sof0.length);
    int remain = sof0.length - sizeof(sof0.length);
    if (remain >= 1) {
        m_Stream.read(reinterpret_cast<char*>(&sof0.precision), sizeof(sof0.precision));
        remain--;
    }
    if (remain >= sizeof(sof0.height) + sizeof(sof0.width)) {
        m_Stream.read(reinterpret_cast<char*>(&sof0.height), sizeof(sof0.height));
        m_Stream.read(reinterpret_cast<char*>(&sof0.width), sizeof(sof0.width));
        sof0.height = std::byteswap(sof0.height);
        sof0.width = std::byteswap(sof0.width);
        remain -= sizeof(sof0.height) + sizeof(sof0.width);
    }
    if (remain >= 1) {
        m_Stream.read(reinterpret_cast<char*>(&sof0.numComponents), sizeof(sof0.numComponents));
        remain--;
    }
    for (int i = 0; i < sof0.numComponents; ++i) {
        if (remain >= sizeof(sof0.components[i])) {
            m_Stream.read(reinterpret_cast<char*>(&sof0.components[i]), sizeof(sof0.components[i]));
            remain -= sizeof(sof0.components[i]);
        }
    }

    std::cout << jpg::to_string(sof0) << std::endl;
}

void JpegParser::parseDHT()
{
    auto dht = jpg::segments::DHT{};
    dht.reserved = jpg::marker_upper(jpg::Marker::DHT);
    dht.marker = jpg::marker_lower(jpg::Marker::DHT);
    m_Stream.read(reinterpret_cast<char*>(&dht.length), sizeof(dht.length));
    dht.length = std::byteswap(dht.length);
    int remain = dht.length - sizeof(dht.length);
    if (remain >= 1) {
        uint8_t value;
        m_Stream.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
        dht.tableID = value;
        remain--;
    }
    if (remain >= sizeof(dht.counts)) {
        m_Stream.read(reinterpret_cast<char*>(&dht.counts), sizeof(dht.counts));
        remain -= sizeof(dht.counts);
    }
    if (remain > 0) {
        dht.symbols.resize(remain);
        m_Stream.read(reinterpret_cast<char*>(dht.symbols.data()), remain);
        remain = 0;
    }

    std::cout << jpg::to_string(dht) << std::endl;
}

void JpegParser::parseSOS()
{
    auto sos = jpg::segments::SOS{};
    sos.reserved = jpg::marker_upper(jpg::Marker::SOS);
    sos.marker = jpg::marker_lower(jpg::Marker::SOS);
    m_Stream.read(reinterpret_cast<char*>(&sos.length), sizeof(sos.length));
    sos.length = std::byteswap(sos.length);
    int remain = sos.length - sizeof(sos.length);
    if (remain >= 1) {
        m_Stream.read(reinterpret_cast<char*>(&sos.numComponents), sizeof(sos.numComponents));
        remain--;
    }
    for (int i = 0; i < sos.numComponents; ++i) {
        if (remain >= sizeof(sos.components[i])) {
            m_Stream.read(reinterpret_cast<char*>(&sos.components[i]), sizeof(sos.components[i]));
            remain -= sizeof(sos.components[i]);
        }
    }
    if (remain >= 3) {
        m_Stream.read(reinterpret_cast<char*>(&sos.spectralSelectionStart), sizeof(sos.spectralSelectionStart));
        m_Stream.read(reinterpret_cast<char*>(&sos.spectralSelectionEnd), sizeof(sos.spectralSelectionEnd));
        m_Stream.read(reinterpret_cast<char*>(&sos.successiveApproximation), sizeof(sos.successiveApproximation));
        remain -= 3;
    }

    std::cout << jpg::to_string(sos) << std::endl;

    parseECS();
}

void JpegParser::parseECS()
{
    auto current_pos = m_Stream.tellg();
    m_Stream.seekg(-2, std::ios::end);
    auto end_pos = m_Stream.tellg();

    if (readMarker(m_Stream) != Marker::EOI) {
        std::cerr << "Invalid JPEG file" << std::endl;
        return;
    }

    m_Stream.seekg(current_pos);

    std::vector<uint8_t> ecs(end_pos - current_pos);
    m_Stream.read(reinterpret_cast<char*>(ecs.data()), ecs.size());

    std::cout << jpg::to_string(ecs) << std::endl;
}

void JpegParser::parseEOI()
{
    auto eoi = jpg::segments::EOI{};
    eoi.reserved = jpg::marker_upper(jpg::Marker::EOI);
    eoi.marker = jpg::marker_lower(jpg::Marker::EOI);
    std::cout << jpg::to_string(eoi) << std::endl;
}

namespace jpg
{
    std::string to_string(const segments::SOI& soi)
    {
        std::string result;
        result += "SOI\n";
        result += std::format("  reserved: 0x{:02X}\n", soi.reserved);
        result += std::format("  marker: 0x{:02X}\n", soi.marker);
        return result;
    }

    std::string to_string(const segments::APP0& app0)
    {
        std::string result;
        result += "APP0\n";
        result += std::format("  reserved: 0x{:02X}\n", app0.reserved);
        result += std::format("  marker: 0x{:02X}\n", app0.marker);
        result += std::format("  length: 0x{:04X}\n", app0.length);
        result += std::format("  identifier: {}\n", app0.identifier);
        result += std::format("  version: 0x{:02X}\n", app0.version);
        result += std::format("  units: 0x{:02X}\n", app0.units);
        result += std::format("  xDensity: 0x{:04X}\n", app0.xDensity);
        result += std::format("  yDensity: 0x{:04X}\n", app0.yDensity);
        result += std::format("  thumbnailWidth: 0x{:02X}\n", app0.thumbnailWidth);
        result += std::format("  thumbnailHeight: 0x{:02X}\n", app0.thumbnailHeight);
        return result;
    }

    std::string to_string(const segments::DQT& dqt)
    {
        std::string result;
        result += "DQT\n";
        result += std::format("  reserved: 0x{:02X}\n", dqt.reserved);
        result += std::format("  marker: 0x{:02X}\n", dqt.marker);
        result += std::format("  length: 0x{:04X}\n", dqt.length);
        result += std::format("  precision: 0x{:01X}\n", dqt.precision);
        result += std::format("  tableID: 0x{:01X}\n", dqt.tableID);
        result += "  table:\n";
        for (int i = 0; i < 64; ++i) {
            result += std::format("    0x{:02X}", dqt.table[i]);
            if (i % 8 == 7) {
                result += '\n';
            }
        }
        return result;
    }

    std::string to_string(const segments::SOF0& sof0)
    {
        std::string result;
        result += "SOF0\n";
        result += std::format("  reserved: 0x{:02X}\n", sof0.reserved);
        result += std::format("  marker: 0x{:02X}\n", sof0.marker);
        result += std::format("  length: 0x{:04X}\n", sof0.length);
        result += std::format("  precision: 0x{:01X}\n", sof0.precision);
        result += std::format("  height: 0x{:04X}\n", sof0.height);
        result += std::format("  width: 0x{:04X}\n", sof0.width);
        result += std::format("  numComponents: 0x{:02X}\n", sof0.numComponents);
        for (int i = 0; i < sof0.numComponents; ++i) {
            result += std::format("  Component[{}]:\n", i);
            result += std::format("    id: 0x{:02X}\n", sof0.components[i].id);
            result += std::format("    samplingFactor: 0x{:02X}\n", sof0.components[i].samplingFactor);
            result += std::format("    quantizationTableID: 0x{:02X}\n", sof0.components[i].quantizationTableID);
        }
        return result;
    }

    std::string to_string(const segments::DHT& dht)
    {
        std::string result;
        result += "DHT\n";
        result += std::format("  reserved: 0x{:02X}\n", dht.reserved);
        result += std::format("  marker: 0x{:02X}\n", dht.marker);
        result += std::format("  length: 0x{:04X}\n", dht.length);
        result += std::format("  tableID: 0x{:02X}\n", dht.tableID);
        result += "  counts:\n";
        for (int i = 0; i < 16; ++i) {
            result += std::format("    0x{:02X}", dht.counts[i]);
            if (i % 8 == 7) {
                result += '\n';
            }
        }
        result += "  symbols:\n";
        for (int i = 0; i < dht.symbols.size(); ++i) {
            result += std::format("    0x{:02X}", dht.symbols[i]);
            if (i % 8 == 7) {
                result += '\n';
            }
        }
        return result;
    }

    std::string to_string(const segments::SOS& sos)
    {
        std::string result;
        result += "SOS\n";
        result += std::format("  reserved: 0x{:02X}\n", sos.reserved);
        result += std::format("  marker: 0x{:02X}\n", sos.marker);
        result += std::format("  length: 0x{:04X}\n", sos.length);
        result += std::format("  numComponents: 0x{:02X}\n", sos.numComponents);
        for (int i = 0; i < sos.numComponents; ++i) {
            result += std::format("  Component[{}]:\n", i);
            result += std::format("    id: 0x{:02X}\n", sos.components[i].id);
            result += std::format("    huffmanTable: 0x{:02X}\n", sos.components[i].huffmanTable);
        }
        result += std::format("  spectralSelectionStart: 0x{:02X}\n", sos.spectralSelectionStart);
        result += std::format("  spectralSelectionEnd: 0x{:02X}\n", sos.spectralSelectionEnd);
        result += std::format("  successiveApproximation: 0x{:02X}\n", sos.successiveApproximation);
        return result;
    }

    std::string to_string(const std::vector<uint8_t>& ecs)
    {
        std::string result;
        result += "ECS: \n";
        for (int i = 0; i < ecs.size(); ++i) {
            result += std::format("{:02X}", ecs[i]);
            if (i % 32 == 31) {
                result += '\n';
            }
        }
        result += "\n";
        return result;
    }

    std::string to_string(const segments::EOI& eoi)
    {
        std::string result;
        result += "EOI\n";
        result += std::format("  reserved: 0x{:02X}\n", eoi.reserved);
        result += std::format("  marker: 0x{:02X}\n", eoi.marker);
        return result;
    }
}
