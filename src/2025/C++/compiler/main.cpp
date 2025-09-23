#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Assembly/Assembly.hpp"
#include "Generator.hpp"
#include "Node/Node.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

using namespace yoctocc;
using enum LinkerDirective;
using enum OpCode;
using enum Register;
using enum Section;
using enum SystemCall;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }
    std::string source_file = argv[1];
    std::ifstream ifs(source_file);
    if (!ifs) {
        return 1;
    }
    std::ofstream ofs("build/program.asm");
    if (!ofs) {
        return 1;
    }

    AssemblyWriter writer{};
    writer.section(TEXT);
    writer.section_text_symbol(GLOBAL, ENTRY_POINT_NAME);

    auto token = tokenize(ifs);

    Parser parser{};
    auto node = parser.parse(token);

    Generator generator{};

    std::vector<std::string> entryPointBody = prologue<('z' - 'a' + 1) * 8>();
    for (const auto& line : generator.run(node)) {
        entryPointBody.emplace_back(line);
    }
    entryPointBody.emplace_back(mov(RDI, RAX));
    entryPointBody.emplace_back(mov(RAX, std::to_underlying(EXIT)));
    entryPointBody.emplace_back(syscall());
    writer.func(ENTRY_POINT_NAME, entryPointBody);

    writer.compile();

    for (const auto &line : writer.get_code()) {
        ofs << line;
    }
    ofs.close();

    return 0;
}
