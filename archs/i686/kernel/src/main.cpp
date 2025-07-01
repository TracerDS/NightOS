#include <init.hpp>
#include <cstddef>
#include <cstdint>

#include <terminal.hpp>

#include <descriptors/gdt.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/isr.hpp>
#include <logger.hpp>
#include <io.hpp>
#include <serial.hpp>
#include <cpuid.hpp>
#include <paging.hpp>
#include <stackframe.hpp>
#include <descriptors/irq.hpp>
#include <klibc/cassert>

#include <tests.hpp>

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
extern std::uint8_t __kernel_start__[];
extern std::uint8_t __kernel_end__[];

std::uint64_t GetFreeMemoryAddressBegin(multiboot_info* mb_info) noexcept {
	return 0;
}

void __kernel_main__(std::uint32_t magic, multiboot_info* mb_info) 
{    
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


	auto* mmap_entry = 
		reinterpret_cast<multiboot_memory_map_t*>(mb_info->mmap_addr);

	// Compute end‐of‐buffer pointer:
	uintptr_t buffer_end = 
		mb_info->mmap_addr + mb_info->mmap_length;

	// Walk all entries:
	while (reinterpret_cast<uintptr_t>(mmap_entry) < buffer_end) {
		uint64_t base = mmap_entry->addr;
		uint64_t length = mmap_entry->len;
		uint32_t type = mmap_entry->type;

		IO::kprintf(
			"Memory region: base=0x%llX, length=0x%llX, type=",
			base, length
		);

		// e.g. print or record this region:
		switch (type) {
			case MULTIBOOT_MEMORY_AVAILABLE:
				IO::kprintf("Usable ram\r\n");
				break;
			case MULTIBOOT_MEMORY_RESERVED:
				IO::kprintf("Reserved by BIOS\r\n");
				break;
			case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
				IO::kprintf("ACPI reclaimable\r\n");
				break;
			case MULTIBOOT_MEMORY_NVS:
				IO::kprintf("ACPI NVS (non‐volatile storage)\r\n");
				break;
			case MULTIBOOT_MEMORY_BADRAM:
				IO::kprintf("Bad RAM (not usable)\r\n");
				break;
			default:
				IO::kprintf("Unknown type (%d)\r\n", type);
		}

		// Advance to the next entry:
		// note: each entry is “size” bytes plus the size‐field itself
		mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(
			reinterpret_cast<uint8_t*>(mmap_entry)
			+ mmap_entry->size + sizeof(mmap_entry->size)
		);
	}
	return;

	// Initialize terminal interface

	//Serial::Initialize(Serial::Ports::COM1);
	//StackFrame::PrintFrames(2);
	float a = 6.422f;
	float b = 3.210f;
	float c = a/b;
	auto d = static_cast<std::uint32_t>(c);
	IO::kprintf("a: %f, b: %f, c: %f, d: %d\r\n", a, b, c, d);
	return;

	{
		float a = 1.23456789;

		IO::kprintf("Float: %x | 0x%x\r\n", a, &a);

		#define SPEC1 "20"
		#define SPEC2 "f"
		IO::kprintf("%%" SPEC1 "hh" SPEC2 ""   ":\t[%" SPEC1 "hh" SPEC2 "]\r\n", a);
		IO::kprintf("%%" SPEC1 "h"  SPEC2 " "  ":\t[%" SPEC1 "h"  SPEC2 "]\r\n", a);
		IO::kprintf("%%" SPEC1 ""   SPEC2 "  " ":\t[%" SPEC1 ""   SPEC2 "]\r\n", a);
		IO::kprintf("%%" SPEC1 "l"  SPEC2 " "  ":\t[%" SPEC1 "l"  SPEC2 "]\r\n", a);
		IO::kprintf("%%" SPEC1 "ll" SPEC2 ""   ":\t[%" SPEC1 "ll" SPEC2 "]\r\n", a);
		IO::kprintf("%%" SPEC1 "j"  SPEC2 " "  ":\t[%" SPEC1 "j"  SPEC2 "]\r\n", a);
		IO::kprintf("%%" SPEC1 "z"  SPEC2 " "  ":\t[%" SPEC1 "z"  SPEC2 "]\r\n", a);
		IO::kprintf("%%" SPEC1 "t"  SPEC2 " "  ":\t[%" SPEC1 "t"  SPEC2 "]\r\n", a);
		IO::kprintf("%%" SPEC1 "L"  SPEC2 " "  ":\t[%" SPEC1 "L"  SPEC2 "]\r\n", a);
		#undef SPEC1

		IO::kprintf("\r\n\r\n");
	}

	GDT::GDT_Initialize();
	IDT::IDT_Initialize();
	ISR::ISR_Initialize();
	IRQ::IRQ_Init();

	//Log::Logger::log("Logging is working!");

	IO::kprintf_color("Hello\nKernel\nWorld\r\n", Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_MAGENTA);
	IO::kprintf("__cplusplus: %d\r\n", __cplusplus);
	IO::kprintf("CPUID supported: %s\r\n", __kernel_check_cpuid__() ? "true" : "false");

	char vendor[13] {0};
	CPUID::GetVendor(vendor);
	IO::kprintf("CPUID vendor: %s\r\n", vendor);
	IO::kprintf("vendor addr: 0x%X\r\n", &vendor);
	IO::kprintf("LAPIC built in: %s\r\n", CPUID::LAPIC_Supported() ? "true" : "false");

	//Paging::Paging_Initialize();

	if (true) {
		ISR::ISR_RegisterHandler(0x20, [](ISR::ISR_RegistersState* regs) {
			static int ticks = 0;
			++ticks;
			if (ticks > 1000) {
				ticks = 0;
				//IO::kprintf("Tick!\r\n");
			}
		});
		if (true)
		ISR::ISR_RegisterHandler(0x21, [](ISR::ISR_RegistersState* regs) {
			static bool pressed = false;
			static std::uint8_t prevCode = 0;
			
			auto code = Serial::ReadByte(0x60); // Acknowledge the keyboard interrupt
			if (pressed && code == prevCode) {
				// Ignore repeated key events
				return;
			}

			char bits[9]{0};
			for (int i = 0; i < 8; ++i) {
				bits[7 - i] = (code & (1 << i)) ? '1' : '0';
			}
			if (code & 0x80) {
				pressed = false;
				// Key released
				//IO::kprintf("Key released: 0b%s\r\n", bits);
			} else {
				pressed = true;
				// Key pressed
				//IO::kprintf("Key pressed:  0b%s | 0x%X\r\n", bits, code);
			}
			prevCode = code;
		});
	}

	IO::kprintf("__kernel_start__: 0x%X\r\n", __kernel_start__);
	IO::kprintf("__kernel_end__:   0x%X\r\n", __kernel_end__);
	IO::kprintf("Kernel size:      0x%X\r\n", __kernel_end__ - __kernel_start__);
	
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

__CPP_END__