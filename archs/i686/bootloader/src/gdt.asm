section .text

global __kernel_load_GDT__:function (__kernel_load_GDT__.end - __kernel_load_GDT__.start)
__kernel_load_GDT__:
    .start:
        push ebp
        mov ebp, esp

        ; [ebp + 8] = descriptor
        mov edx, dword [ebp + 8]
        test edx, edx
        jz .failed

        lgdt [edx]
        jmp 0x08:.reload_cs
    .reload_cs:
        mov eax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        mov eax, 1
        jmp .done
    .failed:
        mov eax, 0
    .done:
        pop ebp
        ret
    .end: