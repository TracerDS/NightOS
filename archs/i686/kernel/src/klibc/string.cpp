#include <klibc/string.h>
#include <cstdint>

size_t strlen(const char* str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }   
    return length;
}

#ifdef __STDC_WANT_LIB_EXT1__
size_t strnlen_s(const char* str, size_t strsz) {
    if (!str)
        return 0;
    
    for (size_t i = 0; i < strsz; i++) {
        if (str[i] == '\0')
            return i;
    }

    return strsz;
}
#endif

int memcmp(const void* lhs, const void* rhs, size_t count) {
    const uint8_t* p1 = static_cast<const uint8_t*>(lhs);
    const uint8_t* p2 = static_cast<const uint8_t*>(rhs);
    
    for (size_t i = 0; i < count; ++i) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

void* memset(void* dest, int ch, size_t count) {
    uint8_t* p = static_cast<uint8_t*>(dest);
    for (size_t i = 0; i < count; ++i) {
        p[i] = static_cast<uint8_t>(ch);
    }
    return dest;
}
void* memset_explicit(void* dest, int ch, size_t count) {
    void*(*volatile volatile_memset)(void*, int, size_t) = memset;
    return volatile_memset(dest, ch, count);
}

void* memcpy(void* dest, const void* src, size_t count) {
    uint8_t* p1 = static_cast<uint8_t*>(dest);
    const uint8_t* p2 = static_cast<const uint8_t*>(src);

    for (size_t i = 0; i < count; ++i) {
        p1[i] = p2[i];
    }
    return dest;
}