#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "def.hpp"
#include "AssemblyWriter.hpp"
#include "Node.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

using namespace yoctocc;
using enum LinkerDirective;
using enum OpCode;
using enum Register;
using enum Section;
using enum SystemCall;

std::vector<std::string> parsed_ast{};

void parse_ast(std::shared_ptr<Node> node) {
    assert(node);
    if (!node) {
        return;
    }

    if (node->type == NodeType::NUMBER) {
        parsed_ast.emplace_back(mov(RAX, node->value));
        return;
    }

    parse_ast(node->right);
    parsed_ast.emplace_back(push(RAX));

    parse_ast(node->left);
    parsed_ast.emplace_back(pop(RDI));

    switch (node->type) {
        case NodeType::NUMBER:
            // skip
            break;
        case NodeType::ADD:
            parsed_ast.emplace_back(add(RAX, RDI));
            break;
        case NodeType::SUB:
            parsed_ast.emplace_back(sub(RAX, RDI));
            break;
        case NodeType::MUL:
            parsed_ast.emplace_back(imul(RAX, RDI));
            break;
        case NodeType::DIV:
            parsed_ast.emplace_back(cqo());
            parsed_ast.emplace_back(idiv(RDI));
            break;
        default:
            break;
    }
}

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
    auto node = parse_expression(token, token);

    parse_ast(node);

    std::vector<std::string> entryPointBody{};
    entryPointBody.swap(parsed_ast);
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
