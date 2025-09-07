#pragma once
#include <format>
#include <string_view>

enum class OpCode {
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    INC,
    DEC,
    PUSH,
    POP,
    CALL,
    RET,
    JMP,
    JE,
    JNE,
    JL,
    JLE,
    JG,
    JGE,
    SYSCALL
};

template <>
struct std::formatter<OpCode> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const OpCode& op, std::format_context& ctx) const -> std::format_context::iterator {
        using enum OpCode;
        std::string_view name;
        switch (op) {
            case MOV: name = "mov"; break;
            case ADD: name = "add"; break;
            case SUB: name = "sub"; break;
            case MUL: name = "mul"; break;
            case DIV: name = "div"; break;
            case INC: name = "inc"; break;
            case DEC: name = "dec"; break;
            case PUSH: name = "push"; break;
            case POP: name = "pop"; break;
            case CALL: name = "call"; break;
            case RET: name = "ret"; break;
            case JMP: name = "jmp"; break;
            case JE: name = "je"; break;
            case JNE: name = "jne"; break;
            case JL: name = "jl"; break;
            case JLE: name = "jle"; break;
            case JG: name = "jg"; break;
            case JGE: name = "jge"; break;
            case SYSCALL: name = "syscall"; break;
        }
        return std::format_to(ctx.out(), "{}", name);
    }
};
