#include <klibc/cassert>
#include <io.hpp>

extern "C" void __kernel_halt__();
extern "C" void __kernel_clear_interrupts__();

void __kassert(const char* msg, const char* file, int line, const char* function) {
#ifdef __KERNEL_DEBUG__
    IO::kprintf_color(
        "Assertion failed: %s\r\n"
        "- File: %s\r\n"
        "- Line: %d\r\n"
        "- Function: %s\r\n",
        Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
        Terminal::Terminal::VGAColor::VGA_COLOR_BLACK,
        msg,
        file,
        line,
        function
    );
    
    while (true) {
        __kernel_clear_interrupts__();
        __kernel_halt__();
    }
#endif
}