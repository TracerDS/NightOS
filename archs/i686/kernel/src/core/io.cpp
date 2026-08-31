#include <io.hpp>
#include <terminal.hpp>

#include <cstdarg>
#include <bit>
#include <limits>
#include <climits>
#include <cstddef>
#include <concepts>

namespace NOS::IO {
    namespace detail {
        constexpr std::uint16_t numDigits(std::integral auto number, std::uint8_t base = 10) noexcept
        {
            std::uint16_t digits = 0;
            while (number) {
                number /= base;
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

    void kprintf(const char* format, ...) noexcept {        
        va_list args;
        va_start(args, format);

        kvprintf(format, args);
        
        va_end(args);
    }

    void kprintf_color (
        const char* format,
        Terminal::VGAColor foreground,
        Terminal::VGAColor background,
        ...
    ) noexcept {
        va_list args;
        va_start(args, background);

        kvprintf_color(
            format,
            args,
            foreground,
            background
        );
        
        va_end(args);
    }
    
    void kvprintf(const char* format, va_list args) noexcept {
        kvprintf_color(
            format,
            args,
            Terminal::VGAColor::VGA_COLOR_LIGHT_GRAY,
            Terminal::VGAColor::VGA_COLOR_BLACK
        );
    }

    void kvprintf_color (
        const char* format,
        va_list args,
        Terminal::VGAColor foreground,
        Terminal::VGAColor background
    ) noexcept {
        while (*format) {
            if (*format != '%') {
                Terminal::g_terminal.write_char(*format++, foreground, background);
                continue;
            }
            ++format;

            if (*format == '\0')
                return;
            
            if (*format == '%') {
                Terminal::g_terminal.write_char('%', foreground, background);
                ++format;
                continue;
            }

            bool plusSign = false;
            bool spaceSign = false;
            bool leftAlign = false;
            bool zeroPad = false;
            [[maybe_unused]] bool prefix = false;

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
                while (*format >= '0' && *format <= '9') {
                    int digit = *format++ - '0';
                    // Check for integer overflow
                    if (width > (INT_MAX - digit) / 10) {
                        width = INT_MAX;
                        break;
                    }
                    width = width * 10 + digit;
                }
            } else if (*format == '*') {
                width = va_arg(args, int);
                // Clamp negative or excessive widths
                if (width < 0) width = 0;
                if (width > 1024) width = 1024;
                ++format;
            }

            // Precision
            int precision = -1;
            if (*format == '.') {
                ++format;
                precision = 0;
                if (*format == '*') {
                    precision = va_arg(args, int);
                    // Clamp negative or excessive precision
                    if (precision < 0) precision = -1;
                    if (precision > 512) precision = 512;
                    ++format;
                } else {
                    while (*format >= '0' && *format <= '9') {
                        int digit = *format++ - '0';
                        // Check for integer overflow
                        if (precision > (INT_MAX - digit) / 10) {
                            precision = 512;
                            break;
                        }
                        precision = precision * 10 + digit;
                    }
                    if (precision > 512) precision = 512;
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
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }
                    Terminal::g_terminal.write_char(c, foreground, background);
                    if (leftAlign && width > 1) {
                        for (int i = 1; i < width; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    if (!str) str = "(null)";

                    int len = 0;
                    // Add reasonable maximum length to prevent infinite loops
                    int max_len = (precision >= 0 && precision < 1024) ? precision : 1024;
                    while (str[len] && len < max_len) ++len;
                    if (!leftAlign && width > len) {
                        for (int i = 0; i < width - len; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }

                    for (int i = 0; i < len; ++i) {
                        Terminal::g_terminal.write_char(str[i], foreground, background);
                    }

                    if (leftAlign && width > len) {
                        for (int i = 0; i < width - len; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
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
                        case LEN_J:  num = va_arg(args, std::intmax_t); break;
                        case LEN_Z:  num = static_cast<std::int64_t>(va_arg(args, std::size_t)); break;
                        case LEN_T:  num = static_cast<std::int64_t>(va_arg(args, std::ptrdiff_t)); break;
                        default:     num = va_arg(args, int);
                    }
                    auto digitsCount = detail::numDigits(num);

                    if (!leftAlign && width > 1) {
                        for (int i = 0; i < width - digitsCount; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }

                    Terminal::g_terminal.write_number(
                        num,
                        10,
                        foreground,
                        background
                    );

                    if (leftAlign && width > 1) {
                        for (int i = 0; i < width - digitsCount; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }
                    break;
                }
                case 'o': {
                    std::uint64_t num;
                    std::uint8_t numWidth;
                    switch (length) {
                        case LEN_HH: {
                            num = static_cast<std::uint8_t>(va_arg(args, unsigned int));
                            numWidth = sizeof(std::uint8_t);
                            break;
                        }
                        case LEN_H: {
                            num = static_cast<std::uint16_t>(va_arg(args, unsigned int));
                            numWidth = sizeof(std::uint16_t);
                            break;
                        }
                        case LEN_L: {
                            num = va_arg(args, unsigned long);
                            numWidth = sizeof(unsigned long);
                            break;
                        }
                        case LEN_LL: {
                            num = va_arg(args, unsigned long long);
                            numWidth = sizeof(unsigned long long);
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
                            num = va_arg(args, std::size_t);
                            numWidth = sizeof(std::size_t);
                            break;
                        }
                        default: {
                            num = va_arg(args, unsigned int);
                            numWidth = sizeof(unsigned int);
                            break;
                        }
                    }
                    numWidth *= 3; // Each octal digit takes 3 characters
                    
                    if (!leftAlign && width > 1) {
                        for (int i = 0; i < width - numWidth; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }

                    auto digits = detail::numDigits(num, 8);

                    if (zeroPad && width > 0) {
                        for (auto i = 0; i < width - digits; ++i) {
                            Terminal::g_terminal.write_char('0', foreground, background);
                        }
                    }

                    Terminal::g_terminal.write_number(num, 8, foreground, background);
                    
                    if (leftAlign && width > 1) {
                        for (int i = 0; i < width - numWidth; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }
                    break;
                }
                case 'x':
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
                            num = va_arg(args, unsigned long);
                            numWidth = sizeof(unsigned long);
                            break;
                        }
                        case LEN_LL: {
                            num = va_arg(args, unsigned long long);
                            numWidth = sizeof(unsigned long long);
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
                            num = va_arg(args, std::size_t);
                            numWidth = sizeof(std::size_t);
                            break;
                        }
                        default: {
                            num = va_arg(args, unsigned int);
                            numWidth = sizeof(unsigned int);
                            break;
                        }
                    }
                    numWidth *= 2; // Each hex digit takes 2 characters
                    if (!leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }

                    auto digits = detail::numDigits(num, 16);

                    if (zeroPad && width > 0) {
                        if (width < digits) {
                            width = digits;
                        }
                        if (digits == 0) {
                            --width;
                        }
                        for (auto i = 0; i < width - digits; ++i) {
                            Terminal::g_terminal.write_char('0', foreground, background);
                        }
                    }
                    Terminal::g_terminal.write_hex(num, (*format) == 'X', foreground, background);
                    
                    if (leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
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
                            num = va_arg(args, std::size_t);
                            break;
                        }
                        default: {
                            num = va_arg(args, unsigned int);
                            break;
                        }
                    }
                    auto numWidth = detail::numDigits(num);
                    if (!leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }

                    Terminal::g_terminal.write_number(num, 10, foreground, background);
                    
                    if (leftAlign && width > 1) {
                        for (auto i = 0; i < width - numWidth; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
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
                        Terminal::g_terminal.write_char('+', foreground, background);
                    } else if (spaceSign) {
                        Terminal::g_terminal.write_char(' ', foreground, background);
                    }

                    if (precision < 0) precision = 6;

                    Terminal::g_terminal.write_float(num, precision, foreground, background);
                    break;
                }
                case 'p': {
                    auto data = reinterpret_cast<std::uintptr_t>(va_arg(args, void*));
                    
                    if (!leftAlign && width > 1) {
                        for (int i = 1; i < width; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }

                    Terminal::g_terminal.write_string("0x", foreground, background);
                    Terminal::g_terminal.write_hex(data, false, foreground, background);

                    if (leftAlign && width > 1) {
                        for (int i = 1; i < width; ++i) {
                            Terminal::g_terminal.write_char(' ', foreground, background);
                        }
                    }
                    break;
                }
            }

            ++format;
        }
    }
}