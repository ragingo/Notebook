#pragma once
#include <cstdint>
#include <string>

namespace yoctocc {

constexpr std::string ENTRY_POINT_NAME = "_start";

} // namespace yoctocc

#include "LinkerDirective.hpp"
#include "OpCode.hpp"
#include "Register.hpp"
#include "Section.hpp"
#include "SystemCall.hpp"

#include "Instructions/Instructions.hpp"
#include "Instructions/InstructionsUtil.hpp"
#include "AssemblyWriter.hpp"
