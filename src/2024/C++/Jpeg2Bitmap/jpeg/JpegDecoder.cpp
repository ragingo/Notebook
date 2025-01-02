#include <cassert>
#include <cstdint>
#include <format>
#include <iostream>
#include <print>
#include <vector>

#include "jpeg.h"
#include "JpegDecoder.h"
#include "debugging.h"

using namespace jpeg;
using namespace jpeg::segments;

namespace
{
    // Figure C.1 – Generation of table of Huffman code sizes
    // counts (BITS) には、各ビット長(1～16)のハフマン符号の数が格納されている。
    // 例えば、 counts[] = { 0, 5, 1, 1 } の場合、
    // ハフマン符号のビット長は 1ビットが5つ、2ビットが1つ、3ビットが1つとなる。
    // input: counts[0] = 0, counts[1] = 5, counts[2] = 1, counts[3] = 1
    // output: { 1, 1, 1, 1, 1, 2, 3 }
    std::vector<int> createHuffSize(const std::array<uint8_t, 16>& counts)
    {
        std::vector<int> huffSize;
        for (int bits = 1; bits <= counts.size(); ++bits) {
            for (int count = 0; count < counts[bits - 1]; ++count) {
                huffSize.push_back(bits);
            }
        }
        return huffSize;
    }
}

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

    debugging::dumpSummary(m_Markers, m_Segments);

    auto sos = findFirstSegment<SOS>(m_Segments);
    assert(sos);
    auto dhts = findSegments<DHT>(m_Segments);
    assert(!dhts.empty());

    for (const auto& dht : dhts) {
        auto bits = dht->counts;
        auto symbols = dht->symbols;
        auto huffSize = createHuffSize(bits);
    }
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
    auto soi = SOI{};
    soi.marker = Marker::SOI;
    m_Segments.emplace_back(std::make_shared<SOI>(soi));
}

void JpegDecoder::parseAPP0()
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

void JpegDecoder::parseDQT()
{
    auto dqt = DQT{};
    dqt.marker = Marker::DQT;
    m_FileReader.ReadUInt16(dqt.length);

    int remain = dqt.length - sizeof(dqt.length);
    if (remain >= 1) {
        uint8_t value;
        m_FileReader.ReadUInt8(value);
        dqt.precision = static_cast<DQT::Precision>(value >> 4);
        dqt.tableID = static_cast<QuantizationTableID>(value & 0x0F);
        remain--;
    }

    switch (dqt.precision) {
    case DQT::Precision::BITS_8:
        if (int size = sizeof(std::get<DQT::Bits8Table>(dqt.table)); remain >= size)
        {
            m_FileReader.ReadBytes(std::get<DQT::Bits8Table>(dqt.table));
            remain -= size;
        }

        break;
    case DQT::Precision::BITS_16:
        if (int size = sizeof(std::get<DQT::Bits16Table>(dqt.table)); remain >= size) {
            m_FileReader.ReadBytes(std::get<DQT::Bits16Table>(dqt.table));
            remain -= size;
        }
        break;
    default:
        break;
    }

    m_Segments.emplace_back(std::make_shared<DQT>(dqt));
}

void JpegDecoder::parseSOF0()
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

void JpegDecoder::parseDHT()
{
    auto dht = DHT{};
    dht.marker = Marker::DHT;
    m_FileReader.ReadUInt16(dht.length);

    int remain = dht.length - sizeof(dht.length);
    if (remain >= 1) {
        uint8_t value;
        m_FileReader.ReadUInt8(value);
        dht.tableClass = static_cast<DHT::TableClass>(value >> 4);
        dht.tableID = static_cast<HuffmanTableID>(value & 0xFF);
        remain--;
    }
    if (remain >= sizeof(dht.counts)) {
        m_FileReader.ReadBytes(dht.counts);
        remain -= sizeof(dht.counts);
    }
    if (remain > 0) {
        dht.symbols.resize(remain);
        m_FileReader.ReadBytes(dht.symbols);
        remain = 0;
    }

    m_Segments.emplace_back(std::make_shared<DHT>(dht));
}

void JpegDecoder::parseSOS()
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
}

void JpegDecoder::parseEOI()
{
    auto eoi = EOI{};
    eoi.marker = Marker::EOI;
    m_Segments.emplace_back(std::make_shared<EOI>(eoi));
}
