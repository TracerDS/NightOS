#pragma once

#ifdef __cplusplus
#   if __cplusplus < 201103L
#       define noexcept
#   endif
#   define __CPP_FUNC__ extern "C"
#   define __CPP_START__ __CPP_FUNC__ {
#   define __CPP_END__ }
#else
#   define noexcept
#   define __CPP_FUNC__
#   define __CPP_START__
#   define __CPP_END__
#endif

#ifdef _DEBUG
#   ifndef __KERNEL_DEBUG
#      define __KERNEL_DEBUG
#   endif
#endif

static_assert(sizeof(void*) == 4, "Kernel must be compiled for 32-bit architecture");