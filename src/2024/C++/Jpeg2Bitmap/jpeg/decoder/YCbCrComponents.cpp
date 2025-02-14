#pragma once
#include "YCbCrComponents.h"

using namespace jpeg::segments;

namespace jpeg
{
    YCbCrComponents::YCbCrComponents(const SOF0& sof0)
    {
        sampleWidth = sof0.width;
        sampleHeight = sof0.height;

        auto componentSelector = [&](ComponentID id) -> ComponentInfo& {
            switch (id) {
            case ComponentID::Y:
                return y;
            case ComponentID::Cb:
                return cb;
            case ComponentID::Cr:
                return cr;
            default:
                assert(false);
                return y; // dummy
            }
        };

        for (const auto& component : sof0.components) {
            assert(component.id == ComponentID::Y || component.id == ComponentID::Cb || component.id == ComponentID::Cr);
            auto& info = componentSelector(component.id);
            info.horizontalSamplingFactor = component.horizonalSamplingFactor;
            info.verticalSamplingFactor = component.verticalSamplingFactor;
        }

        for (const auto& component : sof0.components) {
            auto& info = componentSelector(component.id);
            const auto hMaxFactor = getMaxHorizontalSamplingFactor();
            const auto vMaxFactor = getMaxVerticalSamplingFactor();
            info.width = (sof0.width * info.horizontalSamplingFactor + hMaxFactor - 1) / hMaxFactor;
            info.height = (sof0.height * info.verticalSamplingFactor + vMaxFactor - 1) / vMaxFactor;
            info.buffer.resize(info.width * info.height, 0);
        }
    }
} // namespace jpeg
