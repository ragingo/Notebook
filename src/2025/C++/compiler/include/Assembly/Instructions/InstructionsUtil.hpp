#pragma once
#include <format>
#include <string>
#include <vector>
#include "Assembly/Assembly.hpp"

namespace yoctocc {

    template <size_t STACK_SIZE>
    inline constexpr std::vector<std::string> prologue() {
        return {
            push(Register::RBP),
            mov(Register::RBP, Register::RSP),
            sub(Register::RSP, STACK_SIZE)
        };
    }

    inline std::vector<std::string> prologue(int stackSize) {
        return {
            push(Register::RBP),
            mov(Register::RBP, Register::RSP),
            sub(Register::RSP, stackSize)
        };
    }

    inline constexpr std::vector<std::string> epilogue() {
        return {
            mov(Register::RSP, Register::RBP),
            pop(Register::RBP),
            ret()
        };
    }

}
