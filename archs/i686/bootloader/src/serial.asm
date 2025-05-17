section .text

global __kernel_serial_write_dword__:function (__kernel_serial_write_dword__.end - __kernel_serial_write_dword__.start)
__kernel_serial_write_dword__:
    .start:
        push ebp
        mov ebp, esp

        mov dx, word [ebp + 8]
        mov eax, dword [ebp + 12]

        out dx, eax
        xor eax, eax

        pop ebp
        ret
    .end:

global __kernel_serial_write_word__:function (__kernel_serial_write_word__.end - __kernel_serial_write_word__.start)
__kernel_serial_write_word__:
    .start:
        push ebp
        mov ebp, esp

        mov dx, word [ebp + 8]
        movzx eax, word [ebp + 12]

        out dx, ax
        xor eax, eax

        pop ebp
        ret
    .end:

global __kernel_serial_write_byte__:function (__kernel_serial_write_byte__.end - __kernel_serial_write_byte__.start)
__kernel_serial_write_byte__:
    .start:
        push ebp
        mov ebp, esp

        mov dx, word [ebp + 8]
        movzx eax, byte [ebp + 12]

        out dx, al
        xor eax, eax

        pop ebp
        ret
    .end:

global __kernel_serial_read_dword__:function (__kernel_serial_read_dword__.end - __kernel_serial_read_dword__.start)
__kernel_serial_read_dword__:
    .start:
        push ebp
        mov ebp, esp

        mov dx, word [ebp + 8]
        in eax, dx

        pop ebp
        ret
    .end:

global __kernel_serial_read_word__:function (__kernel_serial_read_word__.end - __kernel_serial_read_word__.start)
__kernel_serial_read_word__:
    .start:
        push ebp
        mov ebp, esp

        xor eax, eax

        mov dx, word [ebp + 8]
        in ax, dx

        pop ebp
        ret
    .end:

global __kernel_serial_read_byte__:function (__kernel_serial_read_byte__.end - __kernel_serial_read_byte__.start)
__kernel_serial_read_byte__:
    .start:
        push ebp
        mov ebp, esp

        xor eax, eax
        
        mov dx, word [ebp + 8]
        in al, dx

        pop ebp
        ret
    .end: