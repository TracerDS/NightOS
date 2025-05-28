#pragma once

#include <cstdint>

namespace ISR {
    struct ISR_RegistersState {
        using isr_register_type_t = std::uint32_t;

        // in reversed order
        isr_register_type_t ss, gs, fs, es, ds;

        isr_register_type_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

        // pushed by the CPU
        isr_register_type_t interrupt;
        isr_register_type_t error;
        isr_register_type_t eip;
        isr_register_type_t cs;
        isr_register_type_t eflags;
    };
    static_assert(sizeof(ISR_RegistersState) == 18 * 4, "None");

    using ISRHandler = void(*)();

    void ISR_Initialize();
    void ISR_RegisterHandler(std::uint8_t interrupt, ISRHandler handler);
}