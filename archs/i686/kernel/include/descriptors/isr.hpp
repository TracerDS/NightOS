#pragma once

#include <init.hpp>

#include <descriptors/descriptors.hpp>
#include <klibc/array>

namespace NOS::Interrupts::ISR {
    struct InterruptState {
        using register_type = std::uint32_t;

        // in reversed order
        register_type ss, gs, fs, es, ds;

        register_type edi, esi, ebp, esp, ebx, edx, ecx, eax;

        // pushed by the CPU
        register_type interrupt;
        register_type error;
        register_type eip;
        register_type cs;
        register_type eflags;
    };
    static_assert(
        sizeof(InterruptState) == 18 *
        sizeof(InterruptState::register_type),
        "Invalid size of the interrupt routine structure"
    );

    using ISRHandler = void(*)(InterruptState* registers);

    void Init() noexcept;

    void RegisterHandler(std::uint8_t interrupt, ISRHandler handler) noexcept;
    void UnregisterHandler(std::uint8_t interrupt) noexcept;
}