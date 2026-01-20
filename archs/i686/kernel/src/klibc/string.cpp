#include <cstdint>

extern "C" {
#include <klibc/string.h>

std::size_t strlen(const char* str) {
    if (!str) {
        return 0;
    }
    std::size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }   
    return length;
}

#ifdef __STDC_WANT_LIB_EXT1__
std::size_t strnlen_s(const char* str, std::size_t strsz) {
    if (!str)
        return 0;
    
    for (std::size_t i = 0; i < strsz; i++) {
        if (str[i] == '\0')
            return i;
    }

    return strsz;
}
#endif

int memcmp(const void* lhs, const void* rhs, std::size_t count) {
    auto p1 = static_cast<const std::uint8_t*>(lhs);
    auto p2 = static_cast<const std::uint8_t*>(rhs);
    
    for (std::size_t i = 0; i < count; ++i) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

void* memset(void* dest, int ch, std::size_t count) {
    if (!dest || count == 0) {
        return dest;
    }
    auto p = static_cast<std::uint8_t*>(dest);
    for (std::size_t i = 0; i < count; ++i) {
        p[i] = static_cast<std::uint8_t>(ch);
    }
    return dest;
}
void* memset_explicit(void* dest, int ch, std::size_t count) {
    void*(*volatile volatile_memset)(void*, int, std::size_t) = memset;
    return volatile_memset(dest, ch, count);
}

void* memcpy(void* dest, const void* src, size_t count) {
    // Check for null pointers
    if (!dest || !src || count == 0) {
        return dest;
    }

    auto p1 = static_cast<std::uint8_t*>(dest);
    auto p2 = static_cast<const std::uint8_t*>(src);

    for (std::size_t i = 0; i < count; ++i) {
        p1[i] = p2[i];
    }
    return dest;
}

void* memmove(void* dest, const void* src, std::size_t n) {
    auto* destPtr = static_cast<std::uint8_t*>(dest);
    if (src >= dest && src < &(destPtr)[n]) {
        // Copy from right to left
        char* dst = (char*)dest;
        char* src_ = (char*)src;
        for (std::size_t i = 0; i < n; ++i) {
            dst[i] = src_[n - i - 1];
        }
    } else {
        // Copy from left to right
        char* dst = (char*)dest;
        char* src_ = (char*)src;
        for (std::size_t i = 0; i < n; ++i) {
            dst[i] = src_[i];
        }
    }
    return dest;
}

}