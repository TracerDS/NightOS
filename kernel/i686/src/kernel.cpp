#include <init.hpp>
#include <stddef.h>
#include <stdint.h>
#include <gdt.hpp>
#include <terminal.hpp>
#include <crt.hpp>

__CPP_START__

void __kernel_main__(void* a, void* b) 
{
	_init();

	// Initialize terminal interface
	Terminal::Initialize();

	Terminal::WriteString("Hello, kernel World!\n");

	const GDT::GDT_Descriptor* gdt = GDT::GDT_GetCurrentDescriptor();

	auto* entries = GDT::GDT_GetEntries();
	auto num = GDT::GDT_Initialize();
	//terminal_writenumber(num ? 1 : 0);
}

__CPP_END__