ENTRY(_start)
OUTPUT_FORMAT(elf64-x86-64)

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
