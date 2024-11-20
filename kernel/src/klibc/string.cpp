#include <klibc/string.h>

size_t strlen(const char* str) noexcept {
    size_t len = 0;
    while (str[len++]);
    return len;
}

void* memcpy(void* destination, const void* source, size_t length) {
    char* dest = (char*)destination;
    const char* src = (const char*)source;
    for (size_t i = 0; i < length; i++) {
        dest[i] = src[i];
    }
    return destination;
}

void* memmove(void* destination, const void* source, size_t length) {
    char* dest = (char*)destination;
    const char* src = (const char*)source;
    if (dest < src) {
        for (size_t i = 0; i < length; i++) {
            dest[i] = src[i];
        }
    } else {
        for (size_t i = length; i > 0; i--) {
            dest[i - 1] = src[i - 1];
        }
    }
    return destination;
}

char* strcpy(char* destination, const char* source) {
    size_t i = 0;
    while (source[i]) {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
    return destination;
}

char* strncpy(char* destination, const char* source, size_t length) {
    size_t i = 0;
    while (source[i] && i < length) {
        destination[i] = source[i];
        i++;
    }
    while (i < length) {
        destination[i++] = '\0';
    }
    return destination;
}

char* strcat(char* destination, const char* source) {
    size_t dest_len = strlen(destination);
    size_t i = 0;
    while (source[i]) {
        destination[dest_len + i] = source[i];
        i++;
    }
    destination[dest_len + i] = '\0';
    return destination;
}

char* strncat(char* destination, const char* source, size_t length) {
    size_t dest_len = strlen(destination);
    size_t i = 0;
    while (source[i] && i < length) {
        destination[dest_len + i] = source[i];
        i++;
    }
    destination[dest_len + i] = '\0';
    return destination;
}