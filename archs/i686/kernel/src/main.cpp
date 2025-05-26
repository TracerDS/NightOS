#include <init.hpp>
#include <cstddef>
#include <cstdint>

#include <terminal.hpp>

#include <descriptors/gdt.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/isr.hpp>
#include <io.hpp>
#include <cpuid.hpp>
#include <paging.hpp>

#include <video/pixels.hpp>
#include <grub/multiboot.hpp>

__CPP_START__

#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0x00FFFFFF
#define COLOR_RED       0x00FF0000
#define COLOR_GREEN     0x0000FF00
#define COLOR_BLUE      0x000000FF

extern "C" bool __kernel_check_cpuid__() noexcept;

extern std::uint8_t __text_start__[];
extern std::uint8_t __text_end__[];
extern std::uint8_t __data_start__[];
extern std::uint8_t __data_end__[];
extern std::uint8_t __rodata_start__[];
extern std::uint8_t __rodata_end__[];
extern std::uint8_t __bss_start__[];
extern std::uint8_t __bss_end__[];

void __higher_half_kernel__();

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

	char vendor[13] {0};
	CPUID::GetVendor(vendor);
	IO::kprintf("CPUID vendor: %s\r\n", vendor);

	//Paging::Paging_Initialize();

	// 0x20119c
	int a = 1;
	int b = 0;
	//int c = a / b;
	if (true) {
		__asm__ volatile (
			"int 0x8\n"
			// "int 0x1\n"
		);
	}
	IO::kprintf("\r\nStill here");
}

void __higher_half_kernel__() {
	IO::kprintf_color("Higher half kernel is running!\r\n", Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_GREEN);
}

__CPP_END__