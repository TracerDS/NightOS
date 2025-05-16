#include <init.hpp>
#include <cstddef>
#include <cstdint>
#include <gdt.hpp>
#include <terminal.hpp>
#include <crt.hpp>

#include <grub/multiboot.hpp>

__CPP_START__

#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0x00FFFFFF
#define COLOR_RED       0x00FF0000
#define COLOR_GREEN     0x0000FF00
#define COLOR_BLUE      0x000000FF

void draw_pixel(uint32_t *framebuffer, uint32_t width, uint32_t height, 
                uint32_t pitch, int x, int y, uint32_t color) {
    if (x >= width || y >= height) return;
    
    // Calculate pixel position
    // Note: pitch is in bytes, we need to convert to pixels (divide by 4 for 32bpp)
    uint32_t *pixel = (uint32_t*)((uint8_t*)framebuffer + y * pitch + x * 4);
    *pixel = color;
}

void draw_hline(uint32_t *framebuffer, uint32_t width, uint32_t height, 
                uint32_t pitch, int x, int y, int len, uint32_t color) {
    for (int i = 0; i < len; i++) {
        draw_pixel(framebuffer, width, height, pitch, x + i, y, color);
    }
}

// Draw a vertical line
void draw_vline(uint32_t *framebuffer, uint32_t width, uint32_t height, 
                uint32_t pitch, int x, int y, int len, uint32_t color) {
    for (int i = 0; i < len; i++) {
        draw_pixel(framebuffer, width, height, pitch, x, y + i, color);
    }
}

// Draw a rectangle
void draw_rect(uint32_t *framebuffer, uint32_t width, uint32_t height, 
               uint32_t pitch, int x, int y, int w, int h, uint32_t color) {
    draw_hline(framebuffer, width, height, pitch, x, y, w, color);
    draw_hline(framebuffer, width, height, pitch, x, y + h - 1, w, color);
    draw_vline(framebuffer, width, height, pitch, x, y, h, color);
    draw_vline(framebuffer, width, height, pitch, x + w - 1, y, h, color);
}

// Fill a rectangle
void fill_rect(uint32_t *framebuffer, uint32_t width, uint32_t height, 
               uint32_t pitch, int x, int y, int w, int h, uint32_t color) {
    for (int dy = 0; dy < h; dy++) {
        draw_hline(framebuffer, width, height, pitch, x, y + dy, w, color);
    }
}

void __kernel_main__(std::uint32_t magic, multiboot_info* mb_info) 
{
	// Check magic number
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		// Not booted by GRUB
		return;
	}

	if (!(mb_info->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO)) {
		// No framebuffer information
		return;
	}

	CRT::_init();
	uint32_t width = mb_info->framebuffer_width;
    uint32_t height = mb_info->framebuffer_height;
    uint32_t pitch = mb_info->framebuffer_pitch;
    uint32_t *framebuffer = (uint32_t*)(uintptr_t)mb_info->framebuffer_addr;

	fill_rect(framebuffer, width, height, pitch, 0, 0, width, height, COLOR_BLACK);
    
    // Draw a white border
    draw_rect(framebuffer, width, height, pitch, 10, 10, width-20, height-20, COLOR_WHITE);


	// Initialize terminal interface
	//Terminal::Initialize();

	//Terminal::WriteString("Hello, kernel World!\n");

	GDT::GDT_Initialize();
}

__CPP_END__