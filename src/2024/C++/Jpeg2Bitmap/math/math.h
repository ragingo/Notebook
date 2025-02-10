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

    inline constexpr double sin(double x, int max = 10)
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
    constexpr double __sin45 = sin(deg2rad(45));
    static_assert(abs(__sin45 - (1.0 / sqrt2)) <= std::numeric_limits<double>::epsilon());

    inline constexpr double cos(double x, int max = 10)
    {
        return sin(pi / 2.0 - x, max);
    }
    constexpr double __cos45 = cos(deg2rad(45));
    static_assert(abs(__cos45 - (1.0 / sqrt2)) <= std::numeric_limits<double>::epsilon());

    template<int N = 8>
    inline constexpr double __idct_cos(int a, int b)
    {
        return cos((2 * a + 1) * b * pi / (2 * N));
    }

    template<int N = 8>
    inline constexpr std::array<double, N * N> __idct_cos_tbl()
    {
        std::array<double, N * N> tbl{};
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                tbl[y * N + x] = __idct_cos<N>(y, x);
            }
        }
        return tbl;
    }
    static_assert(__idct_cos_tbl().size() == 8 * 8);

    template<int N = 8>
    inline constexpr std::array<double, N * N> __idct_internal()
    {
        std::array<double, N * N> tbl{};

        double c0 = 1.0 / sqrt8;
        double c1 = 0.5; //std::sqrt(2.0 / N);
        auto cos_tbl = __idct_cos_tbl<N>();

        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                for (int v = 0; v < N; ++v) {
                    double cosv = cos_tbl[y * N + v];
                    double cv = ((v == 0) ? c0 : c1) * cosv;
                    tbl[y * N + v] = cv;

                    for (int u = 0; u < N; ++u) {
                        double cosu = cos_tbl[x * N + u];
                        double cu = ((u == 0) ? c0 : c1) * cosu;
                        tbl[x * N + u] = cu;
                    }
                }
            }
        }

        return tbl;
    }
    static_assert(__idct_internal().size() == 8 * 8);

    template<
        int N = 8,
        std::integral ElementType = int,
        typename InOut = std::array<ElementType, N * N>
    >
    inline void idct(const InOut& input, InOut& output)
    {
        constexpr auto tbl = __idct_internal<N>();

        std::array<double, N * N> temp{};

        // 行方向の IDCT
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                double sum = 0.0;
                for (int v = 0; v < N; ++v) {
                    double cv = tbl[y * N + v];
                    double src = static_cast<double>(input[v * N + x]);
                    sum += src * cv;
                }
                temp[y * N + x] = sum;
            }
        }

        // 列方向の IDCT
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                double sum = 0.0;
                for (int u = 0; u < N; ++u) {
                    double cu = tbl[x * N + u];
                    double src = temp[y * N + u];
                    sum += src * cu;
                }
                output[y * N + x] = static_cast<ElementType>(sum);
            }
        }
    }

} // namespace math
