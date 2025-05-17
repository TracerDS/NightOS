#pragma once

#include <stdint.h>
#include <stddef.h>

size_t strlen(const char* str);

#ifdef __STDC_WANT_LIB_EXT1__
size_t strnlen_s(const char* str, size_t strsz);
#endif