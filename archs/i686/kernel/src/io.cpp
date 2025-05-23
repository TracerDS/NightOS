#include <io.hpp>
#include <cstdarg>

namespace IO {
    void __kprintf_core__ (
        const char* format,
        va_list args,
        Terminal::Terminal::VGAColor foreground,
        Terminal::Terminal::VGAColor background
    ) noexcept {
        while (*format) {
            if (*format != '%') {
                Terminal::g_Terminal.WriteChar(*format++, foreground, background);
                continue;
            }
            ++format;
            if (*format == '\0') {
                break;
            }

            switch (*format) {
                case 'c': {
                    char c = static_cast<char>(va_arg(args, int));
                    Terminal::g_Terminal.WriteChar(c, foreground, background);
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    if (!str) {
                        str = "(null)";
                    }
                    Terminal::g_Terminal.WriteString(str, foreground, background);
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    Terminal::g_Terminal.WriteNumber(static_cast<std::int64_t>(num), 10, foreground, background);
                    break;
                }
                case 'u': {
                    std::uint32_t num = va_arg(args, std::uint32_t);
                    Terminal::g_Terminal.WriteNumber(static_cast<std::uint64_t>(num), 10, foreground, background);
                    break;
                }
                case 'x': {
                    std::uint32_t num = va_arg(args, std::uint32_t);
                    Terminal::g_Terminal.WriteHex(num, false, false, foreground, background);
                    break;
                }
                case 'X': {
                    std::uint32_t num = va_arg(args, std::uint32_t);
                    Terminal::g_Terminal.WriteHex(num, true, false, foreground, background);
                    break;
                }
                case 'p': {
                    void* ptr = va_arg(args, void*);
                    Terminal::g_Terminal.WriteHex (
                        reinterpret_cast<std::uintptr_t>(ptr),
                        false, false, foreground, background
                    );
                    break;
                }
                case 'b': {
                    std::uint32_t num = va_arg(args, std::uint32_t);
                    Terminal::g_Terminal.WriteNumber(num, 2, foreground, background);
                    break;
                }
                case 'o': {
                    std::uint32_t num = va_arg(args, std::uint32_t);
                    Terminal::g_Terminal.WriteNumber(num, 8, foreground, background);
                    break;
                }
                case '%': {
                    Terminal::g_Terminal.WriteChar('%', foreground, background);
                    break;
                }
                default: {
                    Terminal::g_Terminal.WriteChar('%', foreground, background);
                    Terminal::g_Terminal.WriteChar(*format, foreground, background);
                    break;
                }
            }
            ++format;
        }
    }

    void kprintf_color (
        const char* format,
        Terminal::Terminal::VGAColor foreground,
        Terminal::Terminal::VGAColor background,
        ...
    ) noexcept {
        va_list args;
        va_start(args, background);

        __kprintf_core__(
            format,
            args,
            foreground,
            background
        );
        
        va_end(args);
    }

    void kprintf(const char* format, ...) noexcept {
        va_list args;
        va_start(args, format);

        __kprintf_core__(
            format,
            args,
            Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_GRAY,
            Terminal::Terminal::VGAColor::VGA_COLOR_BLACK
        );
        
        va_end(args);
    }
}