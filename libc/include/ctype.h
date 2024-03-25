#pragma once

#ifndef __NOS_CTYPE_H__
#define __NOS_CTYPE_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool isalnum(char c);
bool isalpha(char c);
bool iscntrl(char c);
bool isdigit(char c);
bool isgraph(char c);
bool islower(char c);
bool isprint(char c);
bool ispunct(char c);
bool isspace(char c);
bool isupper(char c);
bool isxdigit(char c);
bool tolower(char c);
bool toupper(char c);

#ifdef __cplusplus
}
#endif

#endif