#pragma once
#include <memory>

namespace yoctocc {

struct Node;

enum class TypeKind {
    INT,
    POINTER,
};

struct Type {
    TypeKind kind;
    std::shared_ptr<Type> base;

    Type(TypeKind kind): kind(kind), base(nullptr) {}
};

namespace type {
    static inline const Type intType{TypeKind::INT};

    inline bool isInteger(const std::shared_ptr<Type>& type) {
        return type && type->kind == TypeKind::INT;
    }

    std::shared_ptr<Type> pointerTo(const std::shared_ptr<Type>& base);

    void addType(const std::shared_ptr<Node>& node);
}

} // namespace yoctocc
