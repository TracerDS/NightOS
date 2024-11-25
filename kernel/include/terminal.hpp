#pragma once

#include <stdint.h>
#include <stddef.h>

namespace Terminal {
    /* Hardware text mode color constants. */
    enum vga_color {
        VGA_COLOR_BLACK = 0,
        VGA_COLOR_BLUE = 1,
        VGA_COLOR_GREEN = 2,
        VGA_COLOR_CYAN = 3,
        VGA_COLOR_RED = 4,
        VGA_COLOR_MAGENTA = 5,
        VGA_COLOR_BROWN = 6,
        VGA_COLOR_LIGHT_GREY = 7,
        VGA_COLOR_DARK_GREY = 8,
        VGA_COLOR_LIGHT_BLUE = 9,
        VGA_COLOR_LIGHT_GREEN = 10,
        VGA_COLOR_LIGHT_CYAN = 11,
        VGA_COLOR_LIGHT_RED = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN = 14,
        VGA_COLOR_WHITE = 15,
    };

    uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
    uint16_t vga_entry(unsigned char uc, uint8_t color);
    
    void terminal_initialize();
    void terminal_setcolor(uint8_t color);
    void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
    void terminal_putchar(char c);
    void terminal_write(const char* data, size_t size);

    void terminal_writestring(const char* data);

    void terminal_writenumber(uint64_t number);
    void terminal_writenumber(uint32_t number);
    void terminal_writenumber(uint16_t number);
    void terminal_writenumber(uint8_t number);
    
    void terminal_writenumber(int64_t number);
    void terminal_writenumber(int32_t number);
    void terminal_writenumber(int16_t number);
    void terminal_writenumber(int8_t number);

    inline void terminal_writenumber(int number) { terminal_writenumber((int32_t) number); }
    inline void terminal_writenumber(unsigned int number) { terminal_writenumber((uint32_t) number); }

    void terminal_writehex(uint64_t number);
    void terminal_writefloat(float number);
    
    template <typename T>
    inline void terminal_write(T) {}

    template <> inline void terminal_write(uint64_t value) { terminal_writenumber(value); }
    template <> inline void terminal_write(uint32_t value) { terminal_writenumber(value); }
    template <> inline void terminal_write(uint16_t value) { terminal_writenumber(value); }
    template <> inline void terminal_write(uint8_t value)  { terminal_writenumber(value); }

    template <> inline void terminal_write(int64_t value) { terminal_writenumber(value); }
    template <> inline void terminal_write(int32_t value) { terminal_writenumber(value); }
    template <> inline void terminal_write(int16_t value) { terminal_writenumber(value); }
    template <> inline void terminal_write(int8_t value)  { terminal_writenumber(value); }

    template <> inline void terminal_write(float value)  { terminal_writefloat(value); }

    template <> inline void terminal_write(const char* value) { terminal_writestring(value); }
    template <> inline void terminal_write(const char value)  { terminal_putchar(value); }
}