#pragma once
#include <cassert>
#include <cstdint>
#include "jpeg/syntax/segments.h"
#include "ComponentInfo.h"

namespace jpeg
{
    class YCbCrComponents final
    {
    public:
        YCbCrComponents(const segments::SOF0& sof0);

        inline int getMaxHorizontalSamplingFactor() const
        {
            return std::max({ y.horizontalSamplingFactor, cb.horizontalSamplingFactor, cr.horizontalSamplingFactor });
        }

        inline int getMaxVerticalSamplingFactor() const
        {
            return std::max({ y.verticalSamplingFactor, cb.verticalSamplingFactor, cr.verticalSamplingFactor });
        }

        inline int getMCUWidth() const
        {
            return getMaxHorizontalSamplingFactor() * 8;
        }

        inline int getMCUHeight() const
        {
            return getMaxVerticalSamplingFactor() * 8;
        }

        inline int getMCUHorizontalCount() const
        {
            return (sampleWidth + getMCUWidth() - 1) / getMCUWidth();
        }

        inline int getMCUVerticalCount() const
        {
            return (sampleHeight + getMCUHeight() - 1) / getMCUHeight();
        }

        inline double getCbHorizontalSamplingFactor() const
        {
            return static_cast<double>(cb.horizontalSamplingFactor) / static_cast<double>(y.horizontalSamplingFactor);
        }

        inline double getCbVerticalSamplingFactor() const
        {
            return static_cast<double>(cb.verticalSamplingFactor) / static_cast<double>(y.verticalSamplingFactor);
        }

        inline double getCrHorizontalSamplingFactor() const
        {
            return static_cast<double>(cr.horizontalSamplingFactor) / static_cast<double>(y.horizontalSamplingFactor);
        }

        inline double getCrVerticalSamplingFactor() const
        {
            return static_cast<double>(cr.verticalSamplingFactor) / static_cast<double>(y.verticalSamplingFactor);
        }

        inline const ComponentInfo& getComponent(segments::ComponentID id) const
        {
            switch (id) {
            case segments::ComponentID::Y:
                return y;
            case segments::ComponentID::Cb:
                return cb;
            case segments::ComponentID::Cr:
                return cr;
            default:
                assert(false);
                return y; // dummy
            }
        }

        inline ComponentInfo& getComponent(segments::ComponentID id)
        {
            switch (id) {
            case segments::ComponentID::Y:
                return y;
            case segments::ComponentID::Cb:
                return cb;
            case segments::ComponentID::Cr:
                return cr;
            default:
                assert(false);
                return y; // dummy
            }
        }
    private:
        uint16_t sampleWidth = 0;
        uint16_t sampleHeight = 0;
        ComponentInfo y{};
        ComponentInfo cb{};
        ComponentInfo cr{};
    };
}
