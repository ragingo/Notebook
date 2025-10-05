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

    struct Object {
        int offset;
        std::string name;
        std::shared_ptr<Object> next;
    };

    struct Node {
        NodeType type;
        int value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::shared_ptr<Node> next;
        std::shared_ptr<Object> variable;
    };

    struct Function {
        std::shared_ptr<Node> body;
        std::shared_ptr<Object> locals;
        int stackSize;
    };

} // namespace yoctocc
