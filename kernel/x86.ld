ENTRY(_start)
STARTUP(obj/kernel_entry.o)
OUTPUT(build/kernel.bin)
OUTPUT_FORMAT(elf32-i386)

INPUT (
	obj/kernel_entry.o
	obj/kernel.o
)

MEMORY (
	KERNEL (wx) : ORIGIN = 0x1000, LENGTH = 4M
)