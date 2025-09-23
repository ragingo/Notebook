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

} // namespace yoctocc
