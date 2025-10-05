#pragma once

namespace yoctocc {

constexpr bool isIdentifierChar(char ch, bool isFirstChar) {
    bool isAlpha = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    bool isUnderscore = (ch == '_');
    bool isNumber = (ch >= '0' && ch <= '9');

    if (isFirstChar) {
        return isAlpha || isUnderscore;
    } else {
        return isAlpha || isUnderscore || isNumber;
    }
}

} // namespace yoctocc
