#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#include <array>

#include "logger.h"

inline uint32_t add_two_uints(uint32_t a, uint32_t b) { return a + b; }

template <uint32_t Size>
struct SizedText {
    static constexpr uint32_t size = Size + 1;
    std::array<char, size> text_{};
    uint32_t actualSize = size;

    SizedText() {
        for (uint16_t i = 0; i < size; i++) {
            text_.at(i) = '\0';
        }
        actualSize = 0;
    }

    SizedText(const char* s) {
        const char* t;
        uint32_t index = 0;
        for (t = s; *t != '\0'; t++) {
            text_.at(index) = *t;
            index++;

            if (index == size) {
                break;
            }
        }
        actualSize = std::min(index, size);
    };

    static SizedText fromString(std::string fromString) {
        return SizedText(fromString.c_str());
    }

    const char* c_str() const { return text_.data(); }

    void print() const {
        std::ostringstream ss;
        for (uint32_t i = 0; i < actualSize; i++) {
            ss << text_.at(i);
        }
        ss << std::endl;
        LOG_I() << ss.str();
    }

    uint32_t getSize() { return actualSize; }

    std::array<char, size>& get() { return text_; }
};

#endif
