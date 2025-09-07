#pragma once
#include <cassert>
#include <concepts>
#include <cstdint>
#include <format>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "def.hpp"

namespace yoctocc {

class AssemblyWriter {
public:
    inline void add_line(const std::string& line) noexcept {
        _code.emplace_back(line + "\n");
    }

    inline void section(Section section) noexcept {
        _sections.emplace(section, std::vector<std::string>{});
    }

    inline void section_text_symbol(LinkerDirective directive, const std::string& symbol) noexcept {
        auto it = _sections.find(Section::TEXT);
        assert(it != _sections.end());
        if (it != _sections.end()) {
            it->second.emplace_back(std::format("    {} {}\n", directive, symbol));
        }
    }

    inline void func(const std::string& label, std::vector<std::string> body = {}) noexcept {
        _code.emplace_back(std::format("{}:\n", label));
        for (const auto& line : body) {
            _code.emplace_back("    " + line + "\n");
        }
    }

    inline void func(const std::string& label, std::function<std::vector<std::string>()> body) noexcept {
        _code.emplace_back(std::format("{}:\n", label));
        for (const auto& line : body()) {
            _code.emplace_back("    " + line + "\n");
        }
    }

    inline void compile() noexcept {
        std::vector<std::string> final_code{};

        for (const auto& [section, lines] : _sections) {
            final_code.emplace_back(std::format("section .{}\n", section));
            for (const auto& line : lines) {
                final_code.emplace_back(line);
            }
        }

        for (const auto& line : _code) {
            final_code.emplace_back(line);
        }

        _code = std::move(final_code);
    }

    inline void clear() noexcept {
        _code.clear();
        _sections.clear();
    }

    inline const std::vector<std::string>& get_code() const noexcept {
        return _code;
    }

private:
    std::vector<std::string> _code{};
    std::multimap<Section, std::vector<std::string>> _sections{};
};

} // namespace yoctocc
