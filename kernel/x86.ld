ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
STARTUP(obj/kernel_entry.o)

INPUT (
	obj/kernel_entry.o
	obj/kernel.o
)

MEMORY {
	KERNEL (rwx) : org = 0x1000, len = 1024
}

SECTIONS {
	. = 0x1000;

	.entry : {
		*(.entry)
	}>KERNEL

	.text : {
		*(.text)
	}>KERNEL

	.data : {
		*(.data)
	}>KERNEL

	.rodata : {
		*(.rodata)
	}>KERNEL

	.bss : {
		_BSS_START_ = .;
		*(.bss)
		_BSS_END_ = .;
	}
}