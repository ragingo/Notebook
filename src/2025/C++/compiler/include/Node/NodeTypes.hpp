#pragma once
#include <memory>

namespace yoctocc {

    struct Token;
    struct Type;

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
        ADDRESS,       // unary &
        DEREFERENCE,   // unary *
        RETURN,        // return
        IF,            // if
        FOR,           // for or while
        BLOCK,         // { ... }
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
        NodeType nodeType;
        int value;
        std::shared_ptr<Type> type;
        std::shared_ptr<Token> token;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::shared_ptr<Node> next;
        // if or for
        std::shared_ptr<Node> condition;
        std::shared_ptr<Node> then;
        std::shared_ptr<Node> els;
        std::shared_ptr<Node> init;
        std::shared_ptr<Node> inc;
        // block
        std::shared_ptr<Node> body;
        std::shared_ptr<Object> variable;

        Node(NodeType type = NodeType::UNKNOWN): nodeType(type), value(0) {}
    };

    struct Function {
        std::shared_ptr<Node> body;
        std::shared_ptr<Object> locals;
        int stackSize;
    };

} // namespace yoctocc
