#pragma once
#include <format>
#include <string_view>

namespace yoctocc {

enum class LinkerDirective {
    EXTERN,
    GLOBAL
};

} // namespace yoctocc

template <>
struct std::formatter<yoctocc::LinkerDirective> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const yoctocc::LinkerDirective& dir, std::format_context& ctx) const -> std::format_context::iterator {
        using enum yoctocc::LinkerDirective;
        std::string_view name;
        switch (dir) {
            case EXTERN: name = "extern"; break;
            case GLOBAL: name = "global"; break;
        }
        return std::format_to(ctx.out(), "{}", name);
    }
};
