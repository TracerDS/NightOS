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

global __kernel_are_interrupts_enabled__:function (__kernel_are_interrupts_enabled__.end - __kernel_are_interrupts_enabled__.start)
__kernel_are_interrupts_enabled__:
    .start:
        pushfd
        pop eax
        shr eax, 9
        and eax, 1
        ret
    .end: