#include <init.hpp>
#include <cstddef>
#include <cstdint>
#include <array>

#include <terminal.hpp>

#include <descriptors/gdt.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/isr.hpp>
#include <descriptors/irq.hpp>
#include <memory/paging.hpp>
#include <memory/pmm.hpp>
#include <logger.hpp>
#include <io.hpp>
#include <serial.hpp>
#include <cpuid.hpp>
#include <stackframe.hpp>
#include <keyboard.hpp>
#include <tests.hpp>

#include <klibc/cassert>
#include <klibc/cctype>

#include <video/pixels.hpp>
#include <grub/multiboot.hpp>

#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0x00FFFFFF
#define COLOR_RED       0x00FF0000
#define COLOR_GREEN     0x0000FF00
#define COLOR_BLUE      0x000000FF

extern "C" void __kernel_main__(std::uint32_t magic, multiboot_info* mb_info) noexcept;
extern "C" bool __kernel_check_cpuid__() noexcept;

extern std::uint8_t __text_start__[];
extern std::uint8_t __text_end__[];
extern std::uint8_t __data_start__[];
extern std::uint8_t __data_end__[];
extern std::uint8_t __rodata_start__[];
extern std::uint8_t __rodata_end__[];
extern std::uint8_t __bss_start__[];
extern std::uint8_t __bss_end__[];
extern std::uint8_t __kernel_start__[];
extern std::uint8_t __kernel_end__[];


void __kernel_main__(std::uint32_t magic, multiboot_info* mb_info) noexcept {
	// Check magic number
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		// Not booted by GRUB
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

	// Reinitialize paging
	Paging::Paging_Initialize();

	// Paging enabled, set to our page directory in src/paging.cpp

	// Initialize terminal interface

	//Serial::Initialize(Serial::Ports::COM1);
	StackFrame::PrintFrames(3);

	GDT::GDT_Initialize();
	IDT::IDT_Initialize();
	ISR::ISR_Initialize();
	IRQ::IRQ_Init();
	
	Memory::Init(mb_info);

	ISR::ISR_RegisterHandler(0x20, []([[maybe_unused]] ISR::ISR_RegistersState* regs) {
		if (false) {
			static int ticks = 0;
			++ticks;
			if (ticks > 1000) {
				ticks = 0;
				IO::kprintf("Tick!\r\n");
			}
		}
	});
	
	char* addr = (char*)Memory::request_pages(1);
	IO::kprintf("Allocated page at: 0x%X\r\n", (unsigned int)addr);

	for(int i=0; i<4096 / 4; ++i) {
		((std::uint32_t*)addr)[i] = 0xDEADBEEF;
	}
	
	__asm__("sti\n");
	while (true) {
		__asm__(
			"nop\n"
		);
	}
	return;

	//Log::Logger::log("Logging is working!");

	IO::kprintf_color(
		"Hello\nKernel\nWorld\r\n",
		Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_MAGENTA
	);
	IO::kprintf("__cplusplus: %ld\r\n", __cplusplus);
	IO::kprintf("CPUID supported: %s\r\n", __kernel_check_cpuid__() ? "true" : "false");

	char vendor[13] {0};
	CPUID::GetVendor(vendor);
	IO::kprintf("CPUID vendor: %s\r\n", vendor);
	IO::kprintf("vendor addr: 0x%X\r\n", (unsigned int)&vendor);
	IO::kprintf("LAPIC built in: %s\r\n", CPUID::LAPIC_Supported() ? "true" : "false");


	IO::kprintf("Data: %X\r\n", *(std::uint8_t*)(0x70400 + 0xC0000000) );

	if (true) {
		ISR::ISR_RegisterHandler(0x21, []([[maybe_unused]] ISR::ISR_RegistersState* regs) {
			static bool pressed = false;
			static std::uint8_t prevCode = 0;
			
			auto code = Serial::ReadByte(0x60); // Acknowledge the keyboard interrupt
			if (pressed && code == prevCode) {
				// Ignore repeated key events
				return;
			}

			Keyboard::VirtualKey key = Keyboard::IntToVK(code);
			Keyboard::VirtualKey prevKey = Keyboard::IntToVK(prevCode);
			char charKey = Keyboard::VKToChar(key);

			bool shiftPressed = false;
			// Is shift pressed?
			if (prevKey == Keyboard::VirtualKey::VK_LSHIFT ||
				prevKey == Keyboard::VirtualKey::VK_RSHIFT
			) {
				shiftPressed = true;
			}
			// Special char
			if (charKey == 0) {
				if (key == Keyboard::VirtualKey::VK_BACKSPACE) {
					IO::kprintf("\b");
				}
			} else {
				IO::kprintf("%c", shiftPressed ? klibc::toupper(charKey) : charKey);
			}

			/*
			char bits[9]{0};
			for (int i = 0; i < 8; ++i) {
				bits[7 - i] = (code & (1 << i)) ? '1' : '0';
			}
			if (code & 0x80) {
				pressed = false;
				// Key released
				//IO::kprintf("Key released: 0b%s | 0x%X\r\n", bits, code);
			} else {
				pressed = true;
				// Key pressed
				IO::kprintf("Key pressed:  0b%s | 0x%X\r\n", bits, code);
			}
			*/
			prevCode = code;
		});
	}

	IO::kprintf("__kernel_start__: 0x%X\r\n", (unsigned int)__kernel_start__);
	IO::kprintf("__kernel_end__:   0x%X\r\n", (unsigned int)__kernel_end__);
	IO::kprintf("Kernel size:      0x%X\r\n", (unsigned int)(__kernel_end__ - __kernel_start__));
	
	IO::kprintf("\r\n");

	// __asm__("int 0x1");

	// Enable interrupts and loop
	
	__asm__("sti\n");
	while (true) {
		__asm__(
			"nop\n"
		);
	}
}