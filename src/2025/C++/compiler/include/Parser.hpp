#pragma once
#include "Node/Node.hpp"
#include "Token.hpp"

namespace yoctocc {

std::shared_ptr<Node> parse_expression(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_assignment(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_statement(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_expression_statement(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_equality(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_relational(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_additive(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_multiply(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_unary(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);
std::shared_ptr<Node> parse_primary(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token);

class Parser final {
public:
    std::shared_ptr<Node> parse(std::shared_ptr<Token>& token) {
        assert(token);
        if (!token) {
            return nullptr;
        }

        auto head = std::make_shared<Node>();
        auto current = head;

        while (token->type != TokenType::TERMINATOR) {
            current = current->next = parse_statement(token, token);
        }

        return head->next;
    }
};

std::shared_ptr<Node> parse_expression(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    return parse_assignment(result, token);
}

std::shared_ptr<Node> parse_assignment(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    auto node = parse_equality(token, token);

    if (token->type == TokenType::PUNCTUATOR && token->originalValue == "=") {
        node = create_binary_node(NodeType::ASSIGN, node, parse_assignment(token, token->next));
    }

    *result = *token;
    return node;
}

std::shared_ptr<Node> parse_statement(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    return parse_expression_statement(result, token);
}

std::shared_ptr<Node> parse_expression_statement(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    auto node = create_unary_node(NodeType::EXPRESSION_STATEMENT, parse_expression(token, token));
    if (token->originalValue == ";") {
        token = token->next;
    }
    *result = *token;
    return node;
}

std::shared_ptr<Node> parse_equality(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    auto node = parse_relational(token, token);

    while (true) {
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "==") {
            node = create_binary_node(NodeType::EQUAL, node, parse_relational(token, token->next));
            continue;
        }
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "!=") {
            node = create_binary_node(NodeType::NOT_EQUAL, node, parse_relational(token, token->next));
            continue;
        }
        *result = *token;
        return node;
    }
}

std::shared_ptr<Node> parse_relational(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    auto node = parse_additive(token, token);

    while (true) {
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "<") {
            node = create_binary_node(NodeType::LESS, node, parse_additive(token, token->next));
            continue;
        }
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "<=") {
            node = create_binary_node(NodeType::LESS_EQUAL, node, parse_additive(token, token->next));
            continue;
        }
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == ">") {
            node = create_binary_node(NodeType::GREATER, node, parse_additive(token, token->next));
            continue;
        }
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == ">=") {
            node = create_binary_node(NodeType::GREATER_EQUAL, node, parse_additive(token, token->next));
            continue;
        }
        *result = *token;
        return node;
    }
}

std::shared_ptr<Node> parse_additive(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    auto node = parse_multiply(token, token);

    while (true) {
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "+") {
            node = create_binary_node(NodeType::ADD, node, parse_multiply(token, token->next));
            continue;
        }
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "-") {
            node = create_binary_node(NodeType::SUB, node, parse_multiply(token, token->next));
            continue;
        }
        *result = *token;
        return node;
    }
}

std::shared_ptr<Node> parse_multiply(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    auto node = parse_unary(token, token);

    while (true) {
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "*") {
            node = create_binary_node(NodeType::MUL, node, parse_unary(token, token->next));
            continue;
        }
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "/") {
            node = create_binary_node(NodeType::DIV, node, parse_unary(token, token->next));
            continue;
        }
        *result = *token;
        return node;
    }
}

std::shared_ptr<Node> parse_unary(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    if (token->type == TokenType::PUNCTUATOR && token->originalValue == "+") {
        return parse_primary(result, token->next);
    }
    if (token->type == TokenType::PUNCTUATOR && token->originalValue == "-") {
        return create_unary_node(NodeType::NEGATE, parse_unary(result, token->next));
    }
    return parse_primary(result, token);
}

std::shared_ptr<Node> parse_primary(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
    if (token->type == TokenType::PUNCTUATOR && token->originalValue == "(") {
        auto node = parse_expression(token, token->next);
        if (token->type != TokenType::PUNCTUATOR || token->originalValue != ")") {
            // TODO: エラーハンドリング
        }
        *result = *token->next;
        return node;
    }

    if (token->type == TokenType::IDENTIFIER) {
        auto node = create_variable_node(token->originalValue);
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
