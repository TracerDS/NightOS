#include <klibc/cstdio>
#include <klibc/errno.h>
#include <core/io.hpp>

#include <cstddef>
#include <cstdarg>

extern "C" {
    FILE* stdin = nullptr;
    FILE* stdout = nullptr;
    FILE* stderr = nullptr;
}

struct __FILE {
    enum class stream_kind {
        memory,
        console
    };

    stream_kind kind = stream_kind::memory;
    char* buffer = nullptr;
    size_t size = 0;
    size_t capacity = 0;
    size_t position = 0;
    int handle = -1;
    bool owns_buffer = false;
    bool eof = false;
    bool error = false;
};

struct __fpos_t {

};

namespace {
    constexpr size_t __stdio_buffer_growth = 256;

    [[maybe_unused]]
    bool __is_readable_mode(const char* mode) {
        if (mode == nullptr) {
            return false;
        }

        switch (mode[0]) {
            case 'r':
                return true;
            case 'a':
            case 'w':
                return false;
            default:
                return mode[1] == '+' || mode[2] == '+';
        }
    }

    [[maybe_unused]]
    bool __is_writeable_mode(const char* mode) {
        if (mode == nullptr) {
            return false;
        }

        switch (mode[0]) {
            case 'w':
            case 'a':
                return true;
            case 'r':
                return mode[1] == '+' || mode[2] == '+';
            default:
                return true;
        }
    }

    FILE* __make_console_stream(int handle) {
        auto* file = new __FILE{};
        file->kind = __FILE::stream_kind::console;
        file->handle = handle;
        return file;
    }

    FILE* __make_memory_stream(const char* mode) {
        auto* file = new __FILE{};
        file->kind = __FILE::stream_kind::memory;
        file->buffer = nullptr;
        file->capacity = 0;
        file->size = 0;
        file->position = 0;
        file->owns_buffer = true;

        if (__is_writeable_mode(mode) == true) {
            file->buffer = new char[__stdio_buffer_growth]{};
            file->capacity = __stdio_buffer_growth;
        }

        return file;
    }

    void __ensure_stdio() {
        if (!stdin) {
            stdin = __make_console_stream(0);
        }
        if (!stdout) {
            stdout = __make_console_stream(1);
        }
        if (!stderr) {
            stderr = __make_console_stream(2);
        }
    }

    bool __is_console_name(const char* filename) {
        if (filename == nullptr) {
            return true;
        }

        return filename[0] == '\0' ||
               filename[0] == '-' ||
               filename[0] == 'C' ||
               filename[0] == 'c';
    }

    void __append_bytes(FILE* stream, const void* data, size_t length) {
        if (stream == nullptr || data == nullptr || length == 0) {
            return;
        }

        if (stream->kind != __FILE::stream_kind::memory) {
            return;
        }

        if (stream->position + length >= stream->capacity) {
            size_t new_capacity = stream->capacity == 0 ? __stdio_buffer_growth : stream->capacity;
            while (stream->position + length >= new_capacity) {
                new_capacity *= 2;
            }

            char* new_buffer = new char[new_capacity]{};
            for (size_t i = 0; i < stream->size; ++i) {
                new_buffer[i] = stream->buffer[i];
            }

            if (stream->buffer != nullptr) {
                delete[] stream->buffer;
            }

            stream->buffer = new_buffer;
            stream->capacity = new_capacity;
        }

        const auto* bytes = static_cast<const char*>(data);
        for (size_t i = 0; i < length; ++i) {
            stream->buffer[stream->position + i] = bytes[i];
        }

        stream->position += length;
        stream->size = stream->position;
    }

    size_t __read_bytes(FILE* stream, void* data, size_t length) {
        if (stream == nullptr || data == nullptr || length == 0 || stream->kind != __FILE::stream_kind::memory) {
            return 0;
        }

        size_t available = stream->size > stream->position ? stream->size - stream->position : 0;
        size_t count = available < length ? available : length;

        auto* out = static_cast<char*>(data);
        for (size_t i = 0; i < count; ++i) {
            out[i] = stream->buffer[stream->position + i];
        }

        stream->position += count;
        if (stream->position >= stream->size) {
            stream->eof = true;
        }

        return count;
    }
}

FILE* fopen(const char* restrict filename, const char* restrict mode) {
    if (filename == nullptr || mode == nullptr) {
        errno = EINVAL;
        return nullptr;
    }

    __ensure_stdio();

    if (__is_console_name(filename)) {
        if (filename[0] == '\0' || filename[0] == '-') {
            return stdout;
        }
        if (filename[0] == 'C' || filename[0] == 'c') {
            return stderr;
        }
    }

    return __make_memory_stream(mode);
}

