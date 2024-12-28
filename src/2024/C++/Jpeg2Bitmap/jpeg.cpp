#include <format>
#include <iostream>
#include <print>
#include <nameof.hpp>
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

JpegDecoder::JpegDecoder(const std::string& fileName)
    : m_FileName(fileName)
    , m_Stream(fileName, std::ios::binary)
{
    if (!m_Stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }
}

JpegDecoder::~JpegDecoder()
{
    m_Stream.close();
}

void JpegDecoder::decode()
{
    parse();

    // TODO: デコード処理を書いていく
}

void JpegDecoder::parse()
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

void JpegDecoder::parseSOI()
{
    m_SOI.reserved = jpg::marker_upper(jpg::Marker::SOI);
    m_SOI.marker = jpg::marker_lower(jpg::Marker::SOI);
    std::cout << jpg::to_string(m_SOI) << std::endl;
}

void JpegDecoder::parseAPP0()
{
    m_APP0.reserved = jpg::marker_upper(jpg::Marker::APP0);
    m_APP0.marker = jpg::marker_lower(jpg::Marker::APP0);
    m_Stream.read(reinterpret_cast<char*>(&m_APP0.length), sizeof(m_APP0.length));
    m_APP0.length = std::byteswap(m_APP0.length);
    int remain = m_APP0.length - sizeof(m_APP0.length);
    if (remain >= sizeof(m_APP0.identifier)) {
        m_Stream.read(reinterpret_cast<char*>(&m_APP0.identifier), sizeof(m_APP0.identifier));
        remain -= sizeof(m_APP0.identifier);
    }
    if (remain >= sizeof(m_APP0.version)) {
        m_Stream.read(reinterpret_cast<char*>(&m_APP0.version), sizeof(m_APP0.version));
        m_APP0.version = static_cast<decltype(m_APP0.version)>(std::byteswap(static_cast<uint16_t>(m_APP0.version)));
        remain -= sizeof(m_APP0.version);
    }
    if (remain >= sizeof(m_APP0.units)) {
        m_Stream.read(reinterpret_cast<char*>(&m_APP0.units), sizeof(m_APP0.units));
        remain -= sizeof(m_APP0.units);
    }
    if (remain >= sizeof(m_APP0.xDensity) + sizeof(m_APP0.yDensity)) {
        m_Stream.read(reinterpret_cast<char*>(&m_APP0.xDensity), sizeof(m_APP0.xDensity));
        m_Stream.read(reinterpret_cast<char*>(&m_APP0.yDensity), sizeof(m_APP0.yDensity));
        m_APP0.xDensity = std::byteswap(m_APP0.xDensity);
        m_APP0.yDensity = std::byteswap(m_APP0.yDensity);
        remain -= sizeof(m_APP0.xDensity) + sizeof(m_APP0.yDensity);
    }
    if (remain >= sizeof(m_APP0.thumbnailWidth) + sizeof(m_APP0.thumbnailHeight)) {
        m_Stream.read(reinterpret_cast<char*>(&m_APP0.thumbnailWidth), sizeof(m_APP0.thumbnailWidth));
        m_Stream.read(reinterpret_cast<char*>(&m_APP0.thumbnailHeight), sizeof(m_APP0.thumbnailHeight));
        remain -= sizeof(m_APP0.thumbnailWidth) + sizeof(m_APP0.thumbnailHeight);
    }

    std::cout << jpg::to_string(m_APP0) << std::endl;
}

