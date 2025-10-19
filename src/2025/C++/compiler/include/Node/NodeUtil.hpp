#pragma once
#include <memory>
#include "Node/NodeTypes.hpp"

namespace yoctocc {

    struct Token;

    inline std::shared_ptr<Node> createNumberNode(const std::shared_ptr<Token>& token, int value) {
        auto node = std::make_shared<Node>();
        node->nodeType = NodeType::NUMBER;
        node->value = value;
        node->token = token;
        return node;
    }

    inline std::shared_ptr<Node> createUnaryNode(NodeType type, const std::shared_ptr<Token>& token, const std::shared_ptr<Node>& operand) {
        auto node = std::make_shared<Node>();
        node->nodeType = type;
        node->left = operand;
        node->token = token;
        return node;
    }

    inline std::shared_ptr<Node> createBinaryNode(NodeType type, const std::shared_ptr<Token>& token, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right) {
        auto node = std::make_shared<Node>();
        node->nodeType = type;
        node->left = left;
        node->right = right;
        node->token = token;
        return node;
    }

    inline std::shared_ptr<Node> createVariableNode(const std::shared_ptr<Token>& token, const std::shared_ptr<Object>& variable) {
        auto node = std::make_shared<Node>();
        node->nodeType = NodeType::VARIABLE;
        node->variable = variable;
        node->token = token;
        return node;
    }

    inline std::shared_ptr<Node> createBlockNode(const std::shared_ptr<Token>& token, const std::shared_ptr<Node>& body = nullptr) {
        auto node = std::make_shared<Node>();
        node->nodeType = NodeType::BLOCK;
        node->body = body;
        node->token = token;
        return node;
    }

} // namespace yoctocc
