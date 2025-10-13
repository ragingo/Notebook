#pragma once
#include <string_view>
#include <tuple>

namespace yoctocc {

    enum class Keyword {
        RETURN,
        IF,
        ELSE,
        FOR,
        WHILE,
    };

    inline constexpr std::tuple<Keyword, std::string_view> keywords[] = {
        { Keyword::RETURN, "return" },
        { Keyword::IF, "if" },
        { Keyword::ELSE, "else" },
        { Keyword::FOR, "for" },
        { Keyword::WHILE, "while" },
    };

} // namespace yoctocc
