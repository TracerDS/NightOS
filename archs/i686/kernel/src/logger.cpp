#include <logger.hpp>

#include <interfaces/streams/serial.hpp>
#include <io.hpp>
#include <klibc/cstring>
#include <cstdarg>

namespace NOS::Logger {
    void Log(const char* fmt, ...) noexcept {
        va_list args;
        va_start(args, fmt);
        
        // Use kprintf to format the message
        IO::kvprintf(fmt, args);
        
        va_end(args);
    }

    void Log(const char* message) noexcept {
        Serial::g_serial.write_string(Serial::COM1, message);
        Serial::g_serial.write_string(Serial::COM1, "\r\n");
        
        IO::kprintf("%s", message); // Also print to the console
    }
    
    void LogError(const char* fmt, ...) noexcept {
        va_list args;
        va_start(args, fmt);
        
        // Use kprintf to format the message
        IO::kvprintf_color(
            fmt,
            args,
            Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
            Terminal::VGAColor::VGA_COLOR_BLACK
        );
        
        va_end(args);
    }

    void LogError(const char* message) noexcept {
        Serial::g_serial.write_string(Serial::COM2, message);
        Serial::g_serial.write_string(Serial::COM2, "\r\n");
        
        // Also print to the console
        IO::kprintf_color(
            "%s",
            Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
            Terminal::VGAColor::VGA_COLOR_BLACK,
            message
        );
    }
}