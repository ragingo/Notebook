#include <algorithm>
#include <cassert>
#include <cstdint>
#include <format>
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
    auto soi = segments::SOI{};
    soi.marker = Marker::SOI;
    m_Segments.emplace_back(std::make_shared<segments::SOI>(soi));

    std::cout << jpeg::debugging::to_string(soi) << std::endl;
}

void JpegDecoder::parseAPP0()
{
    auto app0 = segments::APP0{};
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
    m_Segments.emplace_back(std::make_shared<segments::APP0>(app0));

    std::cout << jpeg::debugging::to_string(app0) << std::endl;
}

void JpegDecoder::parseDQT()
{
    auto dqt = segments::DQT{};
    dqt.marker = Marker::DQT;
    m_FileReader.ReadUInt16(dqt.length);
    int remain = dqt.length - sizeof(dqt.length);
    if (remain >= 1) {
        uint8_t value;
        m_FileReader.ReadUInt8(value);
        dqt.precision = static_cast<segments::DQT::Precision>(value >> 4);
        dqt.tableID = value & 0x0F;
        remain--;
    }

    switch (dqt.precision) {
    case segments::DQT::Precision::BITS_8:
        if (int size = sizeof(std::get<segments::DQT::Bits8Table>(dqt.table)); remain >= size)
        {
            m_FileReader.ReadBytes(std::get<segments::DQT::Bits8Table>(dqt.table));
            remain -= size;
        }

        break;
    case segments::DQT::Precision::BITS_16:
        if (int size = sizeof(std::get<segments::DQT::Bits16Table>(dqt.table)); remain >= size) {
            m_FileReader.ReadBytes(std::get<segments::DQT::Bits16Table>(dqt.table));
            remain -= size;
        }
        break;
    default:
        break;
    }

    m_Segments.emplace_back(std::make_shared<segments::DQT>(dqt));

    std::cout << jpeg::debugging::to_string(dqt) << std::endl;
}

void JpegDecoder::parseSOF0()
{
    auto sof0 = segments::SOF0{};
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

    m_Segments.emplace_back(std::make_shared<segments::SOF0>(sof0));

    std::cout << jpeg::debugging::to_string(sof0) << std::endl;
}

void JpegDecoder::parseDHT()
{
    auto dht = segments::DHT{};
    dht.marker = Marker::DHT;
    m_FileReader.ReadUInt16(dht.length);
    int remain = dht.length - sizeof(dht.length);
    if (remain >= 1) {
        uint8_t value;
        m_FileReader.ReadUInt8(value);
        dht.tableClass = static_cast<segments::DHT::TableClass>(value >> 4);
        dht.tableID = value & 0xFF;
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

    m_Segments.emplace_back(std::make_shared<segments::DHT>(dht));

    std::cout << jpeg::debugging::to_string(dht) << std::endl;
}

void JpegDecoder::parseSOS()
{
    auto sos = segments::SOS{};
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

    m_Segments.emplace_back(std::make_shared<segments::SOS>(sos));

    std::cout << jpeg::debugging::to_string(sos) << std::endl;

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
    auto eoi = segments::EOI{};
    eoi.marker = Marker::EOI;

    m_Segments.emplace_back(std::make_shared<segments::EOI>(eoi));

    std::cout << jpeg::debugging::to_string(eoi) << std::endl;
}

void JpegDecoder::dumpSummary()
{
    std::string result;

    result += "==================================================\n";
    result += std::format("File Size: {} bytes\n", m_FileReader.GetSize());

    if (std::ranges::contains(m_Markers, Marker::SOF0)) {
        auto sof0s = findSegments<segments::SOF0>();
        assert(sof0s.size() >= 1);
        auto sof0 = *(sof0s[0]);

        result += "Frame Type: Baseline\n";
        result += std::format("Resolution: {}x{}\n", sof0.width, sof0.height);

        result += "Components: ";
        switch (sof0.numComponents) {
        case 1:
            result += "Grayscale";
            break;
        case 3:
            {
                auto ids = sof0.components
                    | std::ranges::views::transform([](const auto& c) { return c.id; })
                    | std::ranges::to<std::vector>();
                using ID = segments::SOF0::Component::ID;
                if (ids == std::vector{ ID::Y, ID::Cb, ID::Cr }) {
                    result += "YCbCr";
                }
                else if (ids == std::vector{ ID::Y, ID::I, ID::Q }) {
                    result += "YIQ";
                }
                else {
                    result += "Unknown";
                }
            }
            break;
        case 4:
            result += "CMYK";
            break;
        default:
            result += "Unknown";
            break;
        }
        result += "\n";
    }

    // B.2.4.2 Huffman table-specification syntax
    if (std::ranges::contains(m_Markers, Marker::DHT)) {
        result += "Huffman Tables: \n";

        namespace r = std::ranges;
        namespace rv = std::ranges::views;

        auto dhts = findSegments<segments::DHT>();

        for (const auto& dht : dhts) {
            int value = 2;
            for (int i = 0; i < 1; i++) {
                auto m1 = rv::iota(0, 16)
                    | rv::transform([&](int i) { return dht->counts[i]; })
                    | r::to<std::vector>();
                auto m2 = r::fold_left(m1, 0, std::plus<>());
                value += (17 + m2);
            }

            result += std::format("  Lh: 0x{0:02X} ({0})\n", value);
        }
        
        result += "\n";
    }

    result += "==================================================";

    std::cout << result << std::endl;
}
