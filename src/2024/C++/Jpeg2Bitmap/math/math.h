#pragma once
#include <array>
#include <concepts>
#include <numbers>

namespace math
{
    template<
        std::integral ElementType,
        int M,
        int N,
        typename InOut = std::array<ElementType, M * N>
    >
    inline void idct(const InOut& input, InOut& output)
    {
        const double cu0 = 1.0 / std::sqrt(M);
        const double cv0 = 1.0 / std::sqrt(N);
        const double cu1 = std::sqrt(2.0 / M);
        const double cv1 = std::sqrt(2.0 / N);
        constexpr double pi = std::numbers::pi;

        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < M; ++x) {
                double sum = 0.0;
                for (int v = 0; v < N; ++v) {
                    double cosv = std::cos((2 * y + 1) * v * pi / (2 * N));
                    double cv = ((v == 0) ? cv0 : cv1) * cosv;

                    for (int u = 0; u < M; ++u) {
                        double cosu = std::cos((2 * x + 1) * u * pi / (2 * M));
                        double cu = ((u == 0) ? cu0 : cu1) * cosu;

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
