#pragma once
#include <cstdint>
#include <memory>
#include <mdspan>
#include <string>
#include <vector>
#include "../syntax/segments.h"

namespace jpeg::debugging {

    std::string to_string(const segments::SOI& soi);
    std::string to_string(const segments::APP0& app0);
    std::string to_string(const segments::DQT& dqt);
    std::string to_string(const segments::SOF0& sof0);
    std::string to_string(const segments::EOI& eoi);
    std::string to_string(const segments::DHT& dht);
    std::string to_string(const segments::SOS& sos);
    std::string to_string(const std::vector<uint8_t>& ecs);
    std::string to_string(const segments::APP1& app1);
    std::string to_string(const segments::APP13& app13);
    std::string to_string(const segments::APP14& app14);
    void dumpSummary(std::vector<Marker> markers, std::vector<std::shared_ptr<segments::Segment>> segments);
    void dumpMCU(std::vector<int> mcu);
    void dumpBlock(std::string_view label, std::array<int, 64> block);

} // namespace jpg::debugging
