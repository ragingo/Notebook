#pragma once
#include <memory>
#include "Node/NodeTypes.hpp"

namespace yoctocc {

    std::shared_ptr<Node> createNumberNode(int value) {
        auto node = std::make_shared<Node>();
        node->type = NodeType::NUMBER;
        node->value = value;
        return node;
    }

    std::shared_ptr<Node> createUnaryNode(NodeType type, const std::shared_ptr<Node>& operand) {
        auto node = std::make_shared<Node>();
        node->type = type;
        node->left = operand;
        return node;
    }

    std::shared_ptr<Node> createBinaryNode(NodeType type, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right) {
        auto node = std::make_shared<Node>();
        node->type = type;
        node->left = left;
        node->right = right;
        return node;
    }

    std::shared_ptr<Node> createVariableNode(const std::shared_ptr<Object>& variable) {
        auto node = std::make_shared<Node>();
        node->type = NodeType::VARIABLE;
        node->variable = variable;
        return node;
    }

    std::shared_ptr<Node> createBlockNode(const std::shared_ptr<Node>& body) {
        auto node = std::make_shared<Node>();
        node->type = NodeType::BLOCK;
        node->body = body;
        return node;
    }

} // namespace yoctocc
