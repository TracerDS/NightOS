#include <cstdint>

extern "C" {
#include <klibc/string.h>

std::size_t strlen(const char* str) {
#ifdef __NOS_KERNEL_COMPILER_GCC__
    return __builtin_strlen(str);
#else
    if (!str) {
        return 0;
    }
    std::size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }   
    return length;
#endif
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
#ifdef __NOS_KERNEL_COMPILER_GCC__
    return __builtin_memcmp(lhs, rhs, count);
#else
    auto p1 = static_cast<const std::uint8_t*>(lhs);
    auto p2 = static_cast<const std::uint8_t*>(rhs);
    
    for (std::size_t i = 0; i < count; ++i) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
#endif
}

void* memset(void* dest, int ch, std::size_t count) {
#ifdef __NOS_KERNEL_COMPILER_GCC__
    return __builtin_memset(dest, ch, count);
#else
    if (!dest || count == 0) {
        return dest;
    }
    auto p = static_cast<std::uint8_t*>(dest);
    for (std::size_t i = 0; i < count; ++i) {
        p[i] = static_cast<std::uint8_t>(ch);
    }
    return dest;
#endif
}
void* memset_explicit(void* dest, int ch, std::size_t count) {
    void*(*volatile volatile_memset)(void*, int, std::size_t) = memset;
    return volatile_memset(dest, ch, count);
}

void* memcpy(void* dest, const void* src, size_t count) {
#ifdef __NOS_KERNEL_COMPILER_GCC__
    return __builtin_memcpy(dest, src, count);
#else
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
#endif
}

void* memmove(void* dest, const void* src, std::size_t count) {
#ifdef __NOS_KERNEL_COMPILER_GCC__
    return __builtin_memmove(dest, src, count);
#else
    std::uint8_t* destPtr = static_cast<std::uint8_t*>(dest);
    const std::uint8_t* srcPtr = static_cast<const std::uint8_t*>(src);

    if (destPtr == srcPtr || count == 0) {
        return dest;
    }

    if (destPtr < srcPtr) {
        while(count--) {
            *destPtr++ = *srcPtr++;
        }
    } else {
        destPtr += count;
        srcPtr += count;
        while(count--) {
            *(--destPtr) = *(--srcPtr);
        }
    }
    return dest;
#endif
}

}