#pragma once
#include <format>
#include <string>
#include <vector>
#include "OpCode.hpp"
#include "Register.hpp"

namespace yoctocc {

    inline constexpr std::string block(const std::vector<std::string>& lines, int depth = 0) {
        std::string result;
        for (const auto& line : lines) {
            result += std::string(depth * 4, ' ') + line + "\n";
        }
        return result;
    }

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

    inline constexpr std::string cqo() {
        return std::format("{}", OpCode::CQO);
    }

    inline constexpr std::string neg(Register dest) {
        return std::format("{} {}", OpCode::NEG, dest);
    }

    inline constexpr std::string syscall() {
        return std::format("{}", OpCode::SYSCALL);
    }

    inline constexpr std::string push(Register src) {
        return std::format("{} {}", OpCode::PUSH, src);
    }

    inline constexpr std::string pop(Register dest) {
        return std::format("{} {}", OpCode::POP, dest);
    }

} // namespace yoctocc
