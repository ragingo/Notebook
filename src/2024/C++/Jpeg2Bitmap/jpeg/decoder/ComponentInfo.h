#pragma once
#include <cstdint>
#include <vector>
#include "jpeg/syntax/Segment.h"

namespace jpeg
{
    struct ComponentInfo
    {
        segments::ComponentID id;
        int horizontalSamplingFactor;
        int verticalSamplingFactor;
        int width;
        int height;
        std::vector<int16_t> buffer;
    };
}
