#include <cstdint>

namespace __cxxabiv1 {
    __extension__ typedef int __guard __attribute__((mode(__DI__)));

    extern "C" int __cxa_guard_acquire(__guard* g) {
        // TODO: spinlock/mutex
        return !*(char*)(g);
    }

    extern "C" void __cxa_guard_release(__guard* g) {
        // Initialize
        *(char*)g = 1;
        // Free spinlock/mutex
    }

    extern "C" void __cxa_guard_abort(__guard* g) {
        *(char*)g = 0;
    }
}