#pragma once
#include <string_view>
#include <tuple>

namespace yoctocc {

    enum class Keyword {
        RETURN,
    };

    inline constexpr std::tuple<Keyword, std::string_view> keywords[] = {
        { Keyword::RETURN, "return" },
    };

} // namespace yoctocc
