#pragma once
#include <format>
#include <string_view>

enum class LinkerDirective {
    EXTERN,
    GLOBAL
};

template <>
struct std::formatter<LinkerDirective> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const LinkerDirective& dir, std::format_context& ctx) const -> std::format_context::iterator {
        using enum LinkerDirective;
        std::string_view name;
        switch (dir) {
            case EXTERN: name = "extern"; break;
            case GLOBAL: name = "global"; break;
        }
        return std::format_to(ctx.out(), "{}", name);
    }
};
