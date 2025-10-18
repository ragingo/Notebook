#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "Section.hpp"
#include "LinkerDirective.hpp"

namespace yoctocc {

class AssemblyWriter final {
public:
    void section(Section section) noexcept;
    void section_text_symbol(LinkerDirective directive, const std::string& symbol) noexcept;
    void func(const std::string& label, std::vector<std::string> body = {}) noexcept;
    void func(const std::string& label, std::function<std::vector<std::string>()> body) noexcept;
    void compile() noexcept;
    void clear() noexcept;
    const std::vector<std::string>& get_code() const noexcept;

private:
    std::vector<std::string> _code{};
    std::multimap<Section, std::vector<std::string>> _sections{};
};

} // namespace yoctocc
