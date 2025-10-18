#pragma once
#include <print>
#include <string>
#include <string_view>

namespace yoctocc::Log {

inline std::string sourceCode;

inline void error(std::string_view message, bool exit = true) {
    std::println(stderr, "\033[31m{}\033[0m", message);
    if (exit) {
        std::exit(1);
    }
}

inline void error(size_t location, std::string_view message, bool exit = true) {
    std::println(stderr, "\033[31mError at {}: {}\033[0m", location, message);
    if (exit) {
        std::exit(1);
    }
}

inline void error(size_t location, const std::string& message, bool exit = true) {
    error(location, std::string_view(message), exit);
}

} // namespace yoctocc::Log
