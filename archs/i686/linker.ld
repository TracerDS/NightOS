/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(__bootloader_start__)

PHDRS
{
	text PT_LOAD FLAGS(5);
	rodata PT_LOAD FLAGS(4);
	data PT_LOAD FLAGS(6);
	bss PT_LOAD FLAGS(6);
}

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* It used to be universally recommended to use 1M as a start offset,
	   as it was effectively guaranteed to be available under BIOS systems.
	   However, UEFI has made things more complicated, and experimental data
	   strongly suggests that 2M is a safer place to load. In 2016, a new
	   feature was introduced to the multiboot2 spec to inform bootloaders
	   that a kernel can be loaded anywhere within a range of addresses and
	   will be able to relocate itself to run from such a loader-selected
	   address, in order to give the loader freedom in selecting a span of
	   memory which is verified to be available by the firmware, in order to
	   work around this issue. This does not use that feature, so 2M was
	   chosen as a safer option than the traditional 1M. */
	. = 2M;

	/* First put the multiboot header, as it is required to be put very early
	   in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	} : text

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	} : rodata

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	} : data

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	} : bss

	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */

	/* Include the list of initialization functions sorted. */
	.init_array :
	{
		__init_array_start = .;
		crt.o(.init_array)
		KEEP (*(SORT(EXCLUDE_FILE(crt.o) .init_array.*)))
		KEEP (*(EXCLUDE_FILE(crt.o) .init_array))
		__init_array_end = .;
	}

	/* Include the list of termination functions sorted. */
	.fini_array :
	{
		__fini_array_start = .;
		crt.o(.fini_array)
		KEEP (*(SORT(EXCLUDE_FILE(crt.o) .fini_array.*)))
		KEEP (*(EXCLUDE_FILE(crt.o) .fini_array))
		__fini_array_end = .;
	}
}