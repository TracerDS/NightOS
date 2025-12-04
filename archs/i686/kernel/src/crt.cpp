#include <init.hpp>

extern "C" {
	using func_ptr = void (*)();

	extern func_ptr __init_array_start[];
	extern func_ptr __init_array_end[];
	extern func_ptr __fini_array_start[];
	extern func_ptr __fini_array_end[];

	void __kernel_crt_init__() noexcept
	{
		for (
			func_ptr* func = __init_array_start;
			func != __init_array_end;
			func++
		) {
			(*func)();
		}
	}

	void __kernel_crt_fini__() noexcept
	{
		for (
			func_ptr* func = __fini_array_start;
			func != __fini_array_end;
			func++
		) {
			(*func)();
		}
	}
}