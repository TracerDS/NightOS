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
        auto msgLen = klibc::strlen(message);
        for (std::size_t i = 0; i < msgLen; ++i) {
            Serial::WriteByte(Serial::Ports::COM1, message[i]); // 0x3F8 is the default COM1 port
        }
        Serial::WriteByte(Serial::Ports::COM1, '\r'); // Carriage return for proper formatting
        Serial::WriteByte(Serial::Ports::COM1, '\n'); // Add a newline at the end
        
        IO::kprintf("%s", message); // Also print to the console
    }
}