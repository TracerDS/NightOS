#pragma once

#include <terminal.hpp>
#include <cstdarg>

namespace IO {
    void kprintf(const char* format, ...) noexcept __attribute__((format(printf, 1, 2)));
    void kprintf_color (
        const char* format,
        Terminal::Terminal::VGAColor foreground = Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_GRAY,
        Terminal::Terminal::VGAColor background = Terminal::Terminal::VGAColor::VGA_COLOR_BLACK,
        ...
    ) noexcept __attribute__((format(printf, 1, 4)));
}