#pragma once
#include <array>
#include <concepts>
#include <limits>
#include <numbers>

namespace math
{
    inline constexpr double pi = std::numbers::pi;
    inline constexpr double sqrt2 = std::numbers::sqrt2;
    inline constexpr double sqrt8 = 2.0 * std::numbers::sqrt2;

    inline constexpr double abs(double x)
    {
        return x < 0.0 ? -x : x;
    }
    static_assert(abs(-1.0) == 1.0);
    static_assert(abs(1.0) == 1.0);

    inline constexpr double deg2rad(double deg)
    {
        return deg * pi / 180.0;
    }
    static_assert(abs(deg2rad(30) - (pi / 6)) <= std::numeric_limits<double>::epsilon());
    static_assert(abs(deg2rad(45) - (pi / 4)) <= std::numeric_limits<double>::epsilon());
    static_assert(abs(deg2rad(180) - pi) <= std::numeric_limits<double>::epsilon());

    template<double x, int max = 10>
    inline constexpr double sin()
    {
        double y = std::clamp(x, -2.0 * pi, 2.0 * pi);
        double sum = y;
        double t = y;
        for (int n = 1; n <= max; n++) {
            t *= -(y * y) / ((2 * n + 1) * (2 * n));
            sum += t;
        }
        return sum;
    }
    constexpr double __sin45 = sin<deg2rad(45)>();
    static_assert(abs(__sin45 - (1.0 / sqrt2)) <= std::numeric_limits<double>::epsilon());

    template<double x, int max = 10>
    inline constexpr double cos()
    {
        return sin<pi / 2.0 - x, max>();
    }
    constexpr double __cos45 = cos<deg2rad(45)>();
    static_assert(abs(__cos45 - (1.0 / sqrt2)) <= std::numeric_limits<double>::epsilon());

    template<
        std::integral ElementType = int,
        typename InOut = std::array<ElementType, 8 * 8>
    >
    inline void idct(const InOut& input, InOut& output)
    {
        constexpr int N = 8;
        constexpr double c0 = 1.0 / sqrt8;
        constexpr double c1 = 0.5; //std::sqrt(2.0 / N);

        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                double sum = 0.0;
                for (int v = 0; v < N; ++v) {
                    double cosv = std::cos((2 * y + 1) * v * pi / (2 * N));
                    double cv = ((v == 0) ? c0 : c1) * cosv;

                    for (int u = 0; u < N; ++u) {
                        double cosu = std::cos((2 * x + 1) * u * pi / (2 * N));
                        double cu = ((u == 0) ? c0 : c1) * cosu;

                        int offset = v * N + u;
                        sum += static_cast<double>(input[offset]) * cu * cv;
                    }
                }
                int offset = y * N + x;
                output[offset] = static_cast<ElementType>(sum);
            }
        }
    }

} // namespace math