errno_t fopen_s(
    FILE* restrict* restrict streamptr,
    const char* restrict filename,
    const char* restrict mode
) {
    if (!streamptr || !filename || !mode) {
        return EINVAL;
    }
    *streamptr = fopen(filename, mode);
    return 0;
}

int fclose(FILE* stream) {
    if (stream == nullptr) {
        return EOF;
    }

    if (stream->kind == __FILE::stream_kind::memory && stream->owns_buffer && stream->buffer != nullptr) {
        delete[] stream->buffer;
    }

    delete stream;
    return 0;
}

int fflush(FILE* stream) {
    if (stream == nullptr) {
        return EOF;
    }

    if (stream->kind == __FILE::stream_kind::console) {
        return 0;
    }

    if (stream->kind == __FILE::stream_kind::memory) {
        stream->error = false;
        stream->eof = false;
        return 0;
    }

    return EOF;
}

void setbuf(FILE* restrict stream, char* restrict buffer) {
    if (buffer)
        setvbuf(stream, buffer, _IOFBF, BUFSIZ);
    else
        setvbuf(stream, nullptr, _IOFBF, 0);
}

int setvbuf(FILE* restrict stream, char* restrict buffer, int mode, size_t size) {
    return 0;
}


int fputc(int ch, FILE* stream) {
    if (stream == nullptr || ch == EOF) {
        return EOF;
    }

    if (stream->kind == __FILE::stream_kind::console) {
        char value = static_cast<char>(ch);
        NOS::IO::kprintf("%c", value);
        return ch;
    }

    char value = static_cast<char>(ch);
    __append_bytes(stream, &value, 1);
    return ch;
}

int fputs(const char* str, FILE* stream) {
    if (str == nullptr || stream == nullptr) {
        return EOF;
    }

    if (stream->kind == __FILE::stream_kind::console) {
        NOS::IO::kprintf("%s", str);
        return 0;
    }

    size_t length = 0;
    while (str[length] != '\0') {
        ++length;
    }

    __append_bytes(stream, str, length);
    return 0;
}

int puts(const char* str) {
    if (str == nullptr) {
        return EOF;
    }

    if (fputs(str, stdout) != 0) {
        return EOF;
    }

    return fputc('\n', stdout) == EOF ? EOF : 0;
}

size_t fread(void* ptr, size_t size, size_t count, FILE* stream) {
    if (ptr == nullptr || stream == nullptr || size == 0 || count == 0) {
        return 0;
    }

    if (stream->kind == __FILE::stream_kind::console) {
        return 0;
    }

    size_t total = size * count;
    return __read_bytes(stream, ptr, total) / size;
}

size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream) {
    if (ptr == nullptr || stream == nullptr || size == 0 || count == 0) {
        return 0;
    }

    if (stream->kind == __FILE::stream_kind::console) {
        const auto* bytes = static_cast<const char*>(ptr);
        for (size_t i = 0; i < count * size; ++i) {
            if (fputc(static_cast<unsigned char>(bytes[i]), stream) == EOF) {
                return i / size;
            }
        }
        return count;
    }

    __append_bytes(stream, ptr, size * count);
    return count;
}

int fprintf(FILE* stream, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const int result = vfprintf(stream, format, args);
    va_end(args);
    return result;
}

int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    const int result = vprintf(format, args);
    va_end(args);
    return result;
}

int vfprintf(FILE* stream, const char* format, va_list args) {
    if (format == nullptr || stream == nullptr) {
        return EOF;
    }

    if (stream->kind == __FILE::stream_kind::console) {
        NOS::IO::kvprintf(format, args);
        return 0;
    }

    char buffer[256];
    size_t written = 0;
    for (size_t i = 0; format[i] != '\0' && i < sizeof(buffer) - 1; ++i) {
        buffer[i] = format[i];
        written = i + 1;
    }
    buffer[written] = '\0';
    __append_bytes(stream, buffer, written);
    return static_cast<int>(written);
}

int vprintf(const char* format, va_list args) {
    return vfprintf(stdout, format, args);
}

void perror([[maybe_unused]] const char* string) {
    // fputs(string, stderr);
    // fputs(": ", stderr);
    // fputs(strerror(errno), stderr);
    // fputc('\n', stderr);
}