.intel_syntax noprefix
.section .text
    .global _loadGDT
    .type _loadGDT, @function
    // __cdecl _loadGDT ( GDT_Descriptor* descriptor )
    _loadGDT:
        push ebp
        mov ebp, esp

        jmp _loadGDT_ret

        mov eax, [ebp + 8]
        test eax, eax
        jz _loadGDT_ret

        lgdt [eax]

        push 0x8
        lea eax, [._reloadGDTSegments]
        push eax
        retf
    ._reloadGDTSegments:
        mov eax, 0x10
        mov ds, eax
        mov es, eax
        mov fs, eax
        mov gs, eax
        mov ss, eax
        mov eax, 1

    _loadGDT_ret:
        pop ebp
        ret