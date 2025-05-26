global __kernel_load_page_directory__:function (__kernel_load_page_directory__.end - __kernel_load_page_directory__.start)
__kernel_load_page_directory__:
    .start:
        push ebp
        mov ebp, esp

        mov eax, dword [ebp + 8] ; Load the page directory into CR3
        mov eax, dword [eax]
        mov cr3, eax
        xor eax, eax ; Clear EAX to avoid any garbage value

        pop ebp
        ret
    .end:

global __kernel_enable_paging__:function (__kernel_enable_paging__.end - __kernel_enable_paging__.start)
__kernel_enable_paging__:
    .start:
        push ebp
        mov ebp, esp

        mov eax, cr0
        or eax, 1 << 31 ; Set the PG bit in CR0 to enable paging
        mov cr0, eax
        xor eax, eax ; Clear EAX to avoid any garbage value

        pop ebp
        ret
    .end:

