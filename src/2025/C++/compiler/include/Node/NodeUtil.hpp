#pragma once
#include <memory>
#include "Node/NodeTypes.hpp"

namespace yoctocc {

    std::shared_ptr<Node> create_number_node(int value) {
        auto node = std::make_shared<Node>();
        node->type = NodeType::NUMBER;
        node->value = value;
        return node;
    }

    std::shared_ptr<Node> create_unary_node(NodeType type, const std::shared_ptr<Node>& operand) {
        auto node = std::make_shared<Node>();
        node->type = type;
        node->left = operand;
        return node;
    }

    std::shared_ptr<Node> create_binary_node(NodeType type, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right) {
        auto node = std::make_shared<Node>();
        node->type = type;
        node->left = left;
        node->right = right;
        return node;
    }

    std::shared_ptr<Node> create_variable_node(const std::string& name) {
        auto node = std::make_shared<Node>();
        node->type = NodeType::VARIABLE;
        node->name = name;
        return node;
    }

} // namespace yoctocc
