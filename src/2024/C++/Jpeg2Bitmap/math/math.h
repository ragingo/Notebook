#pragma once
#include <array>
#include <concepts>
#include <mdspan>
#include <numbers>

namespace math
{
    template<
        std::integral ElementType,
        int M,
        int N,
        typename Extents = std::extents<int, M, N>,
        typename InOut = std::array<ElementType, M * N>,
        typename OutputView = std::mdspan<ElementType, Extents>
    >
    inline void idct(const InOut& input, InOut& output)
    {
        const double cu0 = 1.0 / std::sqrt(M);
        const double cv0 = 1.0 / std::sqrt(N);
        const double cu1 = std::sqrt(2.0 / M);
        const double cv1 = std::sqrt(2.0 / N);
        double pi = std::numbers::pi;

        auto output_view = OutputView(output.data(), M, N);

        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < M; ++x) {
                double sum = 0.0;
                for (int v = 0; v < N; ++v) {
                    for (int u = 0; u < M; ++u) {
                        double cosu = std::cos((2 * x + 1) * u * pi / (2 * M));
                        double cosv = std::cos((2 * y + 1) * v * pi / (2 * N));
                        double cu = (u == 0) ? cu0 : cu1;
                        double cv = (v == 0) ? cv0 : cv1;
                        int offset = v * N + u;
                        sum += static_cast<double>(input[offset]) * cosu * cosv * cu * cv;
                    }
                }
                output_view[y, x] = static_cast<ElementType>(sum);
            }
        }
    }

} // namespace math
