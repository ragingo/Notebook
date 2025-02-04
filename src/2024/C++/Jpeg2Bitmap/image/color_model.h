#pragma once

#include <algorithm>
#include <cstdint>
#include <tuple>

inline constexpr std::tuple<uint8_t, uint8_t, uint8_t> convertYCbCrToRGB(int y, int cb, int cr)
{
    cb -= 128;
    cr -= 128;
    double r = y + 1.402 * cr;
    double g = y - 0.344136 * cb - 0.714136 * cr;
    double b = y + 1.772 * cb;
    r = std::clamp(r, 0.0, 255.0);
    g = std::clamp(g, 0.0, 255.0);
    b = std::clamp(b, 0.0, 255.0);
    return { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b) };
}
