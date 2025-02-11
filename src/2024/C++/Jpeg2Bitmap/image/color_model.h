#pragma once

#include <algorithm>
#include <cstdint>
#include <tuple>

constexpr std::tuple<uint8_t, uint8_t, uint8_t> convertYCbCrToRGB(int y, int cb, int cr)
{
    constexpr int coeff_r = static_cast<int>(1.402 * 65536);
    constexpr int coeff_g_1 = static_cast<int>(0.344136 * 65536);
    constexpr int coeff_g_2 = static_cast<int>(0.714136 * 65536);
    constexpr int coeff_b = static_cast<int>(1.772 * 65536);

    cb -= 128;
    cr -= 128;
    int r = y + ((coeff_r * cr) >> 16);
    int g = y - ((coeff_g_1 * cb + coeff_g_2 * cr) >> 16);
    int b = y + ((coeff_b * cb) >> 16);
    r = std::clamp(r, 0, 255);
    g = std::clamp(g, 0, 255);
    b = std::clamp(b, 0, 255);
    return { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b) };
}
