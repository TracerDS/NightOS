section .multiboot
	
MB_FRAMEBUFFER 	equ 0 		; Disable framebuffer

MB_FB_WIDTH     equ	1280	; Requested width (can be 0 for default)
MB_FB_HEIGHT    equ	720		; Requested height (can be 0 for default)
MB_FB_DEPTH     equ	32      ; Requested depth (can be 0 for default)

%ifdef __MULTIBOOT2__

MB_MAGIC    		equ 0xE85250D6
MB_ARCHITECTURE		equ 0 ; 32-bit pmode i386
MB_HEADER_LENGTH	equ multiboot_header.end - multiboot_header.start
MB_CHECKSUM			equ -(MB_MAGIC + MB_ARCHITECTURE + MB_HEADER_LENGTH) ; checksum of the header

MB_TAG_END 		equ 0
MB_TAG_OPTIONAL equ 1

section .multiboot
align 4
multiboot_header:
	.start:
		dd MB_MAGIC	; Magic number
		dd MB_ARCHITECTURE	; Architecture
		dd MB_HEADER_LENGTH	; Header length
		dd MB_CHECKSUM	; Checksum
		
%if MB_FRAMEBUFFER != 0
		; Framebuffer request tag
	.framebuffer.start:
		dw 5				; Tag type (5 = framebuffer)
		dw MB_TAG_OPTIONAL	; Tag flags (1 = optional)
		dd .framebuffer.end - .framebuffer.start ; Tag size
		dd MB_FB_WIDTH
		dd MB_FB_HEIGHT
		dd MB_FB_DEPTH

	.framebuffer.end:
		dw MB_TAG_END
		dw 0
		dd 8
%endif
	.end:

%else
MB_MAGIC    equ 0x1BADB002	; 'magic number' lets bootloader find the header
MB_ALIGN    equ 1 << 0		; align loaded modules on page boundaries
MB_MEMINFO  equ 1 << 1		; provide memory map
MB_VIDINFO  equ 1 << 2		; provide video information

; this is the Multiboot 'flag' field
%if MB_FRAMEBUFFER != 0
MB_FLAGS	equ MB_ALIGN | MB_MEMINFO | MB_VIDINFO
%else
MB_FLAGS	equ MB_ALIGN | MB_MEMINFO
%endif
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)	; checksum of the header

MB_FB_MODE	equ 0	; Requested mode (0 for framebuffer)

section .multiboot
align 4
multiboot_header:
	.start:
		dd MB_MAGIC	; Magic number
		dd MB_FLAGS	; Flags
		dd MB_CHECKSUM	; Checksum
		
%if MB_FRAMEBUFFER != 0
		; Unused fields for compatibility
		dd 0            ; Unused
		dd 0            ; Unused
		dd 0            ; Unused
		dd 0            ; Unused
		dd 0            ; Unused
	.framebuffer.start:
		; Framebuffer request tag
		dd MB_FB_MODE	; Requested mode
		dd MB_FB_WIDTH	; Requested width
		dd MB_FB_HEIGHT	; Requested height
		dd MB_FB_DEPTH	; Requested depth
	.framebuffer.end:
%endif
	.end:
%endif ; __MULTIBOOT2__

extern __kernel_start__

BOOT_START equ 0x00200000
KERNEL_VIRTUAL_ADDRESS equ 0xC0000000
KERNEL_PHYSICAL_ADDRESS equ __kernel_start__ - KERNEL_VIRTUAL_ADDRESS ; Physical address of the kernel

section .bss
align 16
stack_bottom:
	resb 16384 ; 16 KiB
stack_top:

; The linker script specifies __kernel_main__ as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
section .preboot
extern __kernel_main__
extern __kernel_crt_init__
extern __kernel_crt_fini__

extern __kernel_paging_enable_pae__
extern __kernel_paging_enable_pse__

global __bootloader_start__:function (__bootloader_start__.end - __bootloader_start__.start)
__bootloader_start__:
	.start:
		; State:
		; - 32bit protected mode
		; - interrupts disabled
		; - no paging
		; - temp GDT loaded
		; - no stack
		; - no heap
		; - no memory management
		; - no C runtime
		; - eax -> multiboot magic
		; - ebx -> multiboot info

		; Save parameters from the bootloader
		mov edi, eax

		; setup initial paging

	.paging.start:
		; Load page directory. Subtract the virtual addr to calculate a physical address
		lea eax, dword [__INIT_PAGE_DIRECTORY__ - KERNEL_VIRTUAL_ADDRESS]
		mov cr3, eax

		; Enable PSE
		mov eax, cr4
		or eax, 10000b
		mov cr4, eax

		; Enable paging
		mov eax, cr0
		or eax, 1 << 31
		mov cr0, eax
	.paging.end:

		; paging enabled.
		; identity mapped the first few MB of memory
		; Kernel is mapped at 0xC0000000

		mov esp, stack_top
		xor ebp, ebp ; setup null frame
		
		call __kernel_crt_init__

		push ebx
		push edi

		; Enter the high-level kernel.
		call __kernel_main__

		call __kernel_crt_fini__

		; Halt
		cli
	.hang:
		hlt
		jmp .hang
	.end:

section .data
align 4096
__INIT_PAGE_DIRECTORY__:
	.start:
		; Identity map the first 4MB of memory
		dd (0x400000 * 0) | 10000011b

		times 768 - 1 dd 0 ; clear the rest

		dd (BOOT_START >> 22) | 10000011b ; Map the kernel at 0xC0000000
		times 1024 - 768 - 1 dd 0
	.end:

%if __INIT_PAGE_DIRECTORY__.end - __INIT_PAGE_DIRECTORY__.start != 4096
	%error "Page directory size mismatch"
%endif