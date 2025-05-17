#pragma once

#include <init.hpp>
#include <cstddef>
#include <type_traits>
#include <concepts>
#include <klibc/string.h>

namespace Terminal {
    void Initialize() noexcept;

    void WriteString(const char* const string) noexcept;
    void WriteNumber(const std::integral auto value) noexcept;
    void WriteChar(char c) noexcept;

    void Write(const std::integral auto value) noexcept;

    enum class VGAColor {
        VGA_COLOR_BLACK = 0,
        VGA_COLOR_BLUE = 1,
        VGA_COLOR_GREEN = 2,
        VGA_COLOR_CYAN = 3,
        VGA_COLOR_RED = 4,
        VGA_COLOR_MAGENTA = 5,
        VGA_COLOR_BROWN = 6,
        VGA_COLOR_LIGHT_GREY = 7,
        VGA_COLOR_DARK_GREY = 8,
        VGA_COLOR_LIGHT_BLUE = 9,
        VGA_COLOR_LIGHT_GREEN = 10,
        VGA_COLOR_LIGHT_CYAN = 11,
        VGA_COLOR_LIGHT_RED = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN = 14,
        VGA_COLOR_WHITE = 15,
    };
    
    uint8_t VGAEntry(VGAColor foreground, VGAColor background) noexcept;
    uint16_t VGAEntry(uint8_t character, uint8_t color) noexcept;
    uint16_t VGAEntry(uint8_t character, VGAColor foreground, VGAColor background) noexcept;

    void SetColor(VGAColor foreground, VGAColor background);

    void terminal_writehex(uint64_t value);

    void terminal_writenumber(int64_t value);

    void terminal_write(const char* data, size_t size);

    void terminal_writestring(const char* data);
}