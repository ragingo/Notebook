﻿#include "debugging.h"
#include <algorithm>
#include <format>
#include <iostream>
#include <variant>
#include <ranges>
#include <nameof.hpp>
#include "jpeg/decoder/Utility.h"

using namespace jpeg::segments;

namespace jpeg::debugging {

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
        result += std::format("    horizonalSamplingFactor: 0x{:02X}\n", sof0.components[i].horizonalSamplingFactor);
        result += std::format("    verticalSamplingFactor: 0x{:02X}\n", sof0.components[i].verticalSamplingFactor);
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

std::string to_string(const APP1& app1)
{
    std::string result;
    result += "APP1\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(app1.marker));
    result += std::format("  length: 0x{:04X}\n", app1.length);
    return result;
}

std::string to_string(const APP13& app13)
{
    std::string result;
    result += "APP13\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(app13.marker));
    result += std::format("  length: 0x{:04X}\n", app13.length);
    return result;
}

std::string to_string(const APP14& app14)
{
    std::string result;
    result += "APP14\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(app14.marker));
    result += std::format("  length: 0x{:04X}\n", app14.length);
    return result;
}

std::string to_string(const COM& com)
{
    std::string result;
    result += "COM\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(com.marker));
    result += std::format("  length: 0x{:04X}\n", com.length);
    result += std::format("  comment: {}\n", com.getComment());
    result += "\n";
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
        case Marker::APP1:
            std::cout << to_string(*segment_cast<APP1>(segment)) << std::endl;
            break;
        case Marker::APP13:
            std::cout << to_string(*segment_cast<APP13>(segment)) << std::endl;
            break;
        case Marker::APP14:
            std::cout << to_string(*segment_cast<APP14>(segment)) << std::endl;
            break;
        case Marker::COM:
            std::cout << to_string(*segment_cast<COM>(segment)) << std::endl;
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
        result += std::format("ColorSpace: {}", NAMEOF_ENUM(getColorSpace(sof0)));
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

void dumpMCU(std::vector<int> mcu)
{
    std::string result;
    for (int i = 0; i < mcu.size(); ++i) {
        result += std::format("  {:3}", mcu[i]);
        if (i % 8 == 7) {
            result += '\n';
        }
    }
    result += "\n";
    std::cout << result;
}

void dumpBlock(std::string_view label, std::array<int, 64> block)
{
    std::mdspan<int, std::extents<int, 8, 8>> blockSpan(block.data());
    std::string result;
    result += std::format("{}\n", label);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            result += std::format("  {:3}", blockSpan[i, j]);
        }
        result += '\n';
    }
    result += "\n";
    std::cout << result;
}

} // namespace jpg::debugging
