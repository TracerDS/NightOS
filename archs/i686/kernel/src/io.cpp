#include <io.hpp>
#include <cstdarg>
#include <bit>
#include <limits>
#include <cstddef>
#include <concepts>

namespace IO {
    namespace detail {
        constexpr std::uint16_t numDigits(std::integral auto number) noexcept
        {
            std::uint16_t digits = 0;
            while (number) {
                number /= 10;
                digits++;
            }
            return digits;
        }
        constexpr std::uint16_t numDigits(std::floating_point auto number) noexcept
        {
            auto decimal = static_cast<std::uint64_t>(number);
            std::uint16_t digits = numDigits(decimal);

            number -= decimal;

            while (number >= std::numeric_limits<double>::epsilon()) {
                number *= 10.0f;
                auto digit = static_cast<std::uint64_t>(number);
                number -= digit;
                ++digits;
            }
            return digits;
        }
    }

    std::size_t __ksprintf__core__ (
        char* buffer,
        const char* format,
        va_list args
    ) noexcept {
        bool readOnly = !buffer;
        std::size_t chars = 0;

        while (*format) {
            if (*format != '%') {
                ++chars;

                if (!readOnly) {
                    *buffer++ = *format++;
                }
                continue;
            }
            ++format;

            if (*format == '\0')
                break;

            // %%
            if (*format == '%') {
                ++chars;
                if (!readOnly) {
                    *buffer++ = '%';
                }
                ++format;
                continue;
            }

            bool leftAlign = false;
            bool plusSign = false;
            bool spaceSign = false;
            bool prefix = false;
            bool zeroPad = false;

            // Check flags
            while (*format == '-' || *format == '+' || *format == ' ' || *format == '#' || *format == '0') {
                ++chars;
                if (!readOnly) {
                    *buffer++ = *format;
                }
                switch (*format++) {
                    case '-': leftAlign = true; break;
                    case '+': plusSign  = true; break;
                    case ' ': spaceSign = true; break;
                    case '#': prefix    = true; break;
                    case '0': zeroPad   = true; break;
                }
            }

            int width;

            // Check width
            if (*format >= '0' && *format <= '9') {
                while (*format >= '0' && *format <= '9') {
                    ++chars;
                    width = width * 10 + (*format++ - '0');
                }
            } else if (*format == '*') {
                ++chars;
                width = va_arg(args, int);
                if (width < 0) {
                    width = -width;
                    leftAlign = true;
                }
                ++format;
            }
        }
        return chars;
    }

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

            if (*format == '\0')
                return;
            
            if (*format == '%') {
                Terminal::g_Terminal.WriteChar('%', foreground, background);
                ++format;
                continue;
            }

            bool plusSign = false;
            bool spaceSign = false;
            bool leftAlign = false;
            bool zeroPad = false;
            bool prefix = false;

            // Flags
            while (*format == '-' || *format == '+' || *format == ' ' || *format == '#' || *format == '0') {
                switch (*format++) {
                    case '-': leftAlign = true; break;
                    case '+': plusSign = true; break;
                    case ' ': spaceSign = true; break;
                    case '#': prefix = true; break;
                    case '0': zeroPad = true; break;
                }
            }

            // Width
            int width = 0;
            if (*format >= '0' && *format <= '9') {
                while (*format >= '0' && *format <= '9')
                    width = width * 10 + (*format++ - '0');
            } else if (*format == '*') {
                width = va_arg(args, int);
                ++format;
            }

            // Precision
            int precision = -1;
            if (*format == '.') {
                ++format;
                precision = 0;
                if (*format == '*') {
                    precision = va_arg(args, int);
                    ++format;
                } else {
                    while (*format >= '0' && *format <= '9')
                        precision = precision * 10 + (*format++ - '0');
                }
            }

            // Length modifiers
            enum {
                LEN_NONE,
                LEN_H, LEN_HH,
                LEN_L, LEN_LL,
                LEN_J,
                LEN_Z,
                LEN_T, 
                LEN_BIG_L,
            } length = LEN_NONE;
            if (*format == 'h') {
                ++format;
                if (*format == 'h') {
                    ++format;
                    length = LEN_HH;
                } else {
                    length = LEN_H;
                }
            } else if (*format == 'l') {
                ++format;
                if (*format == 'l') {
                    ++format;
                    length = LEN_LL;
                } else {
                    length = LEN_L;
                }
            } else if (*format == 'j') {
                ++format;
                length = LEN_J;
            } else if (*format == 'z') {
                ++format;
                length = LEN_Z;
            } else if (*format == 't') {
                ++format;
                length = LEN_T;
            } else if (*format == 'L') {
                ++format;
                length = LEN_BIG_L;
            }

