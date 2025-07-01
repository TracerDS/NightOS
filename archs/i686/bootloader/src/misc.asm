section .text
    
global __kernel_halt__:function (__kernel_halt__.end - __kernel_halt__.start)
__kernel_halt__:
	.start:
		hlt
		ret
	.end:

global __kernel_clear_interrupts__:function (__kernel_clear_interrupts__.end - __kernel_clear_interrupts__.start)
__kernel_clear_interrupts__:
	.start:
		cli
		ret
	.end: