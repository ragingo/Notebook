#include "Type.hpp"

#include "Logger.hpp"
#include "Node/Node.hpp"

namespace yoctocc::type {

    std::shared_ptr<Type> pointerTo(const std::shared_ptr<Type>& base) {
        auto type = std::make_shared<Type>(TypeKind::POINTER);
        type->base = base;
        return type;
    }

    void addType(const std::shared_ptr<Node>& node) {
        if (!node || node->type) {
            return;
        }

        addType(node->left);
        addType(node->right);
        addType(node->condition);
        addType(node->then);
        addType(node->els);
        addType(node->init);
        addType(node->inc);

        for (auto body = node->body; body; body = body->next) {
            addType(body);
        }

        switch (node->nodeType) {
            case NodeType::ADD:
            case NodeType::SUB:
            case NodeType::MUL:
            case NodeType::DIV:
            case NodeType::NEGATE:
            case NodeType::ASSIGN:
                node->type = node->left->type;
                return;
            case NodeType::EQUAL:
            case NodeType::NOT_EQUAL:
            case NodeType::LESS:
            case NodeType::LESS_EQUAL:
            case NodeType::GREATER:
            case NodeType::GREATER_EQUAL:
            case NodeType::VARIABLE:
            case NodeType::NUMBER:
                node->type = std::make_shared<Type>(TypeKind::INT);
                return;
            case NodeType::ADDRESS:
                node->type = pointerTo(node->left->type);
                return;
            case NodeType::DEREFERENCE:
                if (node->left->type->kind == TypeKind::POINTER) {
                    node->type = node->left->type->base;
                } else {
                    node->type = std::make_shared<Type>(TypeKind::INT);
                }
                return;
            case NodeType::BLOCK:
            case NodeType::IF:
            case NodeType::FOR:
            case NodeType::EXPRESSION_STATEMENT:
            case NodeType::RETURN:
            case NodeType::UNKNOWN:
                // skip
                return;
        }
    }

} // namespace yoctocc::type
