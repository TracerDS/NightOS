#include <terminal.hpp>
#include <klibc/cstring>
#include <klibc/cctype>

#include <cstdint>
#include <utility>
#include <limits>

namespace NOS::Terminal {
    Terminal g_terminal{};

    Terminal::Terminal() noexcept :
        m_buffer(reinterpret_cast<std::uint16_t*>(0xB8000)),
        m_width(80),
        m_height(25)
    {
        for (auto y = 0; y < m_height; ++y) {
            for (auto x = 0; x < m_width; ++x) {
                write_at(' ', x, y);
            }
        }
    }
    
    void Terminal::scroll() noexcept {
        for (std::uint16_t y = 1; y < m_height; ++y) {
            for (std::uint16_t x = 0; x < m_width; ++x) {
                m_buffer[(y - 1) * m_width + x] = m_buffer[y * m_width + x];
            }
        }

        clear_row(m_height - 1);
    }
    void Terminal::clear_row(std::uint16_t row) noexcept {
        for (std::uint16_t x = 0; x < m_width; ++x) {
            write_at(' ', x, row);
        }
    }


    void Terminal::write_at (
        char c,
        std::uint16_t x,
        std::uint16_t y,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        if (klibc::iscntrl(c)) {
            return;
        }

        // Validate coordinates to prevent buffer overflow
        if (x >= m_width || y >= m_height) {
            return;
        }

        const std::uint16_t index = y * m_width + x;

        auto fg = std::to_underlying(foreground);
        auto bg = std::to_underlying(background);
        std::uint8_t color = (bg << 4) | fg;

        std::uint16_t out = (color << 8) | c;
        m_buffer[index] = out;
    }
    
    void Terminal::write_char (
        char c,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        write_at(c, m_cursorX, m_cursorY, foreground, background);

        if (c == '\n') {
            ++m_cursorY;
        } else if (c == '\r') {
            m_cursorX = 0;
        } else if (c == '\b') {
            if (m_cursorX > 0) {
                --m_cursorX;
            }
            write_at(' ', m_cursorX, m_cursorY, foreground, background);
        } else {
            ++m_cursorX;
        }

        if (m_cursorX >= m_width) {
            m_cursorX = 0;
            ++m_cursorY;
        }

        if (m_cursorY >= m_height) {
            scroll();
            m_cursorY = m_height - 1;
        }
    }

    void Terminal::write_string(
        const char* const string,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        std::size_t stringLen = klibc::strlen(string);
        for (std::size_t i = 0; i < stringLen; ++i) {
            write_char(string[i], foreground, background);
        }
    }

    void Terminal::write_number (
        std::int64_t value,
        std::uint8_t base,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        using value_type = std::decay_t<decltype(value)>;

        // Default to base 10 if invalid
        if (base != 2 && base != 8 && base != 10 && base != 16) {
            base = 10; // Default to base 10 if invalid
        }

        if (value < 0) {
            write_char('-', foreground, background);
            if (value == std::numeric_limits<std::int64_t>::min()) {
                // Handle the case of INT64_MIN separately
                write_number(std::numeric_limits<std::int64_t>::max() / base, base, foreground, background);
                write_number(std::numeric_limits<std::int64_t>::max() % base, base, foreground, background);
                return;
            }
            value = -value;
        }
        write_number(std::make_unsigned_t<value_type>(value), base, foreground, background);
    }

    void Terminal::write_number (
        std::uint64_t value,
        std::uint8_t base,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        if (value == 0) {
            write_char('0', foreground, background);
            return;
        }
        
        // Default to base 10 if invalid
        if (base != 2 && base != 8 && base != 10 && base != 16) {
            base = 10;
        }

        if (base == 16) {
            write_hex(value, false, foreground, background);
            return;
        }

        std::uint64_t divisor = 1;
        // Check for overflow before multiplying
        while (value / divisor >= base && divisor <= UINT64_MAX / base) {
            divisor *= base;
        }

        while (divisor > 0) {
            std::uint8_t digit = value / divisor;
            write_char(digit < 10 ? '0' + digit : 'A' + digit - 10, foreground, background);
            value %= divisor;
            divisor /= base;
        }
    }

    void Terminal::write_float (
        float value,
        int precision,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        Terminal::write_float(static_cast<double>(value), precision, foreground, background);
    }

    void Terminal::write_float (
        double value,
        int precision,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        using value_type = std::decay_t<decltype(value)>;

        using limits = std::numeric_limits<value_type>;
        constexpr auto EPSILON = limits::epsilon();

        if (value == limits::quiet_NaN()) { // Check for NaN
            write_string("NaN", foreground, background);
            return;
        }

        if (value == -limits::infinity()) { // Check for -infinity
            write_string("-inf", foreground, background);
            return;
        }

        if (value == limits::infinity()) { // Check for +infinity
            write_string("inf", foreground, background);
            return;
        }

        if (value < 0) {
            write_char('-', foreground, background);
            value = -value;
        }

        std::uint64_t integerPart = static_cast<std::uint64_t>(value);
        write_number(integerPart, 10, foreground, background);

        double fraction = value - integerPart;
        if (fraction < EPSILON || precision <= 0) {
            return;
        }

        write_char('.', foreground, background);

        // Multiply and round to the nearest integer for the required precision
        double rounding = 0.5;
        for (auto i = 0; i < precision; ++i) {
            rounding /= 10.0;
        }
        fraction += rounding;

        for (auto i = 0; i < precision; ++i) {
            fraction *= 10.0;
            auto digit = static_cast<std::uint8_t>(fraction);
            write_char('0' + digit, foreground, background);
            fraction -= digit;
        }
    }

    void Terminal::write_hex (
        std::uint64_t value,
        bool uppercase,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        if (value == 0) {
            write_char('0', foreground, background);
            return;
        }

        char buf[16]; // max 16 hex digits for 64-bit
        int idx = 0;

        while (value != 0) {
            std::uint8_t digit = value & 0xF;
            buf[idx++] = digit < 10 ? '0' + digit : (uppercase ? 'A' : 'a') + digit - 10;
            value >>= 4;
        }

        // print digits in reverse
        for (auto i = idx - 1; i >= 0; --i) {
            write_char(buf[i], foreground, background);
        }
    }
}