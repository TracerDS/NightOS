#ifndef __NOS_INIT_HPP__
#define __NOS_INIT_HPP__

#ifndef __cplusplus
#   error "Kernel must be compiled as a C++ target"
#endif

static_assert(sizeof(void*) == 4, "Kernel must be compiled for 32-bit architecture");

#ifdef _DEBUG
#   ifndef __NOS_KERNEL_DEBUG__
#      define __NOS_KERNEL_DEBUG__
#   endif
#endif

#ifdef __NOS_KERNEL_DEBUG__
#   define __NOS_SERIAL_DEBUG__
#endif

#ifdef __clang__
#   define __NOS_KERNEL_COMPILER_CLANG__
#elif defined(__GNUC__) || defined(__GNUG__)
#   define __NOS_KERNEL_COMPILER_GCC__
#endif

#if defined(__NOS_KERNEL_COMPILER_CLANG__) || defined(__NOS_KERNEL_COMPILER_GCC__)
#   define FORCE_INLINE [[gnu::always_inline]] inline
#   define NO_INLINE    [[gnu::noinline]]
#   define PACKED       [[gnu::packed]]
#   define ATTRIBUTE(x) [[gnu::x]]
#else
#   error "Unsupported compiler"
#endif

#if defined(_MSC_VER)
#   define restrict __restrict
#elif defined(__GNUC__) || defined(__clang__)
#   define restrict __restrict__
#else
#   define restrict
#endif

#endif