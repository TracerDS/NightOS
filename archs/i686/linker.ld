/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(__bootloader_start__)

PHDRS
{
	multiboot PT_LOAD FLAGS(4);
	text PT_LOAD FLAGS(5);
	rodata PT_LOAD FLAGS(4);
	data PT_LOAD FLAGS(6);
	bss PT_LOAD FLAGS(6);
}

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	. = 0x00100000; /* 1MB */
	__kernel_boot_start__ = .;

	.multiboot ALIGN(4K) :
	{
		__multiboot_start__ = .;
		*(.multiboot)
		*(.multiboot.*)
		__multiboot_end__ = .;
	} : multiboot

	.preboot ALIGN(4K) :
	{
		__preboot_start__ = .;
		*(.preboot)
		*(.preboot.*)
		__preboot_end__ = .;
	}

	/* Include the list of initialization functions sorted. */
	.init_array :
	{
		__init_array_start = .;
		*crt.o(.init_array)
		KEEP (*(SORT(EXCLUDE_FILE(*crt.o) .init_array.*)))
		KEEP (*(EXCLUDE_FILE(*crt.o) .init_array))
		__init_array_end = .;
	}

	/* Include the list of termination functions sorted. */
	.fini_array :
	{
		__fini_array_start = .;
		*crt.o(.fini_array)
		KEEP (*(SORT(EXCLUDE_FILE(*crt.o) .fini_array.*)))
		KEEP (*(EXCLUDE_FILE(*crt.o) .fini_array))
		__fini_array_end = .;
	}

	__kernel_post_boot_start__ = .;
	. += 0xC0000000;
	__kernel_start__ = .;
	
	.text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000)
	{
		__text_start__ = .;
		*(.text)
		*(.text.*)
		__text_end__ = .;
	} : text

	/* Read-only data. */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
	{
		__rodata_start__ = .;
		*(.rodata)
		*(.rodata.*)
		__rodata_end__ = .;
	} : rodata

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000)
	{
		__data_start__ = .;
		*(.data)
		*(.data.*)
		__data_end__ = .;
	} : data

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000)
	{
		__bss_start__ = .;
		*(COMMON)
		*(.bss)
		*(.bss.*)
		__bss_end__ = .;
	} : bss

	__kernel_end__ = .;
}