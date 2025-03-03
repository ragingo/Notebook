#include "JpegParser.h"

#include <cstdint>
#include <memory>
#include <print>
#include <stdexcept>
#include <variant>
#include <vector>
#include "BinaryFileReader.h"
#include "jpeg/syntax/Marker.h"
#include "jpeg/syntax/Segment.h"

using namespace jpeg;
using namespace jpeg::segments;

JpegParser::JpegParser(const char* fileName)
    : m_FileReader(fileName)
{
}

JpegParser::~JpegParser()
{
}

void JpegParser::parse()
{
    Marker marker{};
    m_FileReader.ReadUInt16(marker);
    m_Markers.push_back(marker);

    if (marker != Marker::SOI) {
        throw std::runtime_error("Invalid JPEG file");
    }

    while (true) {
        using enum Marker;
        switch (marker) {
        case SOI:
            parseSOI();
            break;
        case APP0:
            parseAPP0();
            break;
        case DQT:
            parseDQT();
            break;
        case SOF0:
            parseSOF0();
            break;
        case EOI:
            parseEOI();
            return;
        case DHT:
            parseDHT();
            break;
        case SOS:
            parseSOS();
            break;
        case DRI:
            parseDRI();
            break;
        case APP1:
            parseAPP1();
            break;
        case APP2:
            parseAPP2();
            break;
        case APP13:
            parseAPP13();
            break;
        case APP14:
            parseAPP14();
            break;
        case APP3:
        case APP4:
        case APP5:
        case APP6:
        case APP7:
        case APP8:
        case APP9:
        case APP10:
        case APP11:
        case APP12:
        case APP15:
            std::println("Unsupported marker: 0x{:02X}", static_cast<uint16_t>(marker));
            break; // 無視
        case COM:
            parseCOM();
            break;
        default:
            std::println("Unknown marker: 0x{:02X}", static_cast<uint16_t>(marker));
            return; // 中断
        }
        m_FileReader.ReadUInt16(marker);
        m_Markers.push_back(marker);
    }
}

void JpegParser::parseSOI()
{
    auto soi = SOI{};
    soi.marker = Marker::SOI;
    m_Segments.emplace_back(std::make_shared<SOI>(soi));
}

void JpegParser::parseAPP0()
{
    auto app0 = APP0{};
    app0.marker = Marker::APP0;
    m_FileReader.ReadUInt16(app0.length);

    int remain = app0.length - sizeof(app0.length);
    if (remain >= sizeof(app0.identifier)) {
        m_FileReader.ReadBytes(app0.identifier);
        remain -= sizeof(app0.identifier);
    }
    if (remain >= sizeof(app0.version)) {
        m_FileReader.ReadUInt16(app0.version);
        remain -= sizeof(app0.version);
    }
    if (remain >= sizeof(app0.units)) {
        m_FileReader.ReadUInt8(app0.units);
        remain -= sizeof(app0.units);
    }
    if (remain >= sizeof(app0.xDensity) + sizeof(app0.yDensity)) {
        m_FileReader.ReadUInt16(app0.xDensity);
        m_FileReader.ReadUInt16(app0.yDensity);
        remain -= sizeof(app0.xDensity) + sizeof(app0.yDensity);
    }
    if (remain >= sizeof(app0.thumbnailWidth) + sizeof(app0.thumbnailHeight)) {
        m_FileReader.ReadUInt8(app0.thumbnailWidth);
        m_FileReader.ReadUInt8(app0.thumbnailHeight);
        remain -= sizeof(app0.thumbnailWidth) + sizeof(app0.thumbnailHeight);
    }
    m_Segments.emplace_back(std::make_shared<APP0>(app0));
}

