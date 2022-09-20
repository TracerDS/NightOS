ENTRY(_start)
STARTUP(obj/kernel_entry.o)
OUTPUT(build/kernel.bin)
OUTPUT_FORMAT(elf64-x86-64)

INPUT (
	obj/kernel_entry.o
	obj/kernel.o
)

MEMORY (
	RAM (wx) : org = 0x00100000, len = 1M
)