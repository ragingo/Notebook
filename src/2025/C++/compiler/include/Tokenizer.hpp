#pragma once
#include <fstream>
#include <memory>

namespace yoctocc {

struct Token;

std::shared_ptr<Token> tokenize(std::ifstream& ifs);

} // namespace yoctocc
