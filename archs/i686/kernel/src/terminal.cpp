#include <terminal.hpp>
#include <klibc/string.h>
#include <klibc/cctype>
#include <cstdint>
#include <utility>
#include <limits>

namespace Terminal {
    Terminal::Terminal() noexcept
    : m_buffer(reinterpret_cast<std::uint16_t*>(0xB8000)), m_width(80), m_height(25) {
        for (auto y = 0; y < m_height; ++y) {
            for (auto x = 0; x < m_width; ++x) {
                WriteAt(' ', x, y);
            }
        }
    }
    
    void Terminal::Scroll() noexcept {
        for (std::uint16_t y = 1; y < m_height; ++y) {
            for (std::uint16_t x = 0; x < m_width; ++x) {
                m_buffer[(y - 1) * m_width + x] = m_buffer[y * m_width + x];
            }
        }

        ClearRow(m_height - 1);
    }
    void Terminal::ClearRow(std::uint16_t row) noexcept {
        for (std::uint16_t x = 0; x < m_width; ++x) {
            WriteAt(' ', x, row);
        }
    }


    void Terminal::WriteAt (
        char c,
        std::uint16_t x,
        std::uint16_t y,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        if (std::iscntrl(c)) {
            return;
        }

        const std::uint16_t index = y * m_width + x;

        auto fg = std::to_underlying<Terminal::VGAColor>(foreground);
        auto bg = std::to_underlying<Terminal::VGAColor>(background);
        std::uint8_t color = (bg << 4) | fg;

        std::uint16_t out = (color << 8) | c;
        m_buffer[index] = out;
    }
    
    void Terminal::WriteChar (
        char c,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        WriteAt(c, m_cursorX, m_cursorY, foreground, background);
        ++m_cursorX;

        if (c == '\n') {
            ++m_cursorY;
        } else if (c == '\r') {
            m_cursorX = 0;
        }

        if (m_cursorX >= m_width) {
            m_cursorX = 0;
            ++m_cursorY;
        }

        if (m_cursorY >= m_height) {
            Scroll();
            m_cursorY = m_height - 1;
        }
    }
    
    void Terminal::WriteString(
        const char* const string,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        std::size_t stringLen = strlen(string);
        for (std::size_t i = 0; i < stringLen; ++i) {
            WriteChar(string[i], foreground, background);
        }
    }

    void Terminal::WriteNumber (
        std::uint64_t value,
        std::uint8_t base,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        if (value == 0) {
            WriteChar('0', foreground, background);
            return;
        }
        
        if (base != 2 && base != 8 && base != 10 && base != 16) {
            base = 10; // Default to base 10 if invalid
        }

        if (base == 16) {
            WriteHex(value, false, false, foreground, background);
            return;
        }

        std::uint64_t divisor = 1;
        while (value / divisor >= base) {
            divisor *= base;
        }

        while (divisor > 0) {
            std::uint8_t digit = value / divisor;
            WriteChar(digit < 10 ? '0' + digit : 'A' + digit - 10, foreground, background);
            value %= divisor;
            divisor /= base;
        }
    }

    void Terminal::WriteNumber (
        std::int64_t value,
        std::uint8_t base,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        if (value < 0) {
            WriteChar('-', foreground, background);
            if (value == std::numeric_limits<std::int64_t>::min()) {
                // Handle the case of INT64_MIN separately
                WriteNumber(std::numeric_limits<std::int64_t>::max() / base, base, foreground, background);
                WriteNumber(std::numeric_limits<std::int64_t>::max() % base, base, foreground, background);
                return;
            }
            value = -value;
        }
        WriteNumber(static_cast<std::uint64_t>(value), base, foreground, background);
    }

    void Terminal::WriteHex (
        std::uint64_t value,
        bool uppercase,
        bool prefix,
        VGAColor foreground,
        VGAColor background
    ) noexcept {
        if (prefix) {
            WriteString("0x", foreground, background);
        }
        
        if (value == 0) {
            WriteChar('0', foreground, background);
            return;
        }

        std::uint64_t divisor = 1;
        while (value / divisor >= 16) {
            divisor *= 16;
        }

        while (divisor > 0) {
            std::uint8_t digit = value / divisor;
            WriteChar(digit < 10 ? '0' + digit : (uppercase ? 'A' : 'a') + digit - 10, foreground, background);
            value %= divisor;
            divisor /= 16;
        }
    }
}