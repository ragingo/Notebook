#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include "def.hpp"

namespace yoctocc {

    class Generator {
    public:
        std::vector<std::string> run(const std::shared_ptr<Node>& root) {
            generate(root);
            return lines;
        }

    private:
        void generate(std::shared_ptr<Node> node) {
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
                    generate(node->left);
                    lines.emplace_back(neg(RAX));
                    return;
                default:
                    break;
            }

            generate(node->right);
            lines.emplace_back(push(RAX));

            generate(node->left);
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
    };

}  // namespace yoctocc