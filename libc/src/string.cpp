#include <string.h>

size_t strlen(const char* str) noexcept {
    size_t i = 0;
    for(; str[i]; i++);
    return i;
}