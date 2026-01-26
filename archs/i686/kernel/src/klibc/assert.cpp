#include <klibc/cassert>
#include <io.hpp>
#include <init.hpp>

void __kassert(const char* msg, const char* file, int line, const char* function) {
#ifdef __NOS_KERNEL_DEBUG__
    NOS::IO::kprintf_color(
        "Assertion failed: %s\r\n"
        "- File: %s\r\n"
        "- Line: %d\r\n"
        "- Function: %s\r\n",
        NOS::Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
        NOS::Terminal::VGAColor::VGA_COLOR_BLACK,
        msg,
        file,
        line,
        function
    );
    
    asm volatile("cli");
    asm volatile("hlt");
#endif
}