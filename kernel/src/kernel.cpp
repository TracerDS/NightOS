#include <init.hpp>
#include <stddef.h>
#include <stdint.h>

#include <klibc/string.h>

#include <terminal.hpp>
#include <gdt.hpp>

__CPP_START__

using __GlobalConstructorInit_t = void (*)();
extern __GlobalConstructorInit_t* __ctors_start;
extern __GlobalConstructorInit_t* __ctors_end;

void __InitGlobalConstructors() {
    for (auto fn = __ctors_start; fn != __ctors_end; ++fn) {
#ifdef __KERNEL_DEBUG
		Terminal::terminal_write(" [Global Constructors] - Calling ");
		Terminal::terminal_writehex(reinterpret_cast<uint64_t>(fn));
		Terminal::terminal_write("\r\n");
#endif
        (*fn)();
    }
}

class A {
public:
	A() {
		Terminal::terminal_write("A::A()\r\n");
	}
	~A() {
		Terminal::terminal_write("A::~A()\r\n");
	}
};
A a;

void kernel_main() 
{
	Terminal::terminal_initialize();
	__InitGlobalConstructors();	

	const GDT::GDT_Descriptor* descriptor = GDT::GDT_GetDescriptor();
	GDT::GDT_Entry* entries = (GDT::GDT_Entry*)descriptor->entry;

	if (!descriptor->entry) {
		Terminal::terminal_write("GDT Entries are null\r\n");
		return;
	}
	
	auto entries_count = (descriptor->limit + 1) / sizeof(GDT::GDT_Entry);
	Terminal::terminal_write("GDT Entries: ");
	Terminal::terminal_write(entries_count);
	Terminal::terminal_write("\r\n");

	for(uint16_t i = 0; i < entries_count; i++) {
		GDT::GDT_Entry entry = entries[i];
		Terminal::terminal_write("GDT Entry ");
		Terminal::terminal_putchar(i + '0');
		Terminal::terminal_write(": [");
		Terminal::terminal_writehex(*reinterpret_cast<uint64_t*>(&entry));
		Terminal::terminal_write("]\r\n");
	}

	if(true) {
		Terminal::terminal_write("GDT status: ");
		bool status = GDT::GDT_Initialize();
		Terminal::terminal_write(status ? "true" : "false");
		Terminal::terminal_write("\r\n");
	}
}

__CPP_END__