void JpegParser::parseDQT()
{
    auto dqt = DQT{};
    dqt.marker = Marker::DQT;
    m_FileReader.ReadUInt16(dqt.length);

    int remain = dqt.length - sizeof(dqt.length);
    if (remain >= 1) {
        uint8_t value = 0;
        m_FileReader.ReadUInt8(value);
        dqt.precision = static_cast<DQT::Precision>(value >> 4);
        dqt.tableID = static_cast<QuantizationTableID>(value & 0x0F);
        remain--;
    }

    switch (dqt.precision) {
    case DQT::Precision::BITS_8:
        if (int size = sizeof(std::get<DQT::Bits8Table>(dqt.table)); remain >= size) {
            m_FileReader.ReadBytes(std::get<DQT::Bits8Table>(dqt.table));
            remain -= size;
        }
        m_Segments.emplace_back(std::make_shared<DQT>(dqt));

        // Luminance と Chrominance の情報が 1つの DQT にまとめられている場合を考慮
        if (remain >= 1) {
            uint8_t value = 0;
            m_FileReader.ReadUInt8(value);
            dqt.precision = static_cast<DQT::Precision>(value >> 4);
            dqt.tableID = static_cast<QuantizationTableID>(value & 0x0F);
            remain--;
        }

        if (int size = sizeof(std::get<DQT::Bits8Table>(dqt.table)); remain >= size) {
            m_FileReader.ReadBytes(std::get<DQT::Bits8Table>(dqt.table));
            remain -= size;
        }
        m_Segments.emplace_back(std::make_shared<DQT>(dqt));
        break;
    case DQT::Precision::BITS_16:
        if (int size = sizeof(std::get<DQT::Bits16Table>(dqt.table)); remain >= size) {
            m_FileReader.ReadBytes(std::get<DQT::Bits16Table>(dqt.table));
            remain -= size;
        }
        m_Segments.emplace_back(std::make_shared<DQT>(dqt));
        break;
    default:
        break;
    }
}

void JpegParser::parseSOF0()
{
    auto sof0 = SOF0{};
    sof0.marker = Marker::SOF0;
    m_FileReader.ReadUInt16(sof0.length);

    int remain = sof0.length - sizeof(sof0.length);
    if (remain >= 1) {
        m_FileReader.ReadUInt8(sof0.precision);
        remain--;
    }
    if (remain >= sizeof(sof0.height) + sizeof(sof0.width)) {
        m_FileReader.ReadUInt16(sof0.height);
        m_FileReader.ReadUInt16(sof0.width);
        remain -= sizeof(sof0.height) + sizeof(sof0.width);
    }
    if (remain >= 1) {
        m_FileReader.ReadUInt8(sof0.numComponents);
        remain--;
    }
    sof0.components.resize(sof0.numComponents);
    if (remain >= sizeof(sof0.components[0]) * sof0.numComponents) {
        m_FileReader.ReadBytes(sof0.components);
        remain -= sizeof(sof0.components[0]) * sof0.numComponents;
    }

    m_Segments.emplace_back(std::make_shared<SOF0>(sof0));
}

void JpegParser::parseDHT()
{
    auto dht = DHT{};
    dht.marker = Marker::DHT;
    m_FileReader.ReadUInt16(dht.length);

    int remain = dht.length - sizeof(dht.length);
    while (remain > 0) {
        if (remain >= 1) {
            uint8_t value = 0;
            m_FileReader.ReadUInt8(value);
            dht.tableClass = static_cast<DHT::TableClass>(value >> 4);
            dht.tableID = static_cast<HuffmanTableID>(value & 0xFF);
            remain--;
        }
        if (remain >= sizeof(dht.counts)) {
            m_FileReader.ReadBytes(dht.counts);
            remain -= sizeof(dht.counts);
        }
        int total = 0;
        for (auto count : dht.counts) {
            total += count;
        }
        if (remain >= total) {
            dht.symbols.resize(total);
            m_FileReader.ReadBytes(dht.symbols);
            remain -= total;
        }
        m_Segments.emplace_back(std::make_shared<DHT>(dht));
    }
}

