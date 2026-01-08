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

namespace Log {
    void Logger::log(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        
        // Use kprintf to format the message
        IO::__kprintf_core__(fmt, args);
        
        va_end(args);
    }

    void Logger::log(const char* message) {
        Serial::g_serial.write_string(Serial::Serial::COM1, message);
        Serial::g_serial.write_string(Serial::Serial::COM1, "\r\n");
        
        IO::kprintf("%s", message); // Also print to the console
    }
}