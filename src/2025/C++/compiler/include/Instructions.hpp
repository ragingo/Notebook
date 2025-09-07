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

    inline constexpr std::string syscall() {
        return std::format("{}", OpCode::SYSCALL);
    }

} // namespace yoctocc
