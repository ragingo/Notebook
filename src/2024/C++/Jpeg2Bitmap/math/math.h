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
        typename Input = std::array<ElementType, M * N>,
        typename View = std::mdspan<ElementType, Extents>
    >
    inline void idct(Input& input)
    {
        auto cu = [](int u) {
            return u == 0 ? std::sqrt(1.0 / M) : std::sqrt(2.0 / M);
        };
        auto cv = [](int v) {
            return v == 0 ? std::sqrt(1.0 / N) : std::sqrt(2.0 / N);
        };
        double pi = std::numbers::pi;

        auto view = View(input.data(), M, N);

        for (int v = 0; v < N; ++v) {
            for (int u = 0; u < M; ++u) {
                double sum = 0.0;
                for (int y = 0; y < N; ++y) {
                    for (int x = 0; x < M; ++x) {
                        double cosu = std::cos((2 * x + 1) * u * pi / (2 * M));
                        double cosv = std::cos((2 * y + 1) * v * pi / (2 * N));
                        sum += static_cast<double>(view[y, x]) * cosu * cosv * cu(u) * cv(v);
                    }
                }
                view[v, u] = static_cast<ElementType>(sum);
            }
        }
    }

} // namespace math
