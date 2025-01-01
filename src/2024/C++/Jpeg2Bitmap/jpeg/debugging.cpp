#include <format>
#include <variant>
#include <nameof.hpp>
#include "debugging.h"

namespace jpeg { namespace debugging {

std::string to_string(const segments::SOI& soi)
{
    std::string result;
    result += "SOI\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(soi.marker));
    return result;
}

std::string to_string(const segments::APP0& app0)
{
    std::string result;
    result += "APP0\n";
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(app0.marker));
    result += std::format("  length: 0x{:04X}\n", app0.length);
    result += std::format("  identifier: {}\n", static_cast<const char*>(app0.identifier.data()));
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
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(dqt.marker));
    result += std::format("  length: 0x{:04X}\n", dqt.length);
    result += std::format("  precision: 0x{:01X} ({})\n", static_cast<uint8_t>(dqt.precision), NAMEOF_ENUM(dqt.precision));
    result += std::format("  tableID: 0x{:01X}\n", dqt.tableID);
    result += "  table:\n";
    for (int i = 0; i < 64; ++i) {
        switch (dqt.precision) {
        case segments::DQT::Precision::BITS_8:
            {
                auto table = std::get<segments::DQT::Bits8Table>(dqt.table);
                result += std::format("    0x{:02X}", table[i]);
            }
            break;
        case segments::DQT::Precision::BITS_16:
            {
                auto table = std::get<segments::DQT::Bits16Table>(dqt.table);
                result += std::format("    0x{:04X}", table[i]);
            }
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
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(sof0.marker));
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
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(dht.marker));
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
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(sos.marker));
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
        result += std::format(" {:02X}", ecs[i]);
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
    result += std::format("  marker: 0x{:04X}\n", std::to_underlying(eoi.marker));
    return result;
}

} } // namespace jpg::debugging
