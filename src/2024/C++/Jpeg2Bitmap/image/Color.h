#pragma once

#include <algorithm>
#include <cstdint>
#include <tuple>

namespace image
{
    constexpr std::tuple<uint8_t, uint8_t, uint8_t> ycbcrToRGB(int y, int cb, int cr)
    {
        constexpr int coef_r = static_cast<int>(1.402 * 65536);
        constexpr int coef_g_1 = static_cast<int>(0.344136 * 65536);
        constexpr int coef_g_2 = static_cast<int>(0.714136 * 65536);
        constexpr int coef_b = static_cast<int>(1.772 * 65536);

        int cb2 = cb - 128;
        int cr2 = cr - 128;
        int r = y + ((coef_r * cr2) >> 16);
        int g = y - ((coef_g_1 * cb2 + coef_g_2 * cr2) >> 16);
        int b = y + ((coef_b * cb2) >> 16);

        return {
            static_cast<uint8_t>(std::clamp(r, 0, 255)),
            static_cast<uint8_t>(std::clamp(g, 0, 255)),
            static_cast<uint8_t>(std::clamp(b, 0, 255))
        };
    }
    static_assert(ycbcrToRGB(255, 128, 128) == std::make_tuple(255, 255, 255));
    static_assert(ycbcrToRGB(1, 2, 3) == std::make_tuple(0, 134, 0));
} // namespace image