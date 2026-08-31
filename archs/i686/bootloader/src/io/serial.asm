section .text

global __kernel_serial_write_dword__:function (__kernel_serial_write_dword__.end - __kernel_serial_write_dword__.start)
__kernel_serial_write_dword__:
    .start:
        movzx edx, word [esp + 4]
        mov eax, dword [esp + 8]

        out dx, eax
        xor eax, eax

        ret
    .end:

global __kernel_serial_write_word__:function (__kernel_serial_write_word__.end - __kernel_serial_write_word__.start)
__kernel_serial_write_word__:
    .start:
        movzx edx, word [esp + 4]
        movzx eax, word [esp + 8]

        out dx, ax
        xor eax, eax

        ret
    .end:

global __kernel_serial_write_byte__:function (__kernel_serial_write_byte__.end - __kernel_serial_write_byte__.start)
__kernel_serial_write_byte__:
    .start:
        movzx edx, word [esp + 4]
        movzx eax, byte [esp + 8]

        out dx, al
        xor eax, eax

        ret
    .end:

global __kernel_serial_read_dword__:function (__kernel_serial_read_dword__.end - __kernel_serial_read_dword__.start)
__kernel_serial_read_dword__:
    .start:
        movzx edx, word [esp + 4]
        in eax, dx

        ret
    .end:

global __kernel_serial_read_word__:function (__kernel_serial_read_word__.end - __kernel_serial_read_word__.start)
__kernel_serial_read_word__:
    .start:
        xor eax, eax

        movzx edx, word [esp + 4]
        in ax, dx

        ret
    .end:

global __kernel_serial_read_byte__:function (__kernel_serial_read_byte__.end - __kernel_serial_read_byte__.start)
__kernel_serial_read_byte__:
    .start:
        xor eax, eax
        
        movzx edx, word [esp + 4]
        in al, dx

        ret
    .end: