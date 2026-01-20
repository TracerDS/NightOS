section .text

global __kernel_load_IDT__:function (__kernel_load_IDT__.end - __kernel_load_IDT__.start)
__kernel_load_IDT__:
    .start:
        ; [esp + 4] = descriptor
        mov edx, dword [esp + 4]
        test edx, edx
        jz .failed

        lidt [edx]
        mov eax, 1
        ret
    .failed:
        xor eax, eax
        ret
    .end: