#pragma once

#include <cstring>

namespace std {
    std::size_t strlen(const char* str) {
        return ::strlen(str);
    }
}

extern "C" {
    size_t strlen(const char* str) {
        size_t length = 0;
        while (str[length] != '\0') {
            length++;
        }   
        return length;
    }
}