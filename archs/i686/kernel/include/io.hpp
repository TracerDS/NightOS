#pragma once

#include <terminal.hpp>
#include <cstdarg>

namespace NOS::IO {
    ATTRIBUTE(format(printf, 1, 2))
    void kprintf(const char* format, ...) noexcept;
    
    ATTRIBUTE(format(printf, 1, 4))
    void kprintf_color (
        const char* format,
        Terminal::VGAColor foreground = Terminal::VGAColor::VGA_COLOR_LIGHT_GRAY,
        Terminal::VGAColor background = Terminal::VGAColor::VGA_COLOR_BLACK,
        ...
    ) noexcept;

    void kvprintf(const char* format, va_list args) noexcept;

    void kvprintf_color (
        const char* format,
        va_list args,
        Terminal::VGAColor foreground = Terminal::VGAColor::VGA_COLOR_LIGHT_GRAY,
        Terminal::VGAColor background = Terminal::VGAColor::VGA_COLOR_BLACK
    ) noexcept;
}