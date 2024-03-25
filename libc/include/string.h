#pragma once

#ifndef __NOS_STRING_H__
#define __NOS_STRING_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


size_t strlen(const char* string);
int memcmp(const void* a, const void* b, size_t size);

#ifdef __cplusplus
}
#endif

#endif