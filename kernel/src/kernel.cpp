#include <cpp.hpp>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <gdt.hpp>

__CPP_START__

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

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize() 
{
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
	if(c == '\n') {
		terminal_row++;
		return;
	}
	if(c == '\r') {
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

void terminal_writenumber(uint64_t number) {
	static constexpr auto digits = "0123456789ABCDEF";

	terminal_writestring("0x");

	do {
		terminal_putchar(digits[number % 16]);
		number /= 16;
	}while(number > 0);
}

__CPP_END__

template <typename T>
void terminal_write(T) {}

template <> void terminal_write(uint64_t value) { terminal_writenumber(value); }
template <> void terminal_write(uint32_t value) { terminal_writenumber(value); }
template <> void terminal_write(uint16_t value) { terminal_writenumber(value); }
template <> void terminal_write(uint8_t value)  { terminal_writenumber(value); }

template <> void terminal_write(const char* value) { terminal_writestring(value); }
template <> void terminal_write(const char value)  { terminal_putchar(value); }

__CPP_START__

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

void kernel_main() 
{
	terminal_initialize();
	terminal_write(STR(__cplusplus));
	terminal_write("\r\n");

	const GDT::GDT_Descriptor* descriptor = GDT::GDT_GetDescriptor();
	GDT::GDT_Entry* entries = (GDT::GDT_Entry*)descriptor->entry;

	if(!descriptor->entry) {
		terminal_write("GDT Entries are null\r\n");
		return;
	}
	
	auto entries_count = (descriptor->limit + 1) / sizeof(GDT::GDT_Entry);
	terminal_write("GDT Entries: ");
	terminal_write(entries_count);
	terminal_write("\r\n");

	for(uint16_t i = 0; i < entries_count; i++) {
		GDT::GDT_Entry entry = entries[i];
		terminal_write("GDT Entry ");
		terminal_putchar(i + '0');
		terminal_write(": [");
		terminal_write(*reinterpret_cast<uint64_t*>(&entry));
		terminal_write("]\r\n");
	}

	if(true) {
		terminal_write("GDT status: ");
		bool status = GDT::GDT_Initialize();
		terminal_write(status ? "true" : "false");
		terminal_write("\r\n");
	}
}

__CPP_END__