#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include "Assembly/Assembly.hpp"
#include "Node/Node.hpp"

namespace yoctocc {

    class Generator {
    public:
        std::vector<std::string> run(const std::shared_ptr<Function>& func) {
            assert(func);
            if (!func) {
                return {};
            }

            assignLocalVariableOffsets(func);
            generateStatement(func->body);

            return lines;
        }

    private:
        inline int alignTo(int n, int align) {
            return (n + align - 1) / align * align;
        }

        void assignLocalVariableOffsets(const std::shared_ptr<Function>& func) {
            assert(func);
            if (!func) {
                return;
            }

            int offset = 0;
            for (auto obj = func->locals; obj; obj = obj->next) {
                offset += 8;
                obj->offset = -offset;
            }

            func->stackSize = alignTo(offset, 16);
        }

        void generateAddress(const std::shared_ptr<Node>& node) {
            assert(node);
            if (!node) {
                return;
            }
            if (node->type == NodeType::VARIABLE) {
                int offset = node->variable->offset;
                lines.emplace_back(lea(Register::RAX, Address<Register>{Register::RBP, -offset}));
                return;
            }
        }

        void generateStatement(const std::shared_ptr<Node>& node) {
            assert(node);
            if (!node) {
                return;
            }
            if (node->type == NodeType::IF) {
                uint64_t count = labelCount++;
                auto elseLabel = makeElseLabel(count);
                auto endLabel = makeEndLabel(count);

                generateExpression(node->condition);
                // if
                lines.emplace_back(cmp(Register::RAX, 0));
                lines.emplace_back(je(elseLabel.ref()));
                // then
                generateStatement(node->then);
                lines.emplace_back(jmp(endLabel.ref()));
                // else
                lines.emplace_back(elseLabel.def());
                if (node->els) {
                    generateStatement(node->els);
                }
                lines.emplace_back(endLabel.def());
                return;
            }
            if (node->type == NodeType::BLOCK) {
                auto statement = node->body;
                while (statement) {
                    generateStatement(statement);
                    statement = statement->next;
                }
                return;
            }
            if (node->type == NodeType::RETURN) {
                generateExpression(node->left);
                lines.emplace_back(jmp(".L.return"));
                return;
            }
            if (node->type == NodeType::EXPRESSION_STATEMENT) {
                generateExpression(node->left);
                return;
            }
        }

        void generateExpression(const std::shared_ptr<Node>& node) {
            using enum Register;

            assert(node);
            if (!node) {
                return;
            }

            switch (node->type) {
                case NodeType::NUMBER:
                    lines.emplace_back(mov(RAX, node->value));
                    return;
                case NodeType::NEGATE:
                    generateExpression(node->left);
                    lines.emplace_back(neg(RAX));
                    return;
                case NodeType::VARIABLE:
                    generateAddress(node);
                    lines.emplace_back(mov(RAX, Address<Register>{RAX}));
                    return;
                case NodeType::ASSIGN:
                    generateAddress(node->left);
                    lines.emplace_back(push(RAX));
                    generateExpression(node->right);
                    lines.emplace_back(pop(RDI));
                    lines.emplace_back(mov(Address<Register>{RDI}, RAX));
                    return;
                default:
                    break;
            }

            generateExpression(node->right);
            lines.emplace_back(push(RAX));

            generateExpression(node->left);
            lines.emplace_back(pop(RDI));

            switch (node->type) {
                case NodeType::ADD:
                    lines.emplace_back(add(RAX, RDI));
                    break;
                case NodeType::SUB:
                    lines.emplace_back(sub(RAX, RDI));
                    break;
                case NodeType::MUL:
                    lines.emplace_back(imul(RAX, RDI));
                    break;
                case NodeType::DIV:
                    lines.emplace_back(cqo());
                    lines.emplace_back(idiv(RDI));
                    break;
                case NodeType::EQUAL:
                    lines.emplace_back(cmp(RAX, RDI));
                    lines.emplace_back(sete(AL));
                    lines.emplace_back(movzx(RAX, AL));
                    break;
                case NodeType::NOT_EQUAL:
                    lines.emplace_back(cmp(RAX, RDI));
                    lines.emplace_back(setne(AL));
                    lines.emplace_back(movzx(RAX, AL));
                    break;
                case NodeType::LESS:
                    lines.emplace_back(cmp(RAX, RDI));
                    lines.emplace_back(setl(AL));
                    lines.emplace_back(movzx(RAX, AL));
                    break;
                case NodeType::LESS_EQUAL:
                    lines.emplace_back(cmp(RAX, RDI));
                    lines.emplace_back(setle(AL));
                    lines.emplace_back(movzx(RAX, AL));
                    break;
                case NodeType::GREATER:
                    lines.emplace_back(cmp(RAX, RDI));
                    lines.emplace_back(setg(AL));
                    lines.emplace_back(movzx(RAX, AL));
                    break;
                case NodeType::GREATER_EQUAL:
                    lines.emplace_back(cmp(RAX, RDI));
                    lines.emplace_back(setge(AL));
                    lines.emplace_back(movzx(RAX, AL));
                    break;
                default:
                    break;
            }
        }

        std::vector<std::string> lines{};
        uint64_t labelCount = 0UL;
    };

}  // namespace yoctocc