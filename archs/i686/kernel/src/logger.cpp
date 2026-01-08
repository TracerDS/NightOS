#include <logger.hpp>

#include <serial.hpp>
#include <io.hpp>
#include <klibc/cstring>
#include <cstdarg>

namespace IO {
    extern void __kprintf_core__(
        const char* format,
        va_list args,
        Terminal::Terminal::VGAColor foreground = Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_GRAY,
        Terminal::Terminal::VGAColor background = Terminal::Terminal::VGAColor::VGA_COLOR_BLACK
    ) noexcept;
}

namespace NOS::Logger {
    void Log(const char* fmt, ...) noexcept {
        va_list args;
        va_start(args, fmt);
        
        // Use kprintf to format the message
        IO::__kprintf_core__(fmt, args);
        
        va_end(args);
    }

    void Log(const char* message) noexcept {
        Serial::g_serial.write_string(Serial::Serial::COM1, message);
        Serial::g_serial.write_string(Serial::Serial::COM1, "\r\n");
        
        IO::kprintf("%s", message); // Also print to the console
    }
    
    void LogError(const char* fmt, ...) noexcept {
        va_list args;
        va_start(args, fmt);
        
        // Use kprintf to format the message
        IO::__kprintf_core__(fmt, args, Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        
        va_end(args);
    }

    void LogError(const char* message) noexcept {
        Serial::g_serial.write_string(Serial::Serial::COM2, message);
        Serial::g_serial.write_string(Serial::Serial::COM2, "\r\n");
        
        // Also print to the console
        IO::kprintf_color(
            "%s",
            Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
            Terminal::Terminal::VGAColor::VGA_COLOR_BLACK,
            message
        );
    }
}