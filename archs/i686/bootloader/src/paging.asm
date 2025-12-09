section .text

global __kernel_load_page_directory__:function (__kernel_load_page_directory__.end - __kernel_load_page_directory__.start)
__kernel_load_page_directory__:
    .start:
        push ebp
        mov ebp, esp

        mov eax, dword [ebp + 8] ; Load the page directory into CR3
        mov cr3, eax
        xor eax, eax ; Clear EAX to avoid any garbage value

        pop ebp
        ret
    .end:

global __kernel_enable_paging__:function (__kernel_enable_paging__.end - __kernel_enable_paging__.start)
__kernel_enable_paging__:
    .start:
        mov eax, cr0
        or eax, 1 << 31 ; Set the PG bit in CR0 to enable paging
        mov cr0, eax

        xor eax, eax ; Clear EAX to avoid any garbage value
        ret
    .end:

global __kernel_disable_paging__:function (__kernel_disable_paging__.end - __kernel_disable_paging__.start)
__kernel_disable_paging__:
    .start:
        mov eax, cr0
        and eax, ~(1 << 31) ; Clear the PG bit in CR0 to disable paging
        mov cr0, eax

        xor eax, eax ; Clear EAX to avoid any garbage value
        ret
    .end:

global __kernel_paging_enable_pse__:function (__kernel_paging_enable_pse__.end - __kernel_paging_enable_pse__.start)
__kernel_paging_enable_pse__:
    .start:
        mov eax, cr4
        or eax, 1 << 4
        mov cr4, eax

        xor eax, eax ; Clear EAX to avoid any garbage value
        ret
    .end:

global __kernel_paging_enable_pae__:function (__kernel_paging_enable_pae__.end - __kernel_paging_enable_pae__.start)
__kernel_paging_enable_pae__:
    .start:
        mov eax, cr4
        or eax, 1 << 5
        mov cr4, eax

        xor eax, eax ; Clear EAX to avoid any garbage value
        ret
    .end:

global __kernel_flush_tlb_entry__:function (__kernel_flush_tlb_entry__.end - __kernel_flush_tlb_entry__.start)
__kernel_flush_tlb_entry__:
    .start:
        ; [esp + 4] = address to flush
        mov eax, dword [esp + 4] ; Load the address to flush from the stack
        invlpg [eax] ; Invalidate the TLB entry for the specified address

        xor eax, eax ; Clear EAX to avoid any garbage value
        ret
    .end:

global __kernel_flush_tlb_all__:function (__kernel_flush_tlb_all__.end - __kernel_flush_tlb_all__.start)
__kernel_flush_tlb_all__:
    .start:
        mov eax, cr3
        mov cr3, eax ; Reload CR3 to flush the entire TLB
        
        xor eax, eax ; Clear EAX to avoid any garbage value
        ret
    .end:

global __kernel_get_cr2__:function (__kernel_get_cr2__.end - __kernel_get_cr2__.start)
__kernel_get_cr2__:
    .start:
        mov eax, cr2
        ret
    .end:
