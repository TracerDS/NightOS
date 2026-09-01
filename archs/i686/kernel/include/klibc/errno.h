#ifndef __KLIBC_ERRNO_H__
#define __KLIBC_ERRNO_H__

#include <stddef.h>

using errno_t = int;
using rsize_t = size_t;

extern thread_local errno_t __kerrno__;

#define RSIZE_MAX static_cast<rsize_t>(-1)
#define errno __kerrno__

#define E2BIG            1 // Argument list too long
#define EACCES           2 // Permission denied
#define EADDRINUSE       3 // Address in use
#define EADDRNOTAVAIL    4 // Address not available
#define EAFNOSUPPORT     5 // Address family not supported
#define EAGAIN           6 // Resource unavailable, try again
#define EALREADY         7 // Connection already in progress
#define EBADF            8 // Bad file descriptor
#define EBADMSG          9 // Bad message
#define EBUSY           10 // Device or resource busy
#define ECANCELED       11 // Operation canceled
#define ECHILD          12 // No child processes
#define ECONNABORTED    13 // Connection aborted
#define ECONNREFUSED    14 // Connection refused
#define ECONNRESET      15 // Connection reset
#define EDEADLK         16 // Resource deadlock would occur
#define EDESTADDRREQ    17 // Destination address required
#define EDOM            18 // Mathematics argument out of domain of function
#define EEXIST          19 // File exists
#define EFAULT          20 // Bad address
#define EFBIG           21 // File too large
#define EHOSTUNREACH    22 // Host is unreachable
#define EIDRM           23 // Identifier removed
#define EILSEQ          24 // Illegal byte sequence
#define EINPROGRESS     25 // Operation in progress
#define EINTR           26 // Interrupted function
#define EINVAL          27 // Invalid argument
#define EIO             28 // I/O error
#define EISCONN         29 // Socket is connected
#define EISDIR          30 // Is a directory
#define ELOOP           31 // Too many levels of symbolic links
#define EMFILE          32 // File descriptor value too large
#define EMLINK          33 // Too many links
#define EMSGSIZE        34 // Message too large
#define ENAMETOOLONG    35 // Filename too long
#define ENETDOWN        36 // Network is down
#define ENETRESET       37 // Connection aborted by network
#define ENETUNREACH     38 // Network unreachable
#define ENFILE          39 // Too many files open in system
#define ENOBUFS         40 // No buffer space available
#define ENODATA         41 // No message is available on the STREAM head read queue
#define ENODEV          42 // No such device
#define ENOENT          43 // No such file or directory
#define ENOEXEC         44 // Executable file format error
#define ENOLCK          45 // No locks available
#define ENOLINK         46 // Link has been severed
#define ENOMEM          47 // Not enough space
#define ENOMSG          48 // No message of the desired type
#define ENOPROTOOPT     49 // Protocol not available
#define ENOSPC          50 // No space left on device
#define ENOSR           51 // No STREAM resources
#define ENOSTR          52 // Not a STREAM
#define ENOSYS          53 // Function not supported
#define ENOTCONN        54 // The socket is not connected
#define ENOTDIR         55 // Not a directory
#define ENOTEMPTY       56 // Directory not empty
#define ENOTRECOVERABLE 57 // State not recoverable
#define ENOTSOCK        58 // Not a socket
#define ENOTSUP         59 // Not supported
#define ENOTTY          60 // Inappropriate I/O control operation
#define ENXIO           61 // No such device or address
#define EOPNOTSUPP      62 // Operation not supported on socket
#define EOVERFLOW       63 // Value too large to be stored in data type
#define EOWNERDEAD      64 // Previous owner died
#define EPERM           65 // Operation not permitted
#define EPIPE           66 // Broken pipe
#define EPROTO          67 // Protocol error
#define EPROTONOSUPPORT 68 // Protocol not supported
#define EPROTOTYPE      69 // Protocol wrong type for socket
#define ERANGE          70 // Result too large
#define EROFS           71 // Read-only file system
#define ESPIPE          72 // Invalid seek
#define ESRCH           73 // No such process
#define ETIME           74 // Stream ioctl() timeout
#define ETIMEDOUT       75 // Connection timed out
#define ETXTBSY         76 // Text file busy
#define EWOULDBLOCK     77 // Operation would block
#define EXDEV           78 // Cross-device link

#endif 