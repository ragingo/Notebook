#pragma once
#include <cstdint>
#include <string>

namespace yoctocc {

constexpr std::string ENTRY_POINT_NAME = "_start";

template <typename T>
concept SourceOperandType =
    std::is_enum_v<T> && std::is_integral_v<std::underlying_type_t<T>> ||
    std::is_integral_v<T> ||
    std::is_same_v<T, std::string> ||
    std::is_same_v<T, std::string_view>;

} // namespace yoctocc

#include "Instructions.hpp"
#include "LinkerDirective.hpp"
#include "OpCode.hpp"
#include "Register.hpp"
#include "Section.hpp"
#include "SystemCall.hpp"

#include "AssemblyWriter.hpp"
