#ifndef __KLIBC_STRING_H__
#define __KLIBC_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <core/init.hpp>
#include <klibc/errno.h>

#include <cstddef>

char* strcpy(char* restrict dest, const char* restrict src);
char* strncpy(char* restrict dest, const char* restrict src, size_t count);
char* strcat(char* restrict dest, const char* restrict src);
char* strncat(char* restrict dest, const char* restrict src, size_t count);

size_t strlen(const char* str);
size_t strnlen_s(const char* str, size_t strsz);

int strcmp(const char* lhs, const char* rhs);

void* memchr(const void* ptr, int ch, size_t count);
int memcmp(const void* lhs, const void* rhs, size_t count);
void* memset(void* dest, int ch, size_t count);
void* memset_explicit(void* dest, int ch, size_t count);
void* memcpy(void* dest, const void* src, size_t count);
void* memmove(void* dest, const void* src, size_t count);
void* memccpy(void* restrict dest, const void* restrict src, int chr, size_t count);

extern const char* strerror(int error);
errno_t strerror_s(char* buf, rsize_t bufsz, errno_t errnum);
size_t strerrorlen_s(errno_t errnum);

#ifdef __cplusplus
}
#endif

#endif