#include <init.hpp>
#include <cstddef>
#include <cstdint>
#include <descriptors/gdt.hpp>
#include <terminal.hpp>

#include <video/pixels.hpp>
#include <grub/multiboot.hpp>

__CPP_START__

#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0x00FFFFFF
#define COLOR_RED       0x00FF0000
#define COLOR_GREEN     0x0000FF00
#define COLOR_BLUE      0x000000FF

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

    //CRT::crt_init();

    if (false) {
        uint32_t width = mb_info->framebuffer_width;
        uint32_t height = mb_info->framebuffer_height;
        uint32_t pitch = mb_info->framebuffer_pitch;
        uint32_t *framebuffer = (uint32_t*)(uintptr_t)mb_info->framebuffer_addr;

        fill_rect(framebuffer, width, height, pitch, 0, 0, width, height, COLOR_BLACK);
        
        // Draw a white border
        draw_rect(framebuffer, width, height, pitch, 10, 10, width-20, height-20, COLOR_WHITE);
    }
	// Initialize terminal interface
	Terminal::Initialize();

	Terminal::WriteString("Hello, kernel World!\n");

	GDT::GDT_Initialize();

	__asm__ volatile(
		"int 0x0\n"
	);
}

__CPP_END__