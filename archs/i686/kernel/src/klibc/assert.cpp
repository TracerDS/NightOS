#include <klibc/cassert>
#include <io.hpp>

void __kassert(const char* msg, const char* file, int line) {
#ifdef __KERNEL_DEBUG__
    IO::kprintf_color(
        "%s:%d - Assertion failed: %s\r\n",
        Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
        Terminal::Terminal::VGAColor::VGA_COLOR_BLACK,
        file,
        line,
        msg
    );
    
    __kernel_halt__();
#endif
}