            switch (*format) {
                case 'c': {
                    char c = static_cast<char>(va_arg(args, int));
                    if (!leftAlign && width > 1) {
                        for (int i = 1; i < width; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }
                    Terminal::g_Terminal.WriteChar(c, foreground, background);
                    if (leftAlign && width > 1) {
                        for (int i = 1; i < width; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    if (!str) str = "(null)";

                    int len = 0;
                    while (str[len] && (precision < 0 || len < precision)) ++len;
                    if (!leftAlign && width > len) {
                        for (int i = 0; i < width - len; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }

                    for (int i = 0; i < len; ++i) {
                        Terminal::g_Terminal.WriteChar(str[i], foreground, background);
                    }

                    if (leftAlign && width > len) {
                        for (int i = 0; i < width - len; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }

                    break;
                }
                case 'd':
                case 'i': {
                    std::int64_t num;
                    switch (length) {
                        case LEN_H:  num = static_cast<std::int16_t>(va_arg(args, int)); break;
                        case LEN_HH: num = static_cast<std::int8_t>(va_arg(args, int)); break;
                        case LEN_L:  num = va_arg(args, long); break;
                        case LEN_LL: num = va_arg(args, long long); break;
                        case LEN_Z:  num = static_cast<std::int64_t>(va_arg(args, std::size_t)); break;
                        case LEN_T:  num = static_cast<std::int64_t>(va_arg(args, std::ptrdiff_t)); break;
                        default:     num = va_arg(args, int);
                    }
                    auto digitsCount = detail::numDigits(num);

                    if (!leftAlign && width > 1) {
                        for (int i = 0; i < width - digitsCount; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }

                    Terminal::g_Terminal.WriteNumber(
                        num,
                        10,
                        foreground,
                        background
                    );

                    if (leftAlign && width > 1) {
                        for (int i = 0; i < width - digitsCount; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }
                    break;
                }
                case 'o': {
                    std::uint64_t num;
                    std::uint8_t numWidth;
                    switch (length) {
                        case LEN_HH: {
                            num = static_cast<std::uint8_t>(va_arg(args, int));
                            numWidth = sizeof(std::uint8_t);
                            break;
                        }
                        case LEN_H: {
                            num = static_cast<std::uint16_t>(va_arg(args, int));
                            numWidth = sizeof(std::uint16_t);
                            break;
                        }
                        case LEN_L: {
                            num = va_arg(args, std::uint32_t);
                            numWidth = sizeof(std::uint32_t);
                            break;
                        }
                        case LEN_LL: {
                            num = va_arg(args, std::uint64_t);
                            numWidth = sizeof(std::uint64_t);
                            break;
                        }
                        case LEN_J: {
                            num = va_arg(args, std::uintmax_t);
                            numWidth = sizeof(std::uintmax_t);
                            break;
                        }
                        case LEN_Z: {
                            num = va_arg(args, std::size_t);
                            numWidth = sizeof(std::size_t);
                            break;
                        }
                        case LEN_T: {
                            num = va_arg(args, unsigned std::ptrdiff_t);
                            numWidth = sizeof(unsigned std::ptrdiff_t);
                            break;
                        }
                        default: {
                            num = va_arg(args, std::uint32_t);
                            numWidth = sizeof(std::uint32_t);
                            break;
                        }
                    }
                    numWidth *= 3; // Each octal digit takes 3 characters
                    
                    if (!leftAlign && width > 1) {
                        for (int i = 0; i < width - numWidth; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }

                    Terminal::g_Terminal.WriteNumber(num, 8, foreground, background);
                    
                    if (leftAlign && width > 1) {
                        for (int i = 0; i < width - numWidth; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }
                    break;
                }
                case 'x': {
                    std::uint64_t num;
                    std::uint8_t numWidth;
                    switch (length) {
                        case LEN_HH: {
                            num = static_cast<std::uint8_t>(va_arg(args, int));
                            numWidth = sizeof(std::uint8_t);
                            break;
                        }
                        case LEN_H: {
                            num = static_cast<std::uint16_t>(va_arg(args, int));
                            numWidth = sizeof(std::uint16_t);
                            break;
                        }
                        case LEN_L: {
                            num = va_arg(args, std::uint32_t);
                            numWidth = sizeof(std::uint32_t);
                            break;
                        }
                        case LEN_LL: {
                            num = va_arg(args, std::uint64_t);
                            numWidth = sizeof(std::uint64_t);
                            break;
                        }
                        case LEN_J: {
                            num = va_arg(args, std::uintmax_t);
                            numWidth = sizeof(std::uintmax_t);
                            break;
                        }
                        case LEN_Z: {
                            num = va_arg(args, std::size_t);
                            numWidth = sizeof(std::size_t);
                            break;
                        }
                        case LEN_T: {
                            num = va_arg(args, unsigned std::ptrdiff_t);
                            numWidth = sizeof(unsigned std::ptrdiff_t);
                            break;
                        }
                        default: {
                            num = va_arg(args, std::uint32_t);
                            numWidth = sizeof(std::uint32_t);
                            break;
                        }
                    }
                    numWidth *= 2; // Each hex digit takes 2 characters
                    if (!leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }

                    Terminal::g_Terminal.WriteHex(num, false, foreground, background);
                    
                    if (leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }
                    break;
                }
                case 'X': {
                    std::uint64_t num;
                    std::uint8_t numWidth;
                    switch (length) {
                        case LEN_HH: {
                            num = static_cast<std::uint8_t>(va_arg(args, int));
                            numWidth = sizeof(std::uint8_t);
                            break;
                        }
                        case LEN_H: {
                            num = static_cast<std::uint16_t>(va_arg(args, int));
                            numWidth = sizeof(std::uint16_t);
                            break;
                        }
                        case LEN_L: {
                            num = va_arg(args, std::uint32_t);
                            numWidth = sizeof(std::uint32_t);
                            break;
                        }
                        case LEN_LL: {
                            num = va_arg(args, std::uint64_t);
                            numWidth = sizeof(std::uint64_t);
                            break;
                        }
                        case LEN_J: {
                            num = va_arg(args, std::uintmax_t);
                            numWidth = sizeof(std::uintmax_t);
                            break;
                        }
                        case LEN_Z: {
                            num = va_arg(args, std::size_t);
                            numWidth = sizeof(std::size_t);
                            break;
                        }
                        case LEN_T: {
                            num = va_arg(args, unsigned std::ptrdiff_t);
                            numWidth = sizeof(unsigned std::ptrdiff_t);
                            break;
                        }
                        default: {
                            num = va_arg(args, std::uint32_t);
                            numWidth = sizeof(std::uint32_t);
                            break;
                        }
                    }
                    numWidth *= 2; // Each hex digit takes 2 characters
                    if (!leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }

                    Terminal::g_Terminal.WriteHex(num, true, foreground, background);
                    
                    if (leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }
                    break;
                }
                case 'u': {
                    std::uint64_t num;
                    switch (length) {
                        case LEN_HH: {
                            num = static_cast<std::uint8_t>(va_arg(args, int));
                            break;
                        }
                        case LEN_H: {
                            num = static_cast<std::uint16_t>(va_arg(args, int));
                            break;
                        }
                        case LEN_L: {
                            num = va_arg(args, unsigned long);
                            break;
                        }
                        case LEN_LL: {
                            num = va_arg(args, unsigned long long);
                            break;
                        }
                        case LEN_J: {
                            num = va_arg(args, std::uintmax_t);
                            break;
                        }
                        case LEN_Z: {
                            num = va_arg(args, std::size_t);
                            break;
                        }
                        case LEN_T: {
                            num = va_arg(args, unsigned std::ptrdiff_t);
                            break;
                        }
                        default: {
                            num = va_arg(args, std::uint32_t);
                            break;
                        }
                    }
                    auto numWidth = detail::numDigits(num);
                    if (!leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }

                    Terminal::g_Terminal.WriteNumber(num, 10, foreground, background);
                    
                    if (leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }
                    break;
                }
                case 'f': {
                    double num;
                    switch (length) {
                        case LEN_L:  num = va_arg(args, double); break;
                        case LEN_LL: num = va_arg(args, long double); break;
                        default:     num = static_cast<float>(va_arg(args, double));
                    }

                    if (plusSign) {
                        Terminal::g_Terminal.WriteChar('+', foreground, background);
                    } else if (spaceSign) {
                        Terminal::g_Terminal.WriteChar(' ', foreground, background);
                    }

                    if (precision < 0) precision = 6;

                    Terminal::g_Terminal.WriteFloat(num, precision, foreground, background);
                    break;
                }
                case 'p': {
                    auto data = reinterpret_cast<uintptr_t>(va_arg(args, void*));
                    
                    if (!leftAlign && width > 1) {
                        for (int i = 1; i < width; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }

                    Terminal::g_Terminal.WriteString("0x", foreground, background);
                    Terminal::g_Terminal.WriteHex(data, false, foreground, background);

                    if (leftAlign && width > 1) {
                        for (int i = 1; i < width; ++i) {
                            Terminal::g_Terminal.WriteChar(' ', foreground, background);
                        }
                    }
                    break;
                }
            }

                
            /*
            if (current == 'd') {
            } else if (current == 'u') {
                std::uint32_t num = va_arg(args, std::uint32_t);
                Terminal::g_Terminal.WriteNumber(static_cast<std::uint64_t>(num), 10, foreground, background);
            } else if (current == 'x') {
            } else if (current == 'X') {
                std::uint32_t num = va_arg(args, std::uint32_t);
                Terminal::g_Terminal.WriteHex(num, true, foreground, background);
            } else if (current == 'p') {
                void* ptr = va_arg(args, void*);
                Terminal::g_Terminal.WriteHex(
                    reinterpret_cast<std::uintptr_t>(ptr),
                    false, foreground, background
                );
            } else if (current == 'b') {
                std::uint32_t num = va_arg(args, std::uint32_t);
                Terminal::g_Terminal.WriteNumber(num, 2, foreground, background);
            } else if (current == 'o') {
                std::uint32_t num = va_arg(args, std::uint32_t);
                Terminal::g_Terminal.WriteNumber(num, 8, foreground, background);
            } else if (current == '%') {
                Terminal::g_Terminal.WriteChar('%', foreground, background);
            } else {
                Terminal::g_Terminal.WriteChar('%', foreground, background);
                Terminal::g_Terminal.WriteChar(*format, foreground, background);
            }
            */
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