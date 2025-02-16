#pragma once
#include <array>
#include <cstdint>

namespace jpeg
{
    inline constexpr int BLOCK_WIDTH = 8;
    inline constexpr int BLOCK_HEIGHT = 8;
    inline constexpr int BLOCK_SIZE = BLOCK_WIDTH * BLOCK_HEIGHT;

    using MCUBlock8x8 = std::array<int16_t, BLOCK_SIZE>;

    // Figure A.6 – Zig-zag sequence of quantized DCT coefficients
    alignas(32) inline constexpr std::array<int32_t, BLOCK_SIZE> ZIGZAG = {
         0,  1,  5,  6, 14, 15, 27, 28,
         2,  4,  7, 13, 16, 26, 29, 42,
         3,  8, 12, 17, 25, 30, 41, 43,
         9, 11, 18, 24, 31, 40, 44, 53,
        10, 19, 23, 32, 39, 45, 52, 54,
        20, 22, 33, 38, 46, 51, 55, 60,
        21, 34, 37, 47, 50, 56, 59, 61,
        35, 36, 48, 49, 57, 58, 62, 63
    };

} // namespace jpeg
