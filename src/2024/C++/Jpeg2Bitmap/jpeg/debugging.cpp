#include <algorithm>
#include <format>
#include <iostream>
#include <variant>
#include <ranges>
#include <nameof.hpp>
#include "debugging.h"

using namespace jpeg::segments;

namespace jpeg { namespace debugging {

std::string to_string(const SOI& soi)
{
    std::string result;
    result += "SOI\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(soi.marker));
    return result;
}

std::string to_string(const APP0& app0)
{
    std::string result;
    result += "APP0\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(app0.marker));
    result += std::format("  length: 0x{:04X}\n", app0.length);
    result += std::format("  identifier: {}\n", static_cast<const char*>(app0.identifier.data()));
    result += std::format("  version: 0x{:02X} ({})\n", std::to_underlying(app0.version), NAMEOF_ENUM(app0.version));
    result += std::format("  units: 0x{:02X} ({})\n", std::to_underlying(app0.units), NAMEOF_ENUM(app0.units));
    result += std::format("  xDensity: 0x{:04X}\n", app0.xDensity);
    result += std::format("  yDensity: 0x{:04X}\n", app0.yDensity);
    result += std::format("  thumbnailWidth: 0x{:02X}\n", app0.thumbnailWidth);
    result += std::format("  thumbnailHeight: 0x{:02X}\n", app0.thumbnailHeight);
    return result;
}

std::string to_string(const DQT& dqt)
{
    std::string result;
    result += "DQT\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(dqt.marker));
    result += std::format("  length: 0x{:04X}\n", dqt.length);
    result += std::format("  precision: 0x{:01X} ({})\n", std::to_underlying(dqt.precision), NAMEOF_ENUM(dqt.precision));
    result += std::format("  tableID: 0x{:01X} ({})\n", std::to_underlying(dqt.tableID), NAMEOF_ENUM(dqt.tableID));
    result += "  table:\n";
    for (int i = 0; i < 64; ++i) {
        switch (dqt.precision) {
        case DQT::Precision::BITS_8:
            {
                auto table = std::get<DQT::Bits8Table>(dqt.table);
                result += std::format("    0x{0:02X} ({0:3})", table[i]);
            }
            break;
        case DQT::Precision::BITS_16:
            {
                auto table = std::get<DQT::Bits16Table>(dqt.table);
                result += std::format("    0x{0:04X} ({0:3})", table[i]);
            }
            break;
        }
        if (i % 8 == 7) {
            result += '\n';
        }
    }
    return result;
}

std::string to_string(const SOF0& sof0)
{
    std::string result;
    result += "SOF0\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(sof0.marker));
    result += std::format("  length: 0x{:04X}\n", sof0.length);
    result += std::format("  precision: 0x{:01X}\n", sof0.precision);
    result += std::format("  height: 0x{:04X}\n", sof0.height);
    result += std::format("  width: 0x{:04X}\n", sof0.width);
    result += std::format("  numComponents: 0x{:02X}\n", sof0.numComponents);
    for (int i = 0; i < sof0.numComponents; ++i) {
        result += std::format("  Component[{}]:\n", i);
        result += std::format("    id: 0x{:02X} ({})\n", std::to_underlying(sof0.components[i].id), NAMEOF_ENUM(sof0.components[i].id));
        result += std::format("    samplingFactorHorizontalRatio: 0x{:02X}\n", sof0.components[i].samplingFactorHorizontalRatio);
        result += std::format("    samplingFactorVerticalRatio: 0x{:02X}\n", sof0.components[i].samplingFactorVerticalRatio);
        result += std::format("    tableID: 0x{:02X} ({})\n", std::to_underlying(sof0.components[i].tableID), NAMEOF_ENUM(sof0.components[i].tableID));
    }
    return result;
}

std::string to_string(const DHT& dht)
{
    std::string result;
    result += "DHT\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(dht.marker));
    result += std::format("  length: 0x{:04X}\n", dht.length);
    result += std::format("  tableClass: 0x{:02X} ({})\n", std::to_underlying(dht.tableClass), NAMEOF_ENUM(dht.tableClass));
    result += std::format("  tableID: 0x{:02X} ({})\n", std::to_underlying(dht.tableID), NAMEOF_ENUM(dht.tableID));
    result += "  counts:\n";
    for (int i = 0; i < 16; ++i) {
        result += std::format("    0x{0:02X} ({0:3})", dht.counts[i]);
        if (i % 8 == 7) {
            result += '\n';
        }
    }
    result += "  symbols:\n";
    for (int i = 0; i < dht.symbols.size(); ++i) {
        result += std::format("    0x{0:02X} ({0:3})", dht.symbols[i]);
        if (i % 8 == 7) {
            result += '\n';
        }
    }
    return result;
}

std::string to_string(const SOS& sos)
{
    std::string result;
    result += "SOS\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(sos.marker));
    result += std::format("  length: 0x{:04X}\n", sos.length);
    result += std::format("  numComponents: 0x{:02X}\n", sos.numComponents);
    for (int i = 0; i < sos.numComponents; ++i) {
        auto component = sos.components[i];
        result += std::format("  Component[{}]:\n", i);
        result += std::format("    selector: 0x{:02X} ({})\n", std::to_underlying(component.componentSelector), NAMEOF_ENUM(component.componentSelector));
        result += std::format("    dcSelector: 0x{:02X} ({})\n", std::to_underlying(component.dcSelector), NAMEOF_ENUM(component.dcSelector));
        result += std::format("    acSelector: 0x{:02X} ({})\n", std::to_underlying(component.acSelector), NAMEOF_ENUM(component.acSelector));
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
        result += std::format(" {:02X}", ecs[i]);
        if (i % 32 == 31) {
            result += '\n';
        }
    }
    result += "\n";
    return result;
}

std::string to_string(const EOI& eoi)
{
    std::string result;
    result += "EOI\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(eoi.marker));
    return result;
}

void dumpSummary(
    std::vector<Marker> markers,
    std::vector<std::shared_ptr<Segment>> segments
)
{
    for (const auto segment : segments) {
        switch (segment->marker) {
        case Marker::SOI:
            std::cout << to_string(*segment_cast<SOI>(segment)) << std::endl;
            break;
        case Marker::APP0:
            std::cout << to_string(*segment_cast<APP0>(segment)) << std::endl;
            break;
        case Marker::DQT:
            std::cout << to_string(*segment_cast<DQT>(segment)) << std::endl;
            break;
        case Marker::SOF0:
            std::cout << to_string(*segment_cast<SOF0>(segment)) << std::endl;
            break;
        case Marker::DHT:
            std::cout << to_string(*segment_cast<DHT>(segment)) << std::endl;
            break;
        case Marker::SOS:
            std::cout << to_string(*segment_cast<SOS>(segment)) << std::endl;
            break;
        case Marker::EOI:
            std::cout << to_string(*segment_cast<EOI>(segment)) << std::endl;
            break;
        default:
            std::cout << "Unknown marker: 0x" << std::format("{:02X}", std::to_underlying(segment->marker)) << std::endl;
            break;
        }
    }

    std::string result;

    result += "==================================================\n";
    
    if (std::ranges::contains(markers, Marker::SOF0)) {
        auto sof0s = findSegments<SOF0>(segments);
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
            using enum ComponentID;
            if (ids == std::vector{ Y, Cb, Cr }) {
                result += "YCbCr";
            }
            else if (ids == std::vector{ Y, I, Q }) {
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
    if (std::ranges::contains(markers, Marker::DHT)) {
        result += "Huffman Tables: \n";

        namespace r = std::ranges;
        namespace rv = std::ranges::views;

        auto dhts = findSegments<DHT>(segments);

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

} } // namespace jpg::debugging
