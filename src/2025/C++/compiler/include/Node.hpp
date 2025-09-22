#pragma once
#include <memory>

namespace yoctocc {

    enum class NodeType {
        UNKNOWN,
        ADD,           // +
        SUB,           // -
        MUL,           // *
        DIV,           // /
        NEGATE,        // unary -
        EQUAL,         // ==
        NOT_EQUAL,     // !=
        LESS,          // <
        LESS_EQUAL,    // <=
        GREATER,       // >
        GREATER_EQUAL, // >=
        ASSIGN,        // =
        VARIABLE,
        EXPRESSION_STATEMENT,
        NUMBER,
    };

    struct Node {
        NodeType type;
        int value;
        std::string name;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::shared_ptr<Node> next;
    };

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
