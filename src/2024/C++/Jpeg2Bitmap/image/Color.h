#pragma once
#include <immintrin.h>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <span>
#include <tuple>
#include <vector>

namespace image
{
    enum class PixelFormat
    {
        // YCbCr 4:4:4
        YCBCR444_UINT,
        // YCbCr 4:2:0
        YCBCR420_UINT,
        // BGRA 32bit
        B8G8R8A8_UINT,
    };

    template<std::integral T = int, std::integral R = uint8_t>
    constexpr std::tuple<R, R, R> ycbcrToRGB(T y, T cb, T cr) noexcept
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
            static_cast<R>(std::clamp(r, 0, 255)),
            static_cast<R>(std::clamp(g, 0, 255)),
            static_cast<R>(std::clamp(b, 0, 255))
        };
    }
    static_assert(ycbcrToRGB(255, 128, 128) == std::make_tuple(255, 255, 255));
    static_assert(ycbcrToRGB(1, 2, 3) == std::make_tuple(0, 134, 0));

    constexpr std::tuple<double, double> getHorizontalSamplingFactor(PixelFormat format) noexcept
    {
        switch (format) {
        case image::PixelFormat::YCBCR444_UINT:
            return { 1.0, 1.0 };
        case image::PixelFormat::YCBCR420_UINT:
            return { 0.5, 0.5 };
        default:
            assert(false);
            return { 1.0, 1.0 };
        }
    }

    constexpr std::tuple<double, double> getVerticalSamplingFactor(PixelFormat format) noexcept
    {
        switch (format) {
        case image::PixelFormat::YCBCR444_UINT:
            return { 1.0, 1.0 };
        case image::PixelFormat::YCBCR420_UINT:
            return { 0.5, 0.5 };
        default:
            assert(false);
            return { 1.0, 1.0 };
        }
    }

    template <PixelFormat SrcFormat>
    inline void convertYCbCrToBGRA32(
        int width,
        int height,
        std::span<uint8_t> dst,
        std::span<int16_t> srcY,
        std::span<int16_t> srcCb,
        std::span<int16_t> srcCr
    ) noexcept
    {
        assert(dst.size() == width * height * 4);
        assert(srcY.size() == width * height);

        constexpr auto hFactor = getHorizontalSamplingFactor(SrcFormat);
        constexpr auto vFactor = getVerticalSamplingFactor(SrcFormat);

        const auto& [cbHFactor, cbVFactor] = hFactor;
        const auto& [crHFactor, crVFactor] = vFactor;

        const int cbWidth = static_cast<int>(width * cbHFactor);
        const int crWidth = static_cast<int>(width * crHFactor);
        assert(srcCb.size() == cbWidth * height * cbVFactor);
        assert(srcCr.size() == crWidth * height * crVFactor);

        struct Pixel
        {
            uint8_t b = 0;
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t a = 0;
        };

        std::vector<Pixel> pixels(width * height);
        size_t index = 0;

        for (size_t row = 0; row < height; ++row) {
            size_t yRow = row * width;
            size_t cbRow = static_cast<size_t>(row * cbHFactor) * cbWidth;
            size_t crRow = static_cast<size_t>(row * crHFactor) * crWidth;

            for (size_t col = 0; col < width; ++col) {
                size_t yOffset = yRow + col;
                size_t cbOffset = cbRow + static_cast<size_t>(col * cbVFactor);
                size_t crOffset = crRow + static_cast<size_t>(col * crVFactor);

                int16_t y = srcY[yOffset];
                int16_t cb = srcCb[cbOffset];
                int16_t cr = srcCr[crOffset];

                const auto& [r, g, b] = image::ycbcrToRGB(y, cb, cr);
                pixels[index++] = { b, g, r, 0xFF }; // 結局ここが遅い
            }
        }

        for (size_t i = 0; i < pixels.size(); i += 8) {
            __m256i v = _mm256_load_si256(reinterpret_cast<const __m256i*>(&pixels[i]));
            _mm256_store_si256(reinterpret_cast<__m256i*>(&dst[i * 4]), v);
        }
    }

    template <>
    inline void convertYCbCrToBGRA32<PixelFormat::YCBCR444_UINT>(
        int width,
        int height,
        std::span<uint8_t> dst,
        std::span<int16_t> srcY,
        std::span<int16_t> srcCb,
        std::span<int16_t> srcCr
    ) noexcept
    {
        assert(dst.size() == width * height * 4);
        assert(srcY.size() == width * height);
        assert(srcCb.size() == width * height);
        assert(srcCr.size() == width * height);

        struct Pixel
        {
            uint8_t b = 0;
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t a = 0;
        };

        std::vector<Pixel> pixels(width * height);
        size_t index = 0;

        for (size_t i = 0; i < srcY.size(); ++i) {
            const auto& [r, g, b] = image::ycbcrToRGB(srcY[i], srcCb[i], srcCr[i]);
            pixels[index++] = { b, g, r, 0xFF };
        }

        for (size_t i = 0; i < pixels.size(); i += 8) {
            __m256i v = _mm256_load_si256(reinterpret_cast<const __m256i*>(&pixels[i]));
            _mm256_store_si256(reinterpret_cast<__m256i*>(&dst[i * 4]), v);
        }
    }

} // namespace image