void JpegDecoder::parseDQT()
{
    m_DQT.reserved = jpg::marker_upper(jpg::Marker::DQT);
    m_DQT.marker = jpg::marker_lower(jpg::Marker::DQT);
    m_Stream.read(reinterpret_cast<char*>(&m_DQT.length), sizeof(m_DQT.length));
    m_DQT.length = std::byteswap(m_DQT.length);
    int remain = m_DQT.length - sizeof(m_DQT.length);
    if (remain >= 1) {
        uint8_t value;
        m_Stream.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
        m_DQT.precision = static_cast<segments::DQT::Precision>(value >> 4);
        m_DQT.tableID = value & 0x0F;
        remain--;
    }

    switch (m_DQT.precision) {
    case segments::DQT::Precision::BITS_8:
        if (remain >= sizeof(m_DQT.table8)) {
            m_Stream.read(reinterpret_cast<char*>(&m_DQT.table8), sizeof(m_DQT.table8));
            remain -= sizeof(m_DQT.table8);
        }
        break;
    case segments::DQT::Precision::BITS_16:
        if (remain >= sizeof(m_DQT.table16)) {
            m_Stream.read(reinterpret_cast<char*>(&m_DQT.table16), sizeof(m_DQT.table16));
            remain -= sizeof(m_DQT.table16);
        }
        break;
    default:
        break;
    }

    std::cout << jpg::to_string(m_DQT) << std::endl;
}

void JpegDecoder::parseSOF0()
{
    m_SOF0.reserved = jpg::marker_upper(jpg::Marker::SOF0);
    m_SOF0.marker = jpg::marker_lower(jpg::Marker::SOF0);
    m_Stream.read(reinterpret_cast<char*>(&m_SOF0.length), sizeof(m_SOF0.length));
    m_SOF0.length = std::byteswap(m_SOF0.length);
    int remain = m_SOF0.length - sizeof(m_SOF0.length);
    if (remain >= 1) {
        m_Stream.read(reinterpret_cast<char*>(&m_SOF0.precision), sizeof(m_SOF0.precision));
        remain--;
    }
    if (remain >= sizeof(m_SOF0.height) + sizeof(m_SOF0.width)) {
        m_Stream.read(reinterpret_cast<char*>(&m_SOF0.height), sizeof(m_SOF0.height));
        m_Stream.read(reinterpret_cast<char*>(&m_SOF0.width), sizeof(m_SOF0.width));
        m_SOF0.height = std::byteswap(m_SOF0.height);
        m_SOF0.width = std::byteswap(m_SOF0.width);
        remain -= sizeof(m_SOF0.height) + sizeof(m_SOF0.width);
    }
    if (remain >= 1) {
        m_Stream.read(reinterpret_cast<char*>(&m_SOF0.numComponents), sizeof(m_SOF0.numComponents));
        remain--;
    }
    m_SOF0.components.resize(m_SOF0.numComponents);
    for (int i = 0; i < m_SOF0.numComponents; ++i) {
        if (remain >= sizeof(m_SOF0.components[i])) {
            m_Stream.read(reinterpret_cast<char*>(&m_SOF0.components[i]), sizeof(m_SOF0.components[i]));
            remain -= sizeof(m_SOF0.components[i]);
        }
    }

    std::cout << jpg::to_string(m_SOF0) << std::endl;
}

void JpegDecoder::parseDHT()
{
    m_DHT.reserved = jpg::marker_upper(jpg::Marker::DHT);
    m_DHT.marker = jpg::marker_lower(jpg::Marker::DHT);
    m_Stream.read(reinterpret_cast<char*>(&m_DHT.length), sizeof(m_DHT.length));
    m_DHT.length = std::byteswap(m_DHT.length);
    int remain = m_DHT.length - sizeof(m_DHT.length);
    if (remain >= 1) {
        uint8_t value;
        m_Stream.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
        m_DHT.tableClass = static_cast<segments::DHT::TableClass>(value >> 4);
        m_DHT.tableID = value & 0xFF;
        remain--;
    }
    if (remain >= sizeof(m_DHT.counts)) {
        m_Stream.read(reinterpret_cast<char*>(&m_DHT.counts), sizeof(m_DHT.counts));
        remain -= sizeof(m_DHT.counts);
    }
    if (remain > 0) {
        m_DHT.symbols.resize(remain);
        m_Stream.read(reinterpret_cast<char*>(m_DHT.symbols.data()), remain);
        remain = 0;
    }

    std::cout << jpg::to_string(m_DHT) << std::endl;
}

