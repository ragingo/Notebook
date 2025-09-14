#pragma once
#include <fstream>
#include <Token.hpp>

namespace yoctocc {

std::shared_ptr<Token> tokenize(std::ifstream& ifs) {
    auto head = std::make_shared<Token>();
    auto current = head;

    char ch;
    while (ifs.get(ch)) {
        if (ch >= '0' && ch <= '9') {
            auto next = std::make_shared<Token>();
            next->type = TokenType::DIGIT;
            next->numberValue = ch - '0';
            current->next = next;
            current = next;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            auto next = std::make_shared<Token>();
            next->type = TokenType::PUNCTUATOR;
            next->originalValue = ch;
            current->next = next;
            current = next;
        } else if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
            // skip
            continue;
        } else {
            auto next = std::make_shared<Token>();
            next->type = TokenType::UNKNOWN;
            next->originalValue = ch;
            current->next = next;
            current = next;
        }
    }

    auto terminator = std::make_shared<Token>();
    terminator->type = TokenType::TERMINATOR;
    current->next = terminator;

    return head->next;
}

} // namespace yoctocc
