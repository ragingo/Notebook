#pragma once
#include "Assembly/Register.hpp"
#include <format>
#include <string>

namespace yoctocc {

template <typename T>
struct Address {
    T base;
    int offset;

    Address(T base, int offset = 0) : base(base), offset(offset) {}

    Address operator+(int value) const {
        return Address{base, offset + value};
    }

    Address operator-(int value) const {
        return Address{base, offset - value};
    }
};

} // namespace yoctocc

template <typename T>
struct std::formatter<yoctocc::Address<T>> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const yoctocc::Address<T>& addr, std::format_context& ctx) const -> std::format_context::iterator {
        if (addr.offset == 0) {
            return std::format_to(ctx.out(), "[{}]", addr.base);
        } else if (addr.offset > 0) {
            return std::format_to(ctx.out(), "[{} + {}]", addr.base, addr.offset);
        } else {
            return std::format_to(ctx.out(), "[{} - {}]", addr.base, -addr.offset);
        }
    }
};
