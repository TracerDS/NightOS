PHDRS
{
    text PT_LOAD FLAGS(5);  /* Read and Execute */
    rodata PT_LOAD FLAGS(4);  /* Read-Only */
    data PT_LOAD FLAGS(6);  /* Read and Write */
    bss PT_LOAD FLAGS(6);  /* Read and Write */
}

ENTRY(_asm_start)

SECTIONS
{
    . = 2M;

    .text ALIGN(4K) : 
    {
        *(.multiboot)
        *(.text)
        *(.text.*)
    } :text

    .rodata ALIGN(4K) : 
    {
        *(.rodata)
        *(.rodata.*)
    } :rodata

    .data ALIGN(4K) : 
    {
        *(.data)
        *(.data.*)
    } :data

    .init_array :
    {
        __init_array_start = .;
        KEEP(*(SORT_BY_INIT_PRIORITY(EXCLUDE_FILE(crti.o crtn.o) .init_array.*)))
        KEEP(*(EXCLUDE_FILE(crti.o crtn.o) .init_array))
        __init_array_end = .;
    } :data

    .bss ALIGN(4K) : 
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    } :bss
}