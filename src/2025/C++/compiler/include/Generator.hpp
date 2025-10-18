#pragma once

#include <memory>
#include <string>
#include <vector>

namespace yoctocc {

struct Function;
struct Node;

class Generator final {
public:
    std::vector<std::string> run(const std::shared_ptr<Function>& func);

private:
    static constexpr int alignTo(int n, int align) {
        return (n + align - 1) / align * align;
    }
    void assignLocalVariableOffsets(const std::shared_ptr<Function>& func);
    void generateAddress(const std::shared_ptr<Node>& node);
    void generateStatement(const std::shared_ptr<Node>& node);
    void generateExpression(const std::shared_ptr<Node>& node);

private:
    std::vector<std::string> lines{};
    uint64_t labelCount = 0UL;
};

} // namespace yoctocc
