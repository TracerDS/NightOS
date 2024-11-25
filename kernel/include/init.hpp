#pragma once

#ifdef __cplusplus
#   define __CPP_FUNC__ extern "C"
#   define __CPP_START__ __CPP_FUNC__ {
#   define __CPP_END__ }
#else
#   define __CPP_FUNC__
#   define __CPP_START__
#   define __CPP_END__
#endif

#ifdef _DEBUG
#   ifndef __KERNEL_DEBUG
#      define __KERNEL_DEBUG
#   endif
#endif