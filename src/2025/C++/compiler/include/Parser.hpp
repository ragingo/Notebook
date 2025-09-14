#pragma once
#include "Node.hpp"
#include "Token.hpp"

namespace yoctocc {

std::shared_ptr<Node> parse_expression(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_multiply(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_primary(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);

std::shared_ptr<Node> parse_expression(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    auto node = parse_multiply(token, token);

    while (true) {
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == '+') {
            node = create_binary_node(NodeType::ADD, node, parse_multiply(token, token->next));
            continue;
        }
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == '-') {
            node = create_binary_node(NodeType::SUB, node, parse_multiply(token, token->next));
            continue;
        }
        *result = *token;
        return node;
    }
}

std::shared_ptr<Node> parse_multiply(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    auto node = parse_primary(token, token);

    while (true) {
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == '*') {
            node = create_binary_node(NodeType::MUL, node, parse_primary(token, token->next));
            continue;
        }
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == '/') {
            node = create_binary_node(NodeType::DIV, node, parse_primary(token, token->next));
            continue;
        }
        *result = *token;
        return node;
    }
}

std::shared_ptr<Node> parse_primary(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    if (token->type == TokenType::PUNCTUATOR && token->originalValue == '(') {
        auto node = parse_expression(token, token->next);
        if (token->type != TokenType::PUNCTUATOR || token->originalValue != ')') {
            // TODO: エラーハンドリング
        }
        *result = *token->next;
        return node;
    }

    if (token->type == TokenType::DIGIT) {
        auto node = create_number_node(token->numberValue);
        *result = *token->next;
        return node;
    }

    return nullptr;
}

} // namespace yoctocc
