#pragma once
#include <array>
#include <concepts>
#include <numbers>

namespace math
{
    template<
        std::integral ElementType = int,
        typename InOut = std::array<ElementType, 8 * 8>
    >
    inline void idct(const InOut& input, InOut& output)
    {
        constexpr int N = 8;
        constexpr double sqrt2 = std::numbers::sqrt2;
        constexpr double pi = std::numbers::pi;
        constexpr double sqrt8 = 2.0 * sqrt2;
        constexpr double c0 = 1.0 / sqrt8;
        const double c1 = std::sqrt(2.0 / N);

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
