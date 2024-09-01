.intel_syntax noprefix
.section text
    .global _loadGDT
    .type _loadGDT, @function
    _loadGDT:
        push rbp
        mov rbp, rsp

        mov [rbp - 4], edi
        test [rbp - 4], 0
        jz _loadGDT_ret

        # edi - 1 param
        lgdt [edi]

    _loadGDT_ret:
        pop rbp
        ret
    
    .global _reloadGDTSegments
    .type _reloadGDTSegments, @function
    _reloadGDTSegments:
        push 0x8
        lea rax, [._reloadGDTSegments_do]
        push rax
        ret
    ._reloadGDTSegments_do:
        mov eax, 0x10
        mov ds, eax
        mov es, eax
        mov fs, eax
        mov gs, eax
        mov ss, eax