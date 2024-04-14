#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#include <vga.h>

#ifdef __cplusplus
extern "C" {
#endif

const auto VGA_ADDR = 0xB8000;

size_t TERMINAL_MAX_X = 80;
size_t TERMINAL_MAX_Y = 25;

struct {
    size_t x;
    size_t y;
	uint16_t* buffer;
	uint8_t color;
} TerminalState;

void ktermPrintChar(char c, uint8_t color = TerminalState.color) {
	if(TerminalState.x >= TERMINAL_MAX_X) {
		TerminalState.x = 0;
		TerminalState.y++;
	}
	if(TerminalState.y >= TERMINAL_MAX_Y) {
		TerminalState.y = 0;
	}

	if(c == '\r') {
		TerminalState.x = 0;
		return;
	}
	if(c == '\n') {
		TerminalState.y++;
		return;
	}

	auto index = TerminalState.y * TERMINAL_MAX_X + TerminalState.x;

	TerminalState.buffer[index] = vga_entry(c, color);
	TerminalState.x++;
}
void ktermPrint(const char* str, uint8_t color = TerminalState.color) {
	for(auto i = 0; str[i];i++)
		ktermPrintChar(str[i], color);
}

void kprintf(const char* string, ...) {
	(void*)(string);
	/*
	va_list args;
    va_start(args, string);
 
    while (*string) {
        if (*string == 'd') {
            int i = va_arg(args, int);
            //printf("%d\n", i);
        } else if (*string == 'c') {
            // A 'char' variable will be promoted to 'int'
            // A character literal in C is already 'int' by itself
            int c = va_arg(args, int);
            //printf("%c\n", c);
        } else if (*string == 'f') {
            double d = va_arg(args, double);
            //printf("%f\n", d);
        }
        ++string;
    }
 
    va_end(args);
	*/
}

#include <assembly.h>

void nos_kmain() {
	TerminalState.x = 0;
	TerminalState.y = 0;
	TerminalState.buffer = (uint16_t*) VGA_ADDR;
	TerminalState.color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

	int a = 0;
	__asm__ __volatile__("mov %%eax, %0" : "+r"(&a));
	__asm__ __volatile__("mov %0, %%eax" : "r"(2137));
	__asm__ __volatile__("mov %%eax, %0" : "+r"(&a));

	ktermPrint("Hello, World!\nabc");
}

#ifdef __cplusplus
}
#endif