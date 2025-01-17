﻿#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "segments.h"

namespace jpeg { namespace debugging {

    std::string to_string(const segments::SOI& soi);
    std::string to_string(const segments::APP0& app0);
    std::string to_string(const segments::DQT& dqt);
    std::string to_string(const segments::SOF0& sof0);
    std::string to_string(const segments::EOI& eoi);
    std::string to_string(const segments::DHT& dht);
    std::string to_string(const segments::SOS& sos);
    std::string to_string(const std::vector<uint8_t>& ecs);
    void dumpSummary(std::vector<Marker> markers, std::vector<std::shared_ptr<segments::Segment>> segments);

} } // namespace jpg::debugging
