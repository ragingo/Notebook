#pragma once
#include <memory>
#include "Node/NodeTypes.hpp"

namespace yoctocc {

struct Token;

std::shared_ptr<Node> createNumberNode(const std::shared_ptr<Token>& token, int value);
std::shared_ptr<Node> createUnaryNode(NodeType type, const std::shared_ptr<Token>& token, const std::shared_ptr<Node>& operand);
std::shared_ptr<Node> createBinaryNode(NodeType type, const std::shared_ptr<Token>& token, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right);
std::shared_ptr<Node> createVariableNode(const std::shared_ptr<Token>& token, const std::shared_ptr<Object>& variable);
std::shared_ptr<Node> createBlockNode(const std::shared_ptr<Token>& token, const std::shared_ptr<Node>& body = nullptr);
std::shared_ptr<Node> createAddNode(const std::shared_ptr<Token>& token, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right);
std::shared_ptr<Node> createSubNode(const std::shared_ptr<Token>& token, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right);

} // namespace yoctocc
