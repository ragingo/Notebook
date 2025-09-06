#pragma once
#include "def.hpp"
#include <cstdint>
#include <format>
#include <string>
#include <utility>
#include <vector>

class AssemblyWriter {
public:
    inline void add_line(const std::string& line) noexcept {
        _code.emplace_back(line + "\n");
    }

    inline void section_text() noexcept {
        _code.emplace_back("section .text\n");
    }

    inline void section_text_symbol(LinkerDirective directive, const std::string& symbol) noexcept {
        _code.emplace_back(std::format("{} {}\n", directive, symbol));
    }

    inline void func(const std::string& name) noexcept {
        _code.emplace_back(std::format("{}:\n", name));
    }

    inline void op(OpCode op) noexcept {
        _code.emplace_back(std::format("    {}\n", op));
    }

    inline void op(OpCode op, Register dest) noexcept {
        _code.emplace_back(std::format("    {} {}\n", op, dest));
    }

    template <typename T>
    inline void op(OpCode op, const std::string& dest, const T& src) noexcept {
        _code.emplace_back(std::format("    {} {}, {}\n", op, dest, src));
    }

    template <typename T>
        requires std::is_enum_v<T> && std::is_integral_v<std::underlying_type_t<T>>
    inline void op(OpCode op, Register dest, const T& src) noexcept {
        _code.emplace_back(std::format("    {} {}, {}\n", op, dest, std::to_underlying(src)));
    }

    template <typename T>
    inline void op(OpCode op, Register dest, const T& src) noexcept {
        _code.emplace_back(std::format("    {} {}, {}\n", op, dest, src));
    }

    inline const std::vector<std::string>& get_code() const noexcept {
        return _code;
    }

private:
    std::vector<std::string> _code;
};