void JpegDecoder::parseSOS()
{
    m_SOS.reserved = jpg::marker_upper(jpg::Marker::SOS);
    m_SOS.marker = jpg::marker_lower(jpg::Marker::SOS);
    m_Stream.read(reinterpret_cast<char*>(&m_SOS.length), sizeof(m_SOS.length));
    m_SOS.length = std::byteswap(m_SOS.length);
    int remain = m_SOS.length - sizeof(m_SOS.length);
    if (remain >= 1) {
        m_Stream.read(reinterpret_cast<char*>(&m_SOS.numComponents), sizeof(m_SOS.numComponents));
        remain--;
    }
    m_SOS.components.resize(m_SOS.numComponents);
    for (int i = 0; i < m_SOS.numComponents; ++i) {
        if (remain >= sizeof(m_SOS.components[i])) {
            m_Stream.read(reinterpret_cast<char*>(&m_SOS.components[i]), sizeof(m_SOS.components[i]));
            remain -= sizeof(m_SOS.components[i]);
        }
    }
    if (remain >= 3) {
        m_Stream.read(reinterpret_cast<char*>(&m_SOS.spectralSelectionStart), sizeof(m_SOS.spectralSelectionStart));
        m_Stream.read(reinterpret_cast<char*>(&m_SOS.spectralSelectionEnd), sizeof(m_SOS.spectralSelectionEnd));
        m_Stream.read(reinterpret_cast<char*>(&m_SOS.successiveApproximation), sizeof(m_SOS.successiveApproximation));
        remain -= 3;
    }

    std::cout << jpg::to_string(m_SOS) << std::endl;

    parseECS();
}

void JpegDecoder::parseECS()
{
    auto current_pos = m_Stream.tellg();
    m_Stream.seekg(-2, std::ios::end);
    auto end_pos = m_Stream.tellg();

    if (readMarker(m_Stream) != Marker::EOI) {
        std::cerr << "Invalid JPEG file" << std::endl;
        return;
    }

    m_Stream.seekg(current_pos);

    m_ECS.resize(end_pos - current_pos);
    m_Stream.read(reinterpret_cast<char*>(m_ECS.data()), m_ECS.size());

    std::cout << jpg::to_string(m_ECS) << std::endl;
}

void JpegDecoder::parseEOI()
{
    m_EOI.reserved = jpg::marker_upper(jpg::Marker::EOI);
    m_EOI.marker = jpg::marker_lower(jpg::Marker::EOI);
    std::cout << jpg::to_string(m_EOI) << std::endl;
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
        result += std::format("  version: 0x{:02X} ({})\n", static_cast<uint16_t>(app0.version), NAMEOF_ENUM(app0.version));
        result += std::format("  units: 0x{:02X} ({})\n", static_cast<uint8_t>(app0.units), NAMEOF_ENUM(app0.units));
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
        result += std::format("  precision: 0x{:01X} ({})\n", static_cast<uint8_t>(dqt.precision), NAMEOF_ENUM(dqt.precision));
        result += std::format("  tableID: 0x{:01X}\n", dqt.tableID);
        result += "  table:\n";
        for (int i = 0; i < 64; ++i) {
            switch (dqt.precision) {
            case segments::DQT::Precision::BITS_8:
                result += std::format("    0x{:02X}", dqt.table8[i]);
                break;
            case segments::DQT::Precision::BITS_16:
                result += std::format("    0x{:04X}", dqt.table16[i]);
                break;
            }
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
            result += std::format("    id: 0x{:02X} ({})\n", static_cast<uint8_t>(sof0.components[i].id), NAMEOF_ENUM(sof0.components[i].id));
            result += std::format("    samplingFactorHorizontalRatio: 0x{:02X}\n", sof0.components[i].samplingFactorHorizontalRatio);
            result += std::format("    samplingFactorVerticalRatio: 0x{:02X}\n", sof0.components[i].samplingFactorVerticalRatio);
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
        result += std::format("  tableClass: 0x{:02X} ({})\n", static_cast<uint8_t>(dht.tableClass), NAMEOF_ENUM(dht.tableClass));
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
            result += std::format("    selector: 0x{:02X}\n", sos.components[i].componentSelector);
            result += std::format("    dcSelector: 0x{:02X}\n", sos.components[i].dcSelector);
            result += std::format("    acSelector: 0x{:02X}\n", sos.components[i].acSelector);
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
