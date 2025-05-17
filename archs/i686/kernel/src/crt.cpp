#include <init.hpp>

__CPP_START__

using func_ptr = void (*)();

func_ptr __init_array_start[0], __init_array_end[0];
func_ptr __fini_array_start[0], __fini_array_end[0];

void __kernel_crt_init__()
{
	for (func_ptr* func = __init_array_start; func != __init_array_end; func++)
		(*func)();
}

void __kernel_crt_fini__()
{
	for (func_ptr* func = __fini_array_start; func != __fini_array_end; func++)
		(*func)();
}

__CPP_END__