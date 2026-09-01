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

std::size_t strnlen_s(const char* str, std::size_t strsz) {
    if (!str)
        return 0;
    
    for (std::size_t i = 0; i < strsz; i++) {
        if (str[i] == '\0')
            return i;
    }

    return strsz;
}

char* strcpy(char* restrict dest, const char* restrict src) {
    const auto length = strlen(src);
    memcpy(dest, src, length + 1);
    return dest;
}

char* strncpy(char* restrict dest, const char* restrict src, std::size_t count) {
    std::size_t i = 0;

    for (; i < count && src[i] != '\0'; ++i)
        dest[i] = src[i];

    for (; i < count; ++i)
        dest[i] = '\0';

    return dest;
}

char* strcat(char* restrict dest, const char* restrict src) {
    char* ret = dest;

    while (*dest)
        ++dest;

    while ((*dest++ = *src++))
        ;

    return ret;
}

char* strncat(char* restrict dest, const char* restrict src, std::size_t count) {
    char* ret = dest;

    while (*dest)
        ++dest;

    while (count && (*dest++ = *src++))
        --count;

    if (count == 0)
        *dest = '\0';

    return ret;
}

int strcmp(const char* lhs, const char* rhs) {
    while (*lhs && (*lhs == *rhs)) {
        ++lhs;
        ++rhs;
    }
    return static_cast<unsigned char>(*lhs) - static_cast<unsigned char>(*rhs);
}

void* memchr(const void* ptr, int ch, size_t count) {
    auto bytes = static_cast<const unsigned char*>(ptr);
    auto target = static_cast<unsigned char>(ch);

    for (size_t i = 0; i < count; ++i) {
        if (bytes[i] == target) {
            return const_cast<unsigned char*>(bytes + i);
        }
    }

    return nullptr;
}

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

void* memmove(void* dest, const void* src, std::size_t count) {
    auto destPtr = static_cast<std::uint8_t*>(dest);
    auto srcPtr = static_cast<const std::uint8_t*>(src);

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
}

void* memccpy(void* restrict dest, const void* restrict src, int chr, size_t count) {
    if (!dest || !src || count == 0) {
        return dest;
    }

    auto p1 = static_cast<std::uint8_t*>(dest);
    auto p2 = static_cast<const std::uint8_t*>(src);

    for (std::size_t i = 0; i < count; ++i) {
        p1[i] = p2[i];
        if (p2[i] == static_cast<std::uint8_t>(chr)) {
            return p1 + i + 1;
        }
    }
    return nullptr;
}

size_t strerrorlen_s(errno_t errnum) {
    return strlen(strerror(errnum));
}

errno_t strerror_s(char* buf, rsize_t bufsz, errno_t errnum) {
    if (buf == nullptr || bufsz == 0 || bufsz > RSIZE_MAX) {
        return EINVAL;
    }

    const char* errstr = strerror(errnum);
    size_t errlen = strlen(errstr);

    if (errlen + 1 > bufsz) {
        return ERANGE;
    }

    memcpy(buf, errstr, errlen + 1);
    return 0;
}

}