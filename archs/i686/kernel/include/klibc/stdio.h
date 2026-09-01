#ifndef __KLIBC_STDIO_H__
#define __KLIBC_STDIO_H__

#include <core/init.hpp>

#include <klibc/errno.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EOF (-1)

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define BUFSIZ 2048

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

using FILE = struct __FILE;
using fpos_t = struct __fpos_t;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

FILE* fopen(
    const char* restrict filename,
    const char* restrict mode
);
errno_t fopen_s(
    FILE* restrict* restrict streamptr,
    const char* restrict filename,
    const char* restrict mode
);

FILE* freopen(
    const char* restrict filename,
    const char* restrict mode,
    FILE* restrict stream
);
errno_t freopen_s(
    FILE* restrict* restrict newstreamptr,
    const char* restrict filename,
    const char* restrict mode,
    FILE* restrict stream
);

int fclose(FILE* stream);
int fflush(FILE* stream);
void setbuf(FILE* restrict stream, char* restrict buffer);
int setvbuf(FILE* restrict stream, char* restrict buffer, int mode, size_t size);

int fputc(int ch, FILE* stream);
int fputs(const char* str, FILE* stream);
int puts(const char* str);
size_t fread(void* ptr, size_t size, size_t count, FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream);
int fprintf(FILE* stream, const char* format, ...);
int printf(const char* format, ...);
int vfprintf(FILE* stream, const char* format, va_list args);
int vprintf(const char* format, va_list args);

void perror(const char* string);

#ifdef __cplusplus
}
#endif

#endif