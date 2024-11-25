#pragma once

#include <init.hpp>
#include <stddef.h>

__CPP_START__

size_t strlen(const char* str) noexcept;
void* memcpy(void* destination, const void* source, size_t length);
void* memmove(void* destination, const void* source, size_t length);
char* strcpy(char* destination, const char* source);
char* strncpy(char* destination, const char* source, size_t length);
char* strcat(char* destination, const char* source);
char* strncat(char* destination, const char* source, size_t length);

__CPP_END__