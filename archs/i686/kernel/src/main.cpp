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
#include <keyboard.hpp>
#include <memory/mm.hpp>
#include <tests.hpp>

#include <klibc/cassert>
#include <klibc/cctype>

#include <video/pixels.hpp>
#include <grub/multiboot.hpp>

#include <memory/allocator.hpp>

#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0x00FFFFFF
#define COLOR_RED       0x00FF0000
#define COLOR_GREEN     0x0000FF00
#define COLOR_BLUE      0x000000FF

extern "C" void __kernel_main__(std::uint32_t magic, multiboot_info* mb_info);
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

std::uint8_t* __free_memory_beginning__;
std::uint8_t* __free_memory_end__;

static void remap_memory_sections(multiboot_info* mb_info) noexcept {
	auto* mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(mb_info->mmap_addr);

	// Compute end‐of‐buffer pointer:
	std::uintptr_t buffer_end = mb_info->mmap_addr + mb_info->mmap_length;

	std::uint64_t nextAvailAddr = 0;

	while (reinterpret_cast<std::uintptr_t>(mmap_entry) < buffer_end) {
		std::uint64_t base = mmap_entry->addr;
		std::uint64_t length = mmap_entry->len;
		std::uint32_t type = mmap_entry->type;

		auto pagesAmount = align_up(length, Paging::ByteUnits::KB4) / Paging::ByteUnits::KB4;

		if (base < nextAvailAddr) {
			if (base + length < nextAvailAddr) {
				// Already mapped, continue
				continue;
			}
		}

		if (type == MULTIBOOT_MEMORY_RESERVED) {
			IO::kprintf("Reserved: 0x%08llX - 0x%08llX -> 0x%08llX - 0x%08llX\r\n",
				base, base + length - 1, nextAvailAddr, nextAvailAddr + length - 1
			);

			for (auto i = 0; i < pagesAmount; ++i) {
				Paging::map_page(
					base + i * Paging::ByteUnits::KB4,
					nextAvailAddr + i * Paging::ByteUnits::KB4,
					Paging::PageFlags::PAGE_PRESENT | Paging::PageFlags::PAGE_READ_WRITE
				);
			}
			
			nextAvailAddr = align_up(nextAvailAddr + length, Paging::ByteUnits::KB4);
		}

		mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(
			reinterpret_cast<uint8_t*>(mmap_entry)
			+ mmap_entry->size + sizeof(mmap_entry->size)
		);
	}

	auto freeMemBeg = nextAvailAddr;

	mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(mb_info->mmap_addr);
	while (reinterpret_cast<std::uintptr_t>(mmap_entry) < buffer_end) {
		std::uint64_t base = mmap_entry->addr;
		std::uint64_t length = mmap_entry->len;
		std::uint32_t type = mmap_entry->type;

		if (type == MULTIBOOT_MEMORY_AVAILABLE) {
			IO::kprintf("Free: 0x%08llX - 0x%08llX -> 0x%08llX - 0x%08llX\r\n",
				base, base + length, nextAvailAddr, nextAvailAddr + length
			);

			Paging::map_page(
				base,
				nextAvailAddr,
				Paging::PageFlags::PAGE_PRESENT | Paging::PageFlags::PAGE_READ_WRITE |
					Paging::PageFlags::PAGE_USER_SUPERVISOR
			);
			
			nextAvailAddr += length;
		}

		mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(
			reinterpret_cast<uint8_t*>(mmap_entry)
			+ mmap_entry->size + sizeof(mmap_entry->size)
		);
	}

	__free_memory_beginning__ = reinterpret_cast<std::uint8_t*>(freeMemBeg);
	__free_memory_end__ = reinterpret_cast<std::uint8_t*>(nextAvailAddr);
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

	// Reinitialize paging
	Paging::Paging_Initialize();

	// Paging enabled, set to our page directory in src/paging.cpp

	remap_memory_sections(mb_info);

	IO::kprintf("Free memory: 0x%X -> 0x%X\r\n", __free_memory_beginning__, __free_memory_end__);

	// Initialize terminal interface

	//Serial::Initialize(Serial::Ports::COM1);
	StackFrame::PrintFrames(3);

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

	ISR::ISR_RegisterHandler(0x20, [](ISR::ISR_RegistersState* regs) {
		if (false) {
			static int ticks = 0;
			++ticks;
			if (ticks > 1000) {
				ticks = 0;
				IO::kprintf("Tick!\r\n");
			}
		}
	});

	
	namespace LL = Memory::Allocators::LinkedList;
	// LL::Init();
	// auto mem = (char*)LL::kmalloc(14); // Test allocation
	// if (!mem) {
	// 	IO::kprintf("kmalloc failed!\r\n");
	// 	while (true) { __asm__("hlt"); }
	// }
	// IO::kprintf("Allocated 14 bytes at 0x%X\r\n", mem);
	/*

	mem = "Hello, World!";
	IO::kprintf("String at 0x%X: %s\r\n", mem);

	LL::kfree(mem);
	*/
	IO::kprintf("Data: %X\r\n", *(std::uint8_t*)(0x70400 + 0xC0000000) );

	if (true) {
		ISR::ISR_RegisterHandler(0x21, [](ISR::ISR_RegistersState* regs) {
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