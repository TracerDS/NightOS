#include <klibc/string.h>

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