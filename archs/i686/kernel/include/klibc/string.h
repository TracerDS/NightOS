#pragma once

#include <init.hpp>
#include <stddef.h>

__CPP_START__

size_t strlen(const char* str);

#ifdef __STDC_WANT_LIB_EXT1__
size_t strnlen_s(const char* str, size_t strsz);
#endif

int memcmp(const void* lhs, const void* rhs, size_t count);
void* memset(void* dest, int ch, size_t count);
void* memset_explicit(void* dest, int ch, size_t count);
void* memcpy(void* dest, const void* src, size_t count);

__CPP_END__