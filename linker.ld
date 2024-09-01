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
    } :text

    .rodata ALIGN(4K) : 
    {
        *(.rodata)
    } :rodata

    .data ALIGN(4K) : 
    {
        *(.data)
    } :data

    .bss ALIGN(4K) : 
    {
        *(COMMON)
        *(.bss)
    } :bss
}