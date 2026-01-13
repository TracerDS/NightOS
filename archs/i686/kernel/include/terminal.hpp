#pragma once

#include <init.hpp>
#include <klibc/cstring>

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <concepts>

namespace NOS::Terminal {
    enum class VGAColor : std::uint8_t {
        VGA_COLOR_BLACK = 0,
        VGA_COLOR_BLUE = 1,
        VGA_COLOR_GREEN = 2,
        VGA_COLOR_CYAN = 3,
        VGA_COLOR_RED = 4,
        VGA_COLOR_MAGENTA = 5,
        VGA_COLOR_BROWN = 6,
        VGA_COLOR_LIGHT_GRAY = 7,
        VGA_COLOR_DARK_GRAY = 8,
        VGA_COLOR_LIGHT_BLUE = 9,
        VGA_COLOR_LIGHT_GREEN = 10,
        VGA_COLOR_LIGHT_CYAN = 11,
        VGA_COLOR_LIGHT_RED = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN = 14,
        VGA_COLOR_WHITE = 15
    };

    class Terminal {
        std::uint16_t* const m_buffer;
        const std::uint16_t m_width;
        const std::uint16_t m_height;

        std::uint16_t m_cursorX;
        std::uint16_t m_cursorY;

        void scroll() noexcept;
        void clear_row(std::uint16_t row) noexcept;
    public:
        Terminal() noexcept;

        void write_at (
            char c,
            std::uint16_t x,
            std::uint16_t y,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        void write_char (
            char c,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        void write_string (
            const char* const string,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;
        
        void write_number (
            std::int64_t value,
            std::uint8_t base = 10,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        void write_number (
            std::uint64_t value,
            std::uint8_t base = 10,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        void write_float (
            float value,
            int precision = -1,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;
        
        void write_float (
            double value,
            int precision = -1,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;
        
        void write_hex (
            std::uint64_t value,
            bool uppercase = false,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;
    };
    
    extern Terminal g_terminal;
}