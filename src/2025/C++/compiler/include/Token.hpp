#pragma once
#include <format>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

namespace yoctocc {

// https://learn.microsoft.com/ja-jp/cpp/c-language/lexical-grammar?view=msvc-170
enum class TokenType {
    UNKNOWN,
    IDENTIFIER,
    PUNCTUATOR,
    KEYWORD,
    DIGIT,
    TERMINATOR,
};

} // namespace yoctocc

template <>
struct std::formatter<yoctocc::TokenType> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const yoctocc::TokenType& type, std::format_context& ctx) const -> std::format_context::iterator {
        using enum yoctocc::TokenType;
        std::string_view name;
        switch (type) {
            case UNKNOWN:    name = "UNKNOWN";    break;
            case PUNCTUATOR: name = "PUNCTUATOR"; break;
            case DIGIT:      name = "DIGIT";      break;
            case TERMINATOR: name = "TERMINATOR"; break;
            default:         name = "???";        break;
        }
        return std::format_to(ctx.out(), "{}", name);
    }
};

namespace yoctocc {

struct Token {
    TokenType type;
    std::string originalValue;
    int numberValue;
    size_t location;
    std::shared_ptr<Token> next;

    Token(TokenType type = TokenType::UNKNOWN) : type(type), numberValue(0), location(0) {}
};

namespace token {
    inline bool is(const std::shared_ptr<Token>& token, std::string_view originalValue) {
        return token && token->originalValue == originalValue;
    }

    inline std::shared_ptr<Token> skipIf(const std::shared_ptr<Token>& token, std::string_view originalValue) {
        if (token && token->originalValue == originalValue) {
            return token->next;
        }
        return token;
    }

    inline std::shared_ptr<Token> skipIf(const std::shared_ptr<Token>& token, std::function<bool(const std::shared_ptr<Token>&)> predicate) {
        if (token && predicate(token)) {
            return token->next;
        }
        return token;
    }
}

} // namespace yoctocc
