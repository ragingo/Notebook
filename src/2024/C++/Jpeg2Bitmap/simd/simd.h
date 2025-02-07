#pragma once

#include <immintrin.h>

namespace simd {

    inline void clamp256_i32(__m256i& v, const __m256i& low, const __m256i& high)
    {
        v = _mm256_max_epi32(low, v);
        v = _mm256_min_epi32(high, v);
    }

} // namespace simd
