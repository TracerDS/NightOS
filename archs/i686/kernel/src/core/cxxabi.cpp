#include "core/io.hpp"
#include <cstdint>

namespace __cxxabiv1 {
    using __guard = int __attribute__((mode(__DI__)));

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

void* __dso_handle = reinterpret_cast<void*>(&__dso_handle);

namespace {
    constexpr std::size_t MAX_DESTRUCTORS = 128;
    using destructor_func_t = void (*)(void*);

    struct DestructionTarget {
        destructor_func_t destructor;
        void* arg;
        void* dso_handle;
    };

    DestructionTarget g_destructors[MAX_DESTRUCTORS];
    std::size_t g_destructorCount = 0;
}

extern "C" {
    [[noreturn]] void abort() {
        asm volatile("cli");
        while (true) {
            asm volatile("hlt");
        }
    }

    int __cxa_atexit(
        [[maybe_unused]] destructor_func_t destructor,
        [[maybe_unused]] void* arg,
        [[maybe_unused]] void* dso_handle
    ) noexcept {
        if (g_destructorCount >= MAX_DESTRUCTORS) {
            return -1; // No space left
        }
        
        g_destructors[g_destructorCount++] = {
            .destructor = destructor,
            .arg = arg,
            .dso_handle = dso_handle
        };

        return 0;
    }

    void __cxa_finalize(void* dso_handle) noexcept {
        for (std::size_t i = g_destructorCount; i > 0; --i) {
            auto& entry = g_destructors[i - 1];

            if (!entry.destructor) {
                continue;
            }

            // !dso_handle means calling everything
            if (!dso_handle || entry.dso_handle == dso_handle) {
                entry.destructor(entry.arg);
                entry.destructor = nullptr; // Mark as called
            }
        }
    }
}

namespace std {
    [[noreturn]] void terminate() noexcept {
        NOS::IO::kprintf_color(
            "std::terminate() called!\r\n",
            NOS::Terminal::VGAColor::VGA_COLOR_RED,
            NOS::Terminal::VGAColor::VGA_COLOR_BLACK
        );
        ::abort();
    }
}
