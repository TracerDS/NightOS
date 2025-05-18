global __kernel_load_IDT__:function (__kernel_load_IDT__.end - __kernel_load_IDT__.start)
__kernel_load_IDT__:
    .start:
        push ebp
        mov ebp, esp

        ; [ebp + 8] = descriptor
        mov edx, dword [ebp + 8]
        test edx, edx
        jz .failed

        lidt [edx]
        mov eax, 1
        jmp .done
    .failed:
        mov eax, 0
    .done:
        pop ebp
        ret
    .end: