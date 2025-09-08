#pragma once
#include <format>
#include <string>
#include "OpCode.hpp"
#include "Register.hpp"

namespace yoctocc {

    template <SourceOperandType T>
    inline constexpr std::string mov(Register dest, const T& src) {
        return std::format("{} {}, {}", OpCode::MOV, dest, src);
    }

    template <SourceOperandType T>
    inline constexpr std::string add(Register dest, const T& src) {
        return std::format("{} {}, {}", OpCode::ADD, dest, src);
    }

    inline constexpr std::string inc(Register dest) {
        return std::format("{} {}", OpCode::INC, dest);
    }

    template <SourceOperandType T>
    inline constexpr std::string sub(Register dest, const T& src) {
        return std::format("{} {}, {}", OpCode::SUB, dest, src);
    }

    inline constexpr std::string dec(Register dest) {
        return std::format("{} {}", OpCode::DEC, dest);
    }

    template <SourceOperandType T>
    inline constexpr std::string imul(Register dest, const T& src) {
        return std::format("{} {}, {}", OpCode::IMUL, dest, src);
    }

    inline constexpr std::string mul(Register src) {
        return std::format("{} {}", OpCode::MUL, src);
    }

    inline constexpr std::string idiv(Register src) {
        return std::format("{} {}", OpCode::IDIV, src);
    }

    inline constexpr std::string syscall() {
        return std::format("{}", OpCode::SYSCALL);
    }

} // namespace yoctocc
