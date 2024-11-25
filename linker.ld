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
        __text_start = .;

        *(.multiboot)
        *(.text)
        *(.text.*)

        __text_end = .;
    } :text

    .rodata ALIGN(4K) : 
    {
        __rodata_start = .;

        __ctors_start = .;
        *(.init_array)
        __ctors_end = .;
        
        __dtors_start = .;
        *(.fini_array)
        __dtors_end = .;

        *(.rodata)
        *(.rodata.*)

        __rodata_end = .;
    } :rodata

    .data ALIGN(4K) : 
    {
        __data_start = .;

        *(.data)
        *(.data.*)
        
        __data_end = .;
    } :data

    .bss ALIGN(4K) : 
    {
        __bss_start = .;

        *(COMMON)
        *(.bss)
        *(.bss.*)

        __bss_end = .;
    } :bss
}