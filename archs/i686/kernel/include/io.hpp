#pragma once

#include <terminal.hpp>

namespace IO {
    void kprintf(const char* format, ...) noexcept;
    void kprintf_color (
        const char* format,
        Terminal::Terminal::VGAColor foreground = Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_GRAY,
        Terminal::Terminal::VGAColor background = Terminal::Terminal::VGAColor::VGA_COLOR_BLACK,
        ...
    ) noexcept;
}