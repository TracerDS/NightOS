#pragma once

#ifndef __NOS_STDLIB_H__
#define __NOS_STDLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

double atof(const char* str);

int atoi(const char* str);
long atol(const char* str);
long long atoll(const char* str);

#ifdef __cplusplus
}
#endif

#endif