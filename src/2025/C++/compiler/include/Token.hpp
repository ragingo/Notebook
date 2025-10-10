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
    std::shared_ptr<Token> next;
};

namespace token {
    inline bool has_next(const std::shared_ptr<Token>& token) {
        return token && token->next;
    }

    inline bool is_punctuator(const std::shared_ptr<Token>& token) {
        return token && token->type == TokenType::PUNCTUATOR;
    }

    inline bool is_digit(const std::shared_ptr<Token>& token) {
        return token && token->type == TokenType::DIGIT;
    }

    inline bool is_arithmetic_operator(const std::shared_ptr<Token>& token) {
        return is_punctuator(token) && (token->originalValue == "+" || token->originalValue == "-" || token->originalValue == "*" || token->originalValue == "/");
    }

    inline std::string to_string(const std::shared_ptr<Token>& token) {
        if (is_digit(token)) {
            return std::format("type: {}, value: {}, next: {}", token->type, token->numberValue, token->next ? "yes" : "no");
        }
        return std::format("type: {}, value: {}, next: {}", token->type, token->originalValue, token->next ? "yes" : "no");
    }

    inline bool equals(const std::shared_ptr<Token>& token, std::string_view originalValue) {
        return token && token->originalValue == originalValue;
    }

    inline std::shared_ptr<Token> skip(const std::shared_ptr<Token>& token) {
        return token ? token->next : nullptr;
    }

    inline std::shared_ptr<Token> skip_if(const std::shared_ptr<Token>& token, std::string_view originalValue) {
        if (token && token->originalValue == originalValue) {
            return token->next;
        }
        return token;
    }

    inline std::shared_ptr<Token> skip_if(const std::shared_ptr<Token>& token, std::function<bool(const std::shared_ptr<Token>&)> predicate) {
        if (token && predicate(token)) {
            return token->next;
        }
        return token;
    }
}

} // namespace yoctocc
