#pragma once
#include <fstream>
#include <string>
#include "Token.hpp"

namespace yoctocc {

std::shared_ptr<Token> tokenize(std::ifstream& ifs) {
    auto head = std::make_shared<Token>();
    auto current = head;

    std::string number;

    std::string content{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
    auto it = content.begin();

    while (it != content.end()) {
        char ch = *it;

        if (std::isdigit(ch)) {
            number += ch;

            if (std::next(it) != content.end()) {
                ++it;
                continue;
            }
        }

        if (!number.empty()) {
            auto next = std::make_shared<Token>();
            next->type = TokenType::DIGIT;
            next->originalValue = number;
            next->numberValue = std::stoi(number);
            current->next = next;
            current = next;
            number.clear();
        }

        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            auto next = std::make_shared<Token>();
            next->type = TokenType::PUNCTUATOR;
            next->originalValue = ch;
            current->next = next;
            current = next;
        } else if (ch == '=' || ch == '!' || ch == '<' || ch == '>') {
            auto next = std::make_shared<Token>();
            next->type = TokenType::PUNCTUATOR;
            next->originalValue = ch;
            if (std::next(it) != content.end() && *std::next(it) == '=') {
                next->originalValue += '=';
                ++it;
            }
            current->next = next;
            current = next;
        } else if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
            // skip
            ++it;
            continue;
        } else {
            auto next = std::make_shared<Token>();
            next->type = TokenType::UNKNOWN;
            next->originalValue = ch;
            current->next = next;
            current = next;
        }

        ++it;
    }

    auto terminator = std::make_shared<Token>();
    terminator->type = TokenType::TERMINATOR;
    current->next = terminator;

    return head->next;
}

} // namespace yoctocc
