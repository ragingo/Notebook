#pragma once
#include <vector>
#include "jpeg/syntax/segments.h"

namespace jpeg
{
    struct ComponentInfo
    {
        segments::ComponentID id;
        int horizontalSamplingFactor;
        int verticalSamplingFactor;
        int width;
        int height;
        std::vector<int> buffer;
    };
}
