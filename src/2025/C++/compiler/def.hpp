#pragma once
#include <cstdint>
#include <format>
#include <string>

constexpr std::string ENTRY_POINT_NAME = "_start";

enum class Register {
    RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP,
    R8, R9, R10, R11, R12, R13, R14, R15
};

template <>
struct std::formatter<Register> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const Register& reg, std::format_context& ctx) const -> std::format_context::iterator {
        std::string name;
        switch (reg) {
            case Register::RAX: name = "rax"; break;
            case Register::RBX: name = "rbx"; break;
            case Register::RCX: name = "rcx"; break;
            case Register::RDX: name = "rdx"; break;
            case Register::RSI: name = "rsi"; break;
            case Register::RDI: name = "rdi"; break;
            case Register::RBP: name = "rbp"; break;
            case Register::RSP: name = "rsp"; break;
            case Register::R8:  name = "r8";  break;
            case Register::R9:  name = "r9";  break;
            case Register::R10: name = "r10"; break;
            case Register::R11: name = "r11"; break;
            case Register::R12: name = "r12"; break;
            case Register::R13: name = "r13"; break;
            case Register::R14: name = "r14"; break;
            case Register::R15: name = "r15"; break;
        }
        return std::format_to(ctx.out(), "{}", name);
    }
};

enum class OpCode {
    MOV, ADD, SUB, MUL, DIV,
    INC, DEC,
    PUSH, POP,
    CALL, RET,
    JMP, JE, JNE, JL, JLE, JG, JGE,
    SYSCALL
};

template <>
struct std::formatter<OpCode> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const OpCode& op, std::format_context& ctx) const -> std::format_context::iterator {
        std::string name;
        switch (op) {
            case OpCode::MOV: name = "mov"; break;
            case OpCode::ADD: name = "add"; break;
            case OpCode::SUB: name = "sub"; break;
            case OpCode::MUL: name = "mul"; break;
            case OpCode::DIV: name = "div"; break;
            case OpCode::INC: name = "inc"; break;
            case OpCode::DEC: name = "dec"; break;
            case OpCode::PUSH: name = "push"; break;
            case OpCode::POP: name = "pop"; break;
            case OpCode::CALL: name = "call"; break;
            case OpCode::RET: name = "ret"; break;
            case OpCode::JMP: name = "jmp"; break;
            case OpCode::JE: name = "je"; break;
            case OpCode::JNE: name = "jne"; break;
            case OpCode::JL: name = "jl"; break;
            case OpCode::JLE: name = "jle"; break;
            case OpCode::JG: name = "jg"; break;
            case OpCode::JGE: name = "jge"; break;
            case OpCode::SYSCALL: name = "syscall"; break;
        }
        return std::format_to(ctx.out(), "{}", name);
    }
};

enum class LinkerDirective {
    EXTERN, GLOBAL
};
template <>
struct std::formatter<LinkerDirective> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const LinkerDirective& dir, std::format_context& ctx) const -> std::format_context::iterator {
        std::string name;
        switch (dir) {
            case LinkerDirective::EXTERN: name = "extern"; break;
            case LinkerDirective::GLOBAL: name = "global"; break;
        }
        return std::format_to(ctx.out(), "{}", name);
    }
};

enum class SystemCall: uint16_t {
    READ      = 0,
    WRITE     = 1,
    OPEN      = 2,
    NEWFSTAT  = 5,
    LSEEK     = 8,
    NANOSLEEP = 35,
    EXIT      = 60,
    CREAT     = 85
};
