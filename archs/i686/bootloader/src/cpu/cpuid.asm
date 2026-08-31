global __kernel_check_cpuid__:function (__kernel_check_cpuid__.end - __kernel_check_cpuid__.start)
__kernel_check_cpuid__:
	.start:
		push ebp
		mov ebp, esp

		pushfd ; push EFLAGS
		pushfd
		pop eax ; store EFLAGS in EAX
		mov ecx, eax ; copy EAX to ECX
		xor eax, 1 << 21 ; toggle the ID bit
		push eax
		popfd ; update EFLAGS
		pushfd
		pop eax ; read EFLAGS
		xor eax, ecx ; compare EAX with the original EFLAGS
		and eax, 1 << 21 ; check if the ID bit is set
		setnz al ; if changed, CPUID is supported
		popfd ; restore EFLAGS

		pop ebp
		ret
	.end:

; __cdecl CPUID_Data __kernel_call_cpuid__(int eax, int ecx)
global __kernel_call_cpuid__:function (__kernel_call_cpuid__.end - __kernel_call_cpuid__.start)
__kernel_call_cpuid__:
    .start:
        push ebp
        mov ebp, esp

        push edi

        mov eax, dword [ebp + 12] ; type
        mov ecx, dword [ebp + 16] ; subtype
        cpuid

        mov edi, dword [ebp + 8] ; pointer to CPUID_Data

        mov dword [edi + 0], eax ; eax
        mov dword [edi + 4], ebx ; ebx
        mov dword [edi + 8], ecx ; ecx
        mov dword [edi + 12], edx ; edx

        pop edi
        pop ebp
        ret
    .end: