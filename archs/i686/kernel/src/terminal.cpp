#include <terminal.hpp>
#include <cstdint>
#include <klibc/string.h>

namespace Terminal {
    struct {
        const std::size_t VGA_WIDTH = 80;
        const std::size_t VGA_HEIGHT = 50;

        std::size_t row;
        std::size_t column;
        std::uint8_t color;

        uint16_t* const buffer = (uint16_t*) 0xB8000;
    } g_TerminalState;

    void Initialize() noexcept {
        g_TerminalState.row = 0;
        g_TerminalState.column = 0;
        g_TerminalState.color = VGAEntry(VGAColor::VGA_COLOR_LIGHT_GREY, VGAColor::VGA_COLOR_BLACK);

        for (std::size_t y = 0; y < g_TerminalState.VGA_HEIGHT; y++) {
            for (std::size_t x = 0; x < g_TerminalState.VGA_WIDTH; x++) {
                auto index = y * g_TerminalState.VGA_WIDTH + x;
                g_TerminalState.buffer[index] = VGAEntry(' ', g_TerminalState.color);
            }
        }
    }

    void WriteAt(char c, uint8_t color, std::size_t x, std::size_t y) noexcept {
        const size_t index = y * g_TerminalState.VGA_WIDTH + x;
        g_TerminalState.buffer[index] = VGAEntry(c, color);
    }
    void WriteAt(char c, VGAColor foreground, VGAColor background, std::size_t x, std::size_t y) noexcept {
        return WriteAt(c, VGAEntry(foreground, background), x, y);
    }
    
    void WriteChar(char c) noexcept {
        WriteAt(c, g_TerminalState.color, g_TerminalState.column, g_TerminalState.row);
        if (++g_TerminalState.column == g_TerminalState.VGA_WIDTH) {
            g_TerminalState.column = 0;
            if (++g_TerminalState.row == g_TerminalState.VGA_HEIGHT)
                g_TerminalState.row = 0;
        }
    }

    void WriteString(const char* const string) noexcept {
        for (size_t i = 0; i< strlen(string); i++) {
            WriteChar(string[i]);
        }
    }

    void WriteNumber(const std::integral auto value) noexcept {

    }
    
    void Write(const std::integral auto value) noexcept { WriteNumber(value); }

    void terminal_writehex(uint64_t value) 
    {
        int index = 15;
        char buffer[16]{0}; // Enough for 64-bit hex representation
        for(auto i=0;i<16;i++)
        {
            buffer[i] = '0';
        }
        while (value > 0) {
            uint8_t digit = value & 0xF;
            if (digit < 10) {
                buffer[index--] = '0' + digit;
            } else {
                buffer[index--] = 'A' + digit - 10;
            }
            value >>= 4;
        }
        for(auto i=0;i<16;i++)
        {
            WriteChar(buffer[i]);
        }
    }

    void terminal_writenumber(int64_t value) 
    {
        if (value < 0) {
            WriteChar('-');
            value = -value;
        }
        if (value == 0) {
            WriteChar('0');
            return;
        }
        char buffer[20]; // Enough for 64-bit integer
        int index = 0;
        while (value > 0) {
            buffer[index++] = '0' + (value % 10);
            value /= 10;
        }
        for (int i = index - 1; i >= 0; i--) {
            WriteChar(buffer[i]);
        }
    }

    void terminal_write(const char* data, size_t size) 
    {
        for (size_t i = 0; i < size; i++)
        WriteChar(data[i]);
    }
    void terminal_writestring(const char* data) 
    {
        terminal_write(data, strlen(data));
    }
    
    uint8_t VGAEntry(VGAColor foreground, VGAColor background) noexcept {
        return static_cast<uint8_t>(foreground) | (static_cast<uint8_t>(background) << 4);
    }
    uint16_t VGAEntry(uint8_t character, uint8_t color) noexcept {
        return (uint16_t) character | (uint16_t) color << 8;
    }
    uint16_t VGAEntry(uint8_t character, VGAColor foreground, VGAColor background) noexcept {
        return VGAEntry(character, VGAEntry(foreground, background));
    }

    void SetColor(VGAColor foreground, VGAColor background) 
    {
        g_TerminalState.color = VGAEntry(foreground, background);
    }

    
}