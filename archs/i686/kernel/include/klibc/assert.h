#ifndef __KLIBC_ASSERT_H__
#define __KLIBC_ASSERT_H__

#include <init.hpp>

void __kassert(const char* msg, const char* file, int line, const char* function);

#ifdef __NOS_KERNEL_DEBUG__
#   define assert(condition) \
    ( \
        (condition) \
        ? static_cast<void>(0) \
        : __kassert(#condition, __FILE__, __LINE__, __PRETTY_FUNCTION__) \
    )
#else
#   define assert(condition) static_cast<void>(0)
#endif

#endif