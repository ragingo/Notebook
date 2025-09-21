#pragma once
#include <format>
#include <string_view>

namespace yoctocc {

enum class Register {
    RAX,
    RBX,
    RCX,
    RDX,
    RSI,
    RDI,
    RBP,
    RSP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    EAX,
    EBX,
    ECX,
    EDX,
    AL,
    AH,
    AX,
    BL,
    BH,
    BX,
    CL,
    CH,
    CX,
    DL,
    DH,
    DX,
};

} // namespace yoctocc

template <>
struct std::formatter<yoctocc::Register> {
    constexpr auto parse(std::format_parse_context& ctx) -> std::format_parse_context::iterator {
        return ctx.begin();
    }

    auto format(const yoctocc::Register& reg, std::format_context& ctx) const -> std::format_context::iterator {
        using enum yoctocc::Register;
        std::string_view name;
        switch (reg) {
            case RAX: name = "rax"; break;
            case RBX: name = "rbx"; break;
            case RCX: name = "rcx"; break;
            case RDX: name = "rdx"; break;
            case RSI: name = "rsi"; break;
            case RDI: name = "rdi"; break;
            case RBP: name = "rbp"; break;
            case RSP: name = "rsp"; break;
            case R8:  name = "r8";  break;
            case R9:  name = "r9";  break;
            case R10: name = "r10"; break;
            case R11: name = "r11"; break;
            case R12: name = "r12"; break;
            case R13: name = "r13"; break;
            case R14: name = "r14"; break;
            case R15: name = "r15"; break;
            case EAX: name = "eax"; break;
            case EBX: name = "ebx"; break;
            case ECX: name = "ecx"; break;
            case EDX: name = "edx"; break;
            case AL:  name = "al";  break;
            case AH:  name = "ah";  break;
            case AX:  name = "ax";  break;
            case BL:  name = "bl";  break;
            case BH:  name = "bh";  break;
            case BX:  name = "bx";  break;
            case CL:  name = "cl";  break;
            case CH:  name = "ch";  break;
            case CX:  name = "cx";  break;
            case DL:  name = "dl";  break;
            case DH:  name = "dh";  break;
            case DX:  name = "dx";  break;
            default:  name = "???"; break;
        }
        return std::format_to(ctx.out(), "{}", name);
    }
};