void JpegParser::parseSOS()
{
    auto sos = SOS{};
    sos.marker = Marker::SOS;
    m_FileReader.ReadUInt16(sos.length);
    int remain = sos.length - sizeof(sos.length);
    if (remain >= 1) {
        m_FileReader.ReadUInt8(sos.numComponents);
        remain--;
    }
    sos.components.resize(sos.numComponents);
    if (remain >= sizeof(sos.components[0]) * sos.numComponents) {
        m_FileReader.ReadBytes(sos.components);
        remain -= sizeof(sos.components[0]) * sos.numComponents;
    }
    if (remain >= 3) {
        m_FileReader.ReadUInt8(sos.spectralSelectionStart);
        m_FileReader.ReadUInt8(sos.spectralSelectionEnd);
        m_FileReader.ReadUInt8(sos.successiveApproximation);
        remain -= 3;
    }

    m_Segments.emplace_back(std::make_shared<SOS>(sos));

    parseECS();
}

void JpegParser::parseECS()
{
    auto current_pos = m_FileReader.GetCurrentPosition();
    m_FileReader.Seek(-2, BinaryFileReader::SeekOrigin::End);
    auto end_pos = m_FileReader.GetCurrentPosition();

    Marker marker{};
    m_FileReader.ReadUInt16(marker);

    if (marker != Marker::EOI) {
        throw std::runtime_error("Invalid JPEG file");
    }

    m_FileReader.Seek(current_pos);

    m_ECS.resize(end_pos - current_pos);
    m_FileReader.ReadBytes(m_ECS);
}

void JpegParser::parseEOI()
{
    auto eoi = EOI{};
    eoi.marker = Marker::EOI;
    m_Segments.emplace_back(std::make_shared<EOI>(eoi));
}

void JpegParser::parseDRI()
{
    auto dri = DRI{};
    dri.marker = Marker::DRI;
    m_FileReader.ReadUInt16(dri.length);
    int remain = dri.length - sizeof(dri.length);
    if (remain >= sizeof(dri.restartInterval)) {
        m_FileReader.ReadUInt16(dri.restartInterval);
        remain -= sizeof(dri.restartInterval);
    }
    m_Segments.emplace_back(std::make_shared<DRI>(dri));
}

void JpegParser::parseAPP1()
{
    auto app1 = APP1{};
    app1.marker = Marker::APP1;
    m_FileReader.ReadUInt16(app1.length);
    m_Segments.emplace_back(std::make_shared<APP1>(app1));

    int remain = app1.length - sizeof(app1.length);
    m_FileReader.Seek(remain, BinaryFileReader::SeekOrigin::Current); // 捨てる
}

void JpegParser::parseAPP2()
{
    auto app2 = APP2{};
    app2.marker = Marker::APP2;
    m_FileReader.ReadUInt16(app2.length);
    m_Segments.emplace_back(std::make_shared<APP2>(app2));
 
    int remain = app2.length - sizeof(app2.length);
    m_FileReader.Seek(remain, BinaryFileReader::SeekOrigin::Current); // 捨てる
}

void JpegParser::parseAPP13()
{
    auto app13 = APP13{};
    app13.marker = Marker::APP13;
    m_FileReader.ReadUInt16(app13.length);
    m_Segments.emplace_back(std::make_shared<APP13>(app13));

    int remain = app13.length - sizeof(app13.length);
    m_FileReader.Seek(remain, BinaryFileReader::SeekOrigin::Current); // 捨てる
}

void JpegParser::parseAPP14()
{
    auto app14 = APP14{};
    app14.marker = Marker::APP14;
    m_FileReader.ReadUInt16(app14.length);
    m_Segments.emplace_back(std::make_shared<APP14>(app14));

    int remain = app14.length - sizeof(app14.length);
    m_FileReader.Seek(remain, BinaryFileReader::SeekOrigin::Current); // 捨てる
}

void JpegParser::parseCOM()
{
    auto com = COM{};
    com.marker = Marker::COM;
    m_FileReader.ReadUInt16(com.length);
    int remain = com.length - sizeof(com.length);
    com.comment.resize(remain);
    m_FileReader.ReadBytes(com.comment);
    m_Segments.emplace_back(std::make_shared<COM>(com));
}
