#pragma once
#include <format>
#include <string_view>

namespace yoctocc {

enum class Section {
    TEXT,
    DATA,
    BSS
};

} // namespace yoctocc

template <>
struct std::formatter<yoctocc::Section> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const yoctocc::Section& sec, std::format_context& ctx) const -> std::format_context::iterator {
        using enum yoctocc::Section;
        std::string_view name;
        switch (sec) {
            case TEXT: name = "text"; break;
            case DATA: name = "data"; break;
            case BSS:  name = "bss";  break;
        }
        return std::format_to(ctx.out(), "{}", name);
    }
};
