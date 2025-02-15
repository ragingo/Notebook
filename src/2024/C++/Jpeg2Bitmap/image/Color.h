#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <tuple>
#include <vector>

namespace image
{
    constexpr std::tuple<uint8_t, uint8_t, uint8_t> ycbcrToRGB(int y, int cb, int cr) noexcept
    {
        constexpr int coef_r = static_cast<int>(1.402 * 65536);
        constexpr int coef_g_1 = static_cast<int>(0.344136 * 65536);
        constexpr int coef_g_2 = static_cast<int>(0.714136 * 65536);
        constexpr int coef_b = static_cast<int>(1.772 * 65536);

        cb -= 128;
        cr -= 128;
        int r = y + ((coef_r * cr) >> 16);
        int g = y - ((coef_g_1 * cb + coef_g_2 * cr) >> 16);
        int b = y + ((coef_b * cb) >> 16);

        return {
            static_cast<uint8_t>(std::clamp(r, 0, 255)),
            static_cast<uint8_t>(std::clamp(g, 0, 255)),
            static_cast<uint8_t>(std::clamp(b, 0, 255))
        };
    }
    static_assert(ycbcrToRGB(255, 128, 128) == std::make_tuple(255, 255, 255));
    static_assert(ycbcrToRGB(1, 2, 3) == std::make_tuple(0, 134, 0));

    inline void convertYCbCr444ToBGRA32(
        std::vector<uint8_t>& dst,
        uint32_t width,
        uint32_t height,
        const std::vector<int>& srcY,
        const std::vector<int>& srcCb,
        const std::vector<int>& srcCr
    ) noexcept
    {
        assert(dst.size() == width * height * 4);
        assert(srcY.size() == width * height);
        assert(srcCb.size() == width * height);
        assert(srcCr.size() == width * height);

#pragma omp simd
        for (size_t row = 0; row < height; ++row) {
            size_t stride = row * width;

            for (size_t col = 0; col < width; ++col) {
                size_t srcOffset = stride + col;
                assert(srcOffset < srcY.size());

                size_t dstOffset = srcOffset * 4;
                assert(dstOffset + 3 < dst.size());

                int y = srcY[srcOffset];
                int cb = srcCb[srcOffset];
                int cr = srcCr[srcOffset];

                auto [r, g, b] = image::ycbcrToRGB(y, cb, cr);

                dst[dstOffset + 0] = b;
                dst[dstOffset + 1] = g;
                dst[dstOffset + 2] = r;
                dst[dstOffset + 3] = 0xFF;
            }
        }
    }

} // namespace image
