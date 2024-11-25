#include <terminal.hpp>
#include <string.h>

namespace Terminal {
    const size_t VGA_WIDTH = 80;
    const size_t VGA_HEIGHT = 25;

    size_t terminal_row;
    size_t terminal_column;
    uint8_t terminal_color;
    uint16_t* terminal_buffer;

    uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
    {
        return fg | bg << 4;
    }

    uint16_t vga_entry(unsigned char uc, uint8_t color) 
    {
        return (uint16_t) uc | (uint16_t) color << 8;
    }

    void terminal_initialize() 
    {
        if(terminal_buffer) {
            return;
        }
        
        terminal_row = 0;
        terminal_column = 0;
        terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        terminal_buffer = (uint16_t*) 0xB8000;
        for (size_t y = 0; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                const size_t index = y * VGA_WIDTH + x;
                terminal_buffer[index] = vga_entry(' ', terminal_color);
            }
        }
    }

    void terminal_setcolor(uint8_t color) 
    {
        terminal_color = color;
    }

    void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
    {
        const size_t index = y * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(c, color);
    }

    void terminal_putchar(char c)
    {
        if (c == '\n') {
            terminal_row++;
            return;
        }
        if (c == '\r') {
            terminal_column = 0;
            return;
        }
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0;
        }
    }

    void terminal_write(const char* data, size_t size)
    {
        for (size_t i = 0; i < size; i++)
            terminal_putchar(data[i]);
    }

    void terminal_writestring(const char* data) 
    {
        terminal_write(data, strlen(data));
    }

    void terminal_writenumber(int64_t number) {
        if (number < 0) {
            terminal_putchar('-');
            number = -number;
        }

        while(number > 0) {
            terminal_putchar('0' + (number % 10));
            number /= 10;
        }
    }
    void terminal_writenumber(int32_t number) {
        return terminal_writenumber((int64_t) number);
    }
    void terminal_writenumber(int16_t number) {
        return terminal_writenumber((int64_t) number);
    }
    void terminal_writenumber(int8_t number) {
        return terminal_writenumber((int64_t) number);
    }

    void terminal_writenumber(uint64_t number) {
        while(number > 0) {
            terminal_putchar('0' + (number % 10));
            number /= 10;
        }
    }
    void terminal_writenumber(uint32_t number) {
        return terminal_writenumber((uint64_t) number);
    }
    void terminal_writenumber(uint16_t number) {
        return terminal_writenumber((uint64_t) number);
    }
    void terminal_writenumber(uint8_t number) {
        return terminal_writenumber((uint64_t) number);
    }

    void terminal_writehex(uint64_t number) {
        static constexpr auto digits = "0123456789ABCDEF";

        terminal_writestring("0x");

        do {
            terminal_putchar(digits[number % 16]);
            number /= 16;
        }while(number > 0);
    }

    void terminal_writefloat(float number) {
        if (number == 0) {
            terminal_putchar('0');
            return;
        }
        if (number < 0) {
            terminal_putchar('-');
            number = -number;
        }

        // Write integer part
        auto intPart = static_cast<int>(number);
        terminal_writenumber(intPart);

        // Calculate the fraction part
        auto fraction = number - intPart;
        if (fraction == 0) return; // No fractional part

        terminal_putchar('.');
        uint8_t digits = 0;
        static constexpr auto MAX_PRECISION_DIGITS = 6;
        char buffer[MAX_PRECISION_DIGITS + 1]{0}; // Buffer to store fractional digits

        while (fraction > 0 && digits < MAX_PRECISION_DIGITS) {
            fraction *= 10;
            int digit = static_cast<int>(fraction);
            buffer[digits++] = '0' + digit;
            fraction -= digit;
        }

        // Remove trailing zeroes
        while (digits > 0 && buffer[digits - 1] == '0') {
            digits--;
        }

        // Write the fractional part
        for (int i = 0; i < digits; i++) {
            terminal_putchar(buffer[i]);
        }
    }
}