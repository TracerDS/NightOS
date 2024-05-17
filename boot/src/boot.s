.extern nos_kmain
.extern enable_pmode
.global _asm_start

.set MB_MAGIC,    0x1BADB002       		 // 'magic number' lets bootloader find the header
.set MB_ALIGN,    1<<0             		 // align loaded modules on page boundaries
.set MB_MEMINFO,  1<<1             		 // provide memory map
.set MB_FLAGS,    MB_ALIGN | MB_MEMINFO  // this is the Multiboot 'flag' field
.set MB_CHECKSUM, -(MB_MAGIC + MB_FLAGS) // checksum of above, to prove we are multiboot

// We now start the section of the executable that will contain our Multiboot header
.section .multiboot
	.align 4
	.long MB_MAGIC
	.long MB_FLAGS
	.long MB_CHECKSUM

// This section contains data initialised to zeroes when the kernel is loaded
.section .bss
	.align 16
	stack_bottom:
		.skip 16384 # 16 KiB
	stack_top:

/*
The linker script specifies _asm_start as the entry point to the kernel and the
bootloader will jump to this position once the kernel has been loaded. It
doesn't make sense to return from this function as the bootloader is gone.
*/
.section .text
	.type _asm_start, @function
	_asm_start:
		mov $stack_bottom, %esp
		mov $stack_top, %esp

		call nos_kmain

		hang:
			cli
			hlt
			jmp hang

/*
Set the size of the _asm_start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
	.size _asm_start, . - _asm_start
