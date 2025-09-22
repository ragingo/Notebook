#pragma once
#include <format>
#include <string>
#include <vector>
#include "Assembly/Assembly.hpp"

namespace yoctocc {

    template <typename T>
    concept SourceOperandType =
        std::is_enum_v<T> && std::is_integral_v<std::underlying_type_t<T>> ||
        std::is_integral_v<T> ||
        std::is_same_v<T, std::string> ||
        std::is_same_v<T, std::string_view>;

    template <SourceOperandType T>
    inline constexpr std::string mov(Register dest, const T& src) {
        return std::format("{} {}, {}", OpCode::MOV, dest, src);
    }

    inline constexpr std::string movzx(Register dest, Register src) {
        return std::format("{} {}, {}", OpCode::MOVZX, dest, src);
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

    inline constexpr std::string cmp(Register lhs, Register rhs) {
        return std::format("{} {}, {}", OpCode::CMP, lhs, rhs);
    }

    inline constexpr std::string sete(Register dest) {
        return std::format("{} {}", OpCode::SETE, dest);
    }

    inline constexpr std::string setne(Register dest) {
        return std::format("{} {}", OpCode::SETNE, dest);
    }

    inline constexpr std::string setl(Register dest) {
        return std::format("{} {}", OpCode::SETL, dest);
    }

    inline constexpr std::string setle(Register dest) {
        return std::format("{} {}", OpCode::SETLE, dest);
    }

    inline constexpr std::string setg(Register dest) {
        return std::format("{} {}", OpCode::SETG, dest);
    }

    inline constexpr std::string setge(Register dest) {
        return std::format("{} {}", OpCode::SETGE, dest);
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

    inline constexpr std::string ret() {
        return std::format("{}", OpCode::RET);
    }

} // namespace yoctocc
