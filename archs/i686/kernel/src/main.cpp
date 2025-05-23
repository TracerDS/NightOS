#include <init.hpp>
#include <cstddef>
#include <cstdint>

#include <terminal.hpp>

#include <descriptors/gdt.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/isr.hpp>
#include <io.hpp>
#include <cpuid.hpp>

#include <video/pixels.hpp>
#include <grub/multiboot.hpp>

__CPP_START__

#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0x00FFFFFF
#define COLOR_RED       0x00FF0000
#define COLOR_GREEN     0x0000FF00
#define COLOR_BLUE      0x000000FF

extern "C" bool __kernel_check_cpuid__() noexcept;

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

#ifdef __NOS_KERNEL_USE_FRAMEBUFFER__
    {
        uint32_t width = mb_info->framebuffer_width;
        uint32_t height = mb_info->framebuffer_height;
        uint32_t pitch = mb_info->framebuffer_pitch;
        uint32_t* framebuffer = (uint32_t*)(uintptr_t)mb_info->framebuffer_addr;

        fill_rect(framebuffer, width, height, pitch, 0, 0, width, height, COLOR_BLACK);
        
        // Draw a white border
        draw_rect(framebuffer, width, height, pitch, 10, 10, width-20, height-20, COLOR_WHITE);
    }
#endif

	// Initialize terminal interface

	GDT::GDT_Initialize();
	IDT::IDT_Initialize();
	ISR::ISR_Initialize();

	IO::kprintf_color("Hello\nKernel\nWorld\r\n", Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_MAGENTA);
	IO::kprintf("__cplusplus: %d\r\n", __cplusplus);
	IO::kprintf("CPUID supported: %s\r\n", __kernel_check_cpuid__() ? "true" : "false");

	auto data = CPUID::get_cpuid(0);
	char vendor[13] {0};
	memcpy(vendor, &data.ebx, 4);
	memcpy(vendor+4, &data.edx, 4);
	memcpy(vendor+8, &data.ecx, 4);
	IO::kprintf("CPUID vendor: %s\r\n", vendor);

	// 0x20119c
	__asm__ volatile (
		"int 0x20\n"
		"nop\n"
	);
	IO::kprintf("\r\nStill here");
}

__CPP_END__