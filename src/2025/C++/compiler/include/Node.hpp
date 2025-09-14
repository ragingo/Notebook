#pragma once
#include <memory>

namespace yoctocc {

    enum class NodeType {
        UNKNOWN,
        ADD,
        SUB,
        MUL,
        DIV,
        NUMBER,
    };

    struct Node {
        NodeType type;
        int value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
    };

    std::shared_ptr<Node> create_number_node(int value) {
        auto node = std::make_shared<Node>();
        node->type = NodeType::NUMBER;
        node->value = value;
        return node;
    }

    std::shared_ptr<Node> create_binary_node(NodeType type, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right) {
        auto node = std::make_shared<Node>();
        node->type = type;
        node->left = left;
        node->right = right;
        return node;
    }

} // namespace yoctocc
