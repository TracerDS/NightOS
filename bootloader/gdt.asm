GDT_Start:
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
GDT_Code: 
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags changed, again, refer to os-dev.pdf
GDT_Data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

GDT_End:

; GDT descriptor
GDT_Descriptor:
    dw GDT_End - GDT_Start - 1 ; size (16 bit), always one less of its true size
    dd GDT_Start ; address (32 bit)

; define some constants for later use
CODE_SEG equ GDT_Code - GDT_Start
DATA_SEG equ GDT_Data - GDT_Start