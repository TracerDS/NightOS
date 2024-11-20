#pragma once

#include <cpp.hpp>

__CPP_START__

using destructor_func_t = void(*)(void*);
struct atexit_func_entry_t
{
    /*
    * Each member is at least 4 bytes large. Such that each entry is 12bytes.
    * 128 * 12 = 1.5KB exact.
    **/
    destructor_func_t destructor_func;
    void* obj_ptr;
    void* dso_handle;
};

int __cxa_atexit(destructor_func_t f, void* objptr, void* dso);
void __cxa_finalize(void* f);

__CPP_END__