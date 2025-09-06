#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "def.hpp"
#include "AssemblyWriter.hpp"

using enum OpCode;
using enum Register;
using enum LinkerDirective;
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

    std::string lhs;
    std::string rhs;
    std::string op;
    char ch;
    while (ifs.get(ch)) {
        if (ch >= '0' && ch <= '9') {
            if (op.empty()) {
                lhs += ch;
            } else {
                rhs += ch;
            }
        }
        else if (ch == '+') {
            op = '+';
        }
    }

    AssemblyWriter writer;

    writer.section_text();
    writer.section_text_symbol(GLOBAL, ENTRY_POINT_NAME);

    writer.func(ENTRY_POINT_NAME);

    // ここにコンパイル結果のアセンブリコードを書く
    writer.op(MOV, RAX, lhs);
    writer.op(ADD, RAX, rhs);

    writer.op(MOV, RAX, EXIT);
    writer.op(MOV, RDI, 0);
    writer.op(SYSCALL);

    for (const auto &line : writer.get_code()) {
        ofs << line;
    }
    ofs.close();

    return 0;
}
