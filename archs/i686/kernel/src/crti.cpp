#include <crt.hpp>

namespace CRT {
	extern func_ptr __init_array_start[0], __init_array_end[0];
	extern func_ptr __fini_array_start[0], __fini_array_end[0];
	
	void crt_init()
	{
		for (func_ptr* func = __init_array_start; func != __init_array_end; func++)
			(*func)();
	}

	void crt_fini()
	{
		for (func_ptr* func = __fini_array_start; func != __fini_array_end; func++)
			(*func)();
	}

	func_ptr __init_array_start[0] __attribute__ ((used, section(".init_array"), aligned(sizeof(func_ptr)))) = { };
	func_ptr __fini_array_start[0] __attribute__ ((used, section(".fini_array"), aligned(sizeof(func_ptr)))) = { };
}
