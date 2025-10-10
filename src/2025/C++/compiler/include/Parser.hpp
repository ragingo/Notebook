#pragma once
#include "Node/Node.hpp"
#include "Token.hpp"

namespace yoctocc {

class Parser final {
public:
    std::shared_ptr<Function> parse(std::shared_ptr<Token>& token) {
        assert(token);
        if (!token) {
            return nullptr;
        }

        token = token::skip_if(token, [](const auto& t) {
            return t->originalValue == "{";
        });


        auto func = std::make_shared<Function>();
        func->body = parseCompoundStatement(token, token);
        func->locals = _locals;
        func->stackSize = 0;

        return func;
    }

private:
    std::shared_ptr<Object> _locals;

private:
    std::shared_ptr<Object> findLocalVariable(std::shared_ptr<Token>& token) {
        for (auto var = _locals; var; var = var->next) {
            if (var->name == token->originalValue) {
                return var;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Node> parseExpression(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        return parseAssignment(result, token);
    }

    std::shared_ptr<Node> parseAssignment(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        auto node = parseEquality(token, token);

        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "=") {
            node = createBinaryNode(NodeType::ASSIGN, node, parseAssignment(token, token->next));
        }

        result = token;
        return node;
    }

    std::shared_ptr<Node> parseStatement(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        if (token->type == TokenType::KEYWORD && token->originalValue == "return") {
            auto node = createUnaryNode(NodeType::RETURN, parseExpression(token, token->next));
            if (token->originalValue == ";") {
                token = token->next;
            }
            result = token;
            return node;
        }

        if (token->originalValue == "{") {
            return parseCompoundStatement(result, token->next);
        }

        return parseExpressionStatement(result, token);
    }

    std::shared_ptr<Node> parseCompoundStatement(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        auto head = std::make_shared<Node>();
        auto current = head;
        while (token->type != TokenType::TERMINATOR && token->originalValue != "}") {
            current = current->next = parseStatement(token, token);
        }
        result = token->next;
        auto node = createBlockNode(head->next);
        return node;
    }

    std::shared_ptr<Node> parseExpressionStatement(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        if (token::equals(token, ";")) {
            result = token->next;
            return createBlockNode();
        }

        auto node = createUnaryNode(NodeType::EXPRESSION_STATEMENT, parseExpression(token, token));
        result = token::skip_if(token, ";");
        return node;
    }

    std::shared_ptr<Node> parseEquality(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        auto node = parseRelational(token, token);

        while (true) {
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == "==") {
                node = createBinaryNode(NodeType::EQUAL, node, parseRelational(token, token->next));
                continue;
            }
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == "!=") {
                node = createBinaryNode(NodeType::NOT_EQUAL, node, parseRelational(token, token->next));
                continue;
            }
            result = token;
            return node;
        }
    }

    std::shared_ptr<Node> parseRelational(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        auto node = parseAdditive(token, token);

        while (true) {
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == "<") {
                node = createBinaryNode(NodeType::LESS, node, parseAdditive(token, token->next));
                continue;
            }
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == "<=") {
                node = createBinaryNode(NodeType::LESS_EQUAL, node, parseAdditive(token, token->next));
                continue;
            }
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == ">") {
                node = createBinaryNode(NodeType::GREATER, node, parseAdditive(token, token->next));
                continue;
            }
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == ">=") {
                node = createBinaryNode(NodeType::GREATER_EQUAL, node, parseAdditive(token, token->next));
                continue;
            }
            result = token;
            return node;
        }
    }

    std::shared_ptr<Node> parseAdditive(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        auto node = parseMultiply(token, token);

        while (true) {
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == "+") {
                node = createBinaryNode(NodeType::ADD, node, parseMultiply(token, token->next));
                continue;
            }
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == "-") {
                node = createBinaryNode(NodeType::SUB, node, parseMultiply(token, token->next));
                continue;
            }
            result = token;
            return node;
        }
    }

    std::shared_ptr<Node> parseMultiply(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        auto node = parseUnary(token, token);

        while (true) {
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == "*") {
                node = createBinaryNode(NodeType::MUL, node, parseUnary(token, token->next));
                continue;
            }
            if (token->type == TokenType::PUNCTUATOR && token->originalValue == "/") {
                node = createBinaryNode(NodeType::DIV, node, parseUnary(token, token->next));
                continue;
            }
            result = token;
            return node;
        }
    }

    std::shared_ptr<Node> parseUnary(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "+") {
            return parsePrimary(result, token->next);
        }
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "-") {
            return createUnaryNode(NodeType::NEGATE, parseUnary(result, token->next));
        }
        return parsePrimary(result, token);
    }

    std::shared_ptr<Node> parsePrimary(std::shared_ptr<Token>& result, std::shared_ptr<Token>& token) {
        if (token->type == TokenType::PUNCTUATOR && token->originalValue == "(") {
            auto node = parseExpression(token, token->next);
            if (token->type != TokenType::PUNCTUATOR || token->originalValue != ")") {
                // TODO: エラーハンドリング
            }
            result = token->next;
            return node;
        }

        if (token->type == TokenType::IDENTIFIER) {
            auto var = findLocalVariable(token);
            if (!var) {
                var = std::make_shared<Object>();
                var->name = token->originalValue;
                var->next = _locals;
                _locals = var;
            }
            result = token->next;

            auto node = createVariableNode(var);
            return node;
        }

        if (token->type == TokenType::DIGIT) {
            auto node = createNumberNode(token->numberValue);
            result = token->next;
            return node;
        }

        return nullptr;
    }
};

} // namespace yoctocc
