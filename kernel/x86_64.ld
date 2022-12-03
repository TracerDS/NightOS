ENTRY(_start)
OUTPUT_FORMAT(elf64-x86-64)
STARTUP(../bootloader/obj/kernel_entry.o)

MEMORY {
	KERNEL (rwx) : org = 0x1000, len = 10240
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