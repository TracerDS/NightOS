global __kernel_load_TSS__:function (__kernel_load_TSS__.end - __kernel_load_TSS__.start)
__kernel_load_TSS__:
    .start:
        ; [esp + 4] = descriptor
        movzx edx, word [esp + 4]
        test edx, edx
        jz .failed

        ltr dx
        mov eax, 1
        ret
    .failed:
        xor eax, eax
        ret
    .end: