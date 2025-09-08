#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "def.hpp"
#include "AssemblyWriter.hpp"
#include "Token.hpp"

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

    auto headToken = std::make_shared<Token>();
    auto currentToken = headToken;

    char ch;
    while (ifs.get(ch)) {
        if (ch >= '0' && ch <= '9') {
            auto nextToken = std::make_shared<Token>();
            nextToken->type = TokenType::DIGIT;
            nextToken->numberValue = ch - '0';
            currentToken->next = nextToken;
            currentToken = nextToken;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            auto nextToken = std::make_shared<Token>();
            nextToken->type = TokenType::PUNCTUATOR;
            nextToken->originalValue = ch;
            currentToken->next = nextToken;
            currentToken = nextToken;
        } else if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
            // skip
            continue;
        } else {
            auto nextToken = std::make_shared<Token>();
            nextToken->type = TokenType::UNKNOWN;
            nextToken->originalValue = ch;
            currentToken->next = nextToken;
            currentToken = nextToken;
        }
    }

    AssemblyWriter writer{};
    writer.clear();

    writer.section(TEXT);
    writer.section_text_symbol(GLOBAL, ENTRY_POINT_NAME);

    std::vector<std::string> entryPointBody{};
    auto token = headToken;
    while (token::has_next(token)) {
        token = token->next;

        if (token->type == TokenType::PUNCTUATOR) {
            if (token::is_arithmetic_operator(token)) {
                if (!token::has_next(token)) {
                    return 1;
                }
                if (!token::is_digit(token->next)) {
                    return 1;
                }
                char ch = token->originalValue;
                token = token->next;
                if (token->numberValue == 1) {
                    if (ch == '+') {
                        entryPointBody.emplace_back(inc(RAX));
                    }
                    else if (token->originalValue == '-') {
                        entryPointBody.emplace_back(dec(RAX));
                    }
                }
                else {
                    if (ch == '+') {
                        entryPointBody.emplace_back(add(RAX, token->numberValue));
                    }
                    else if (ch == '-') {
                        entryPointBody.emplace_back(sub(RAX, token->numberValue));
                    }
                    else if (ch == '*') {
                        entryPointBody.emplace_back(imul(RAX, token->numberValue));
                    }
                    else if (ch == '/') {
                        entryPointBody.emplace_back(mov(RBX, token->numberValue));
                        entryPointBody.emplace_back(idiv(RBX));
                    }
                }
            }
        }
        else if (token->type == TokenType::DIGIT) {
            entryPointBody.emplace_back(mov(RAX, token->numberValue));
        }
    }

    entryPointBody.emplace_back(mov(RAX, std::to_underlying(EXIT)));
    entryPointBody.emplace_back(mov(RDI, 0));
    entryPointBody.emplace_back(syscall());

    writer.func(ENTRY_POINT_NAME, entryPointBody);

    writer.compile();

    for (const auto &line : writer.get_code()) {
        ofs << line;
    }
    ofs.close();

    return 0;
}
