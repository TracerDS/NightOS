#include <cstdint>

namespace __cxxabiv1 {
    __extension__ typedef int __guard __attribute__((mode(__DI__)));

    namespace {
        using guard_state_t = std::uint32_t;

        constexpr guard_state_t GUARD_INITIALIZED = 1u;
        constexpr guard_state_t GUARD_IN_USE = 1u << 1;
    }

    extern "C" int __cxa_guard_acquire(__guard* g) {
        auto* state = reinterpret_cast<guard_state_t*>(g);

        while (true) {
            guard_state_t current = __atomic_load_n(state, __ATOMIC_ACQUIRE);
            if (current & GUARD_INITIALIZED) {
                return 0;
            }

            if ((current & GUARD_IN_USE) == 0) {
                guard_state_t desired = current | GUARD_IN_USE;
                if (__atomic_compare_exchange_n(
                    state,
                    &current,
                    desired,
                    false,
                    __ATOMIC_ACQ_REL,
                    __ATOMIC_ACQUIRE
                )) {
                    return 1;
                }
            }

            __asm__ volatile("pause");
        }
    }

    extern "C" void __cxa_guard_release(__guard* g) {
        auto* state = reinterpret_cast<guard_state_t*>(g);
        auto current = __atomic_load_n(state, __ATOMIC_RELAXED);
        current |= GUARD_INITIALIZED;
        current &= static_cast<guard_state_t>(~GUARD_IN_USE);
        __atomic_store_n(state, current, __ATOMIC_RELEASE);
    }

    extern "C" void __cxa_guard_abort(__guard* g) {
        auto* state = reinterpret_cast<guard_state_t*>(g);
        __atomic_fetch_and(state, static_cast<guard_state_t>(~GUARD_IN_USE), __ATOMIC_RELEASE);
    }
}