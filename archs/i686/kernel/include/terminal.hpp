#pragma once

#include <init.hpp>
#include <klibc/cstring>

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <concepts>

namespace Terminal {
    class Terminal {
        std::uint16_t* const m_buffer;
        const std::uint16_t m_width;
        const std::uint16_t m_height;

        std::uint16_t m_cursorX;
        std::uint16_t m_cursorY;

        void Scroll() noexcept;
        void ClearRow(std::uint16_t row) noexcept;
    public:
        Terminal() noexcept;
        
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

        void WriteAt (
            char c,
            std::uint16_t x,
            std::uint16_t y,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        void WriteChar (
            char c,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        void WriteString (
            const char* const string,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        template <std::integral T>
        void WriteNumber (
            T value,
            std::uint8_t base = 10,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept {
            using ValueType = decltype(value);
            if constexpr (std::is_unsigned_v<ValueType>) {
                WriteNumber(static_cast<std::uint64_t>(value), base, foreground, background);
            } else {
                WriteNumber(static_cast<std::int64_t>(value), base, foreground, background);
            }
        }
        
        void WriteNumber (
            std::uint64_t value,
            std::uint8_t base = 10,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        void WriteNumber (
            std::int64_t value,
            std::uint8_t base = 10,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        void WriteFloat (
            float value,
            int precision = -1,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept {
            WriteFloat(static_cast<double>(value), precision, foreground, background);
        }

        void WriteFloat (
            double value,
            int precision = -1,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;
        
        void WriteHex (
            std::uint64_t value,
            bool uppercase = false,
            VGAColor foreground = VGAColor::VGA_COLOR_LIGHT_GRAY,
            VGAColor background = VGAColor::VGA_COLOR_BLACK
        ) noexcept;

        static Terminal& GetInstance() noexcept {
            static Terminal term;
            return term;
        }
    };
    
    extern Terminal g_Terminal;
}