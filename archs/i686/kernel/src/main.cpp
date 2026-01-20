#include <init.hpp>
#include <cstddef>
#include <cstdint>

#include <terminal.hpp>

#include <descriptors/gdt.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/isr.hpp>
#include <descriptors/irq.hpp>
#include <descriptors/kernel_interrupts.hpp>
#include <memory/paging.hpp>
#include <memory/pmm.hpp>
#include <memory/vmm.hpp>
#include <logger.hpp>
#include <io.hpp>
#include <interfaces/streams/serial.hpp>
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

namespace Serial = NOS::Serial;
namespace ISR = NOS::Interrupts::ISR;
namespace IO = NOS::IO;

using NOS::Terminal::VGAColor;

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

	// Initialize terminal interface
	Serial::g_serial.init(NOS::Serial::COM1);
	Serial::g_serial.init(NOS::Serial::COM2);

	// Print stackframe
	NOS::StackFrame::PrintFrames(3);

	NOS::Descriptors::Init();
	NOS::Interrupts::Init();

	ISR::RegisterHandler(0x20, []([[maybe_unused]] ISR::InterruptState* regs) {
		if (false) {
			static int ticks = 0;
			++ticks;
			if (ticks > 1000) {
				ticks = 0;
				IO::kprintf("Tick!\r\n");
			}
		}
	});
	ISR::RegisterHandler(
		NOS::Interrupts::KernelInterrupts::KERNEL_PANIC,
		[]([[maybe_unused]] ISR::InterruptState* regs) {
			IO::kprintf_color(
				"Kernel Panic!\r\n"
				"EIP: 0x%08lX\r\n"
				"Error: %ld\r\n"
				"Halting system...\r\n",
				VGAColor::VGA_COLOR_LIGHT_RED,
				VGAColor::VGA_COLOR_BLACK,
				regs->eip,
				regs->error
			);

			asm volatile("cli\n");
			while (true) {
				asm volatile("hlt\n");
			}
		}
	);

	// Reinitialize paging
	NOS::Memory::g_paging.init();

	// Paging enabled, set to our page directory in src/paging.cpp
	NOS::Memory::g_pmmAllocator.init(mb_info);
	
	IO::kprintf(
		"Kernel loaded at: 0x%08lX - 0x%08lX\r\n",
		reinterpret_cast<std::uintptr_t>(__kernel_start__),
		reinterpret_cast<std::uintptr_t>(__kernel_end__)
	);

	IO::kprintf("__cplusplus: %ld\r\n", __cplusplus);
	IO::kprintf("CPUID supported: %s\r\n", __kernel_check_cpuid__() ? "true" : "false");

	char vendor[13] {0};
	CPUID::GetVendor(vendor);
	IO::kprintf("CPUID vendor: %s\r\n", vendor);
	IO::kprintf("LAPIC built in: %s\r\n", CPUID::LAPIC_Supported() ? "true" : "false");

	if (true) {
		ISR::RegisterHandler(0x21, []([[maybe_unused]] ISR::InterruptState* regs) {
			static bool pressed = false;
			static std::uint8_t prevCode = 0;
			
			auto code = Serial::g_serial.read_byte(0x60); // Acknowledge the keyboard interrupt
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

	// Enable interrupts and loop	
	__asm__("sti\n");
	while (true) {
		__asm__(
			"nop\n"
		);
	}
}