#pragma once

#include <init.hpp>

extern "C" void __kernel_halt__();

void __kassert(const char* msg, const char* file, int line);

#ifdef __KERNEL_DEBUG__
#   define assert(condition) __kassert(#condition, __FILE__, __LINE__)
#else
#   define assert(condition) ((void)0)
#endif