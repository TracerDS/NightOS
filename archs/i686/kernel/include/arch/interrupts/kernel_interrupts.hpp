#pragma once

#include <cstdint>
#include <core/init.hpp>

namespace NOS::Interrupts {
    namespace KernelInterrupts {
        constexpr std::uint8_t KERNEL_PANIC = 50;
    }

    template <std::uint8_t IntNum>
    FORCE_INLINE void CallInterrupt() noexcept {
        asm volatile (
            "int %0"
            :
            : "i"(IntNum)
            : "memory", "cc"
        );
    }
    FORCE_INLINE void KernelPanic() noexcept {
        CallInterrupt<Interrupts::KernelInterrupts::KERNEL_PANIC>();
    }
}