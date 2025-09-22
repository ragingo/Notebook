#pragma once
#include <format>
#include <string_view>

namespace yoctocc {

enum class OpCode {
    MOV,
    MOVZX,
    LEA,
    ADD,
    SUB,
    MUL,
    IMUL,
    IDIV,
    INC,
    DEC,
    CQO,
    NEG,
    CMP,
    SETE,
    SETNE,
    SETL,
    SETLE,
    SETG,
    SETGE,
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

} // namespace yoctocc

template <>
struct std::formatter<yoctocc::OpCode> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const yoctocc::OpCode& op, std::format_context& ctx) const -> std::format_context::iterator {
        using enum yoctocc::OpCode;
        std::string_view name;
        switch (op) {
            case MOV: name = "mov"; break;
            case MOVZX: name = "movzx"; break;
            case LEA: name = "lea"; break;
            case ADD: name = "add"; break;
            case SUB: name = "sub"; break;
            case MUL: name = "mul"; break;
            case IMUL: name = "imul"; break;
            case IDIV: name = "idiv"; break;
            case INC: name = "inc"; break;
            case DEC: name = "dec"; break;
            case CQO: name = "cqo"; break;
            case NEG: name = "neg"; break;
            case CMP: name = "cmp"; break;
            case SETE: name = "sete"; break;
            case SETNE: name = "setne"; break;
            case SETL: name = "setl"; break;
            case SETLE: name = "setle"; break;
            case SETG: name = "setg"; break;
            case SETGE: name = "setge"; break;
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
