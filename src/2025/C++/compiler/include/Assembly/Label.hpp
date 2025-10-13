#pragma once
#include <cstdint>
#include <string>

namespace yoctocc {

    class Label final {
    public:
        Label(const std::string& name) : name(name) {}

        inline std::string ref() const {
            return name;
        }

        inline std::string def() const {
            return name + ":";
        }

    private:
        std::string name;
    };

    inline Label makeLabel(const std::string& prefix, uint64_t id) {
        return Label(".L." + prefix + "." + std::to_string(id));
    }

    inline Label makeBeginLabel(uint64_t id) {
        return makeLabel("begin", id);
    }

    inline Label makeElseLabel(uint64_t id) {
        return makeLabel("else", id);
    }

    inline Label makeEndLabel(uint64_t id) {
        return makeLabel("end", id);
    }

} // namespace yoctocc
