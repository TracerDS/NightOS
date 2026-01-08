#pragma once

#include <init.hpp>

#include <descriptors/descriptors.hpp>
#include <klibc/array>

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
    static_assert(
        sizeof(ISR_RegistersState) == 18 *
        sizeof(ISR_RegistersState::isr_register_type_t),
        "Invalid size of the interrupt routine structure"
    );

    using ISRHandler = void(*)(ISR_RegistersState* registers);

    class ISR {
    public:
        void init();

        void register_handler(std::uint8_t interrupt, ISRHandler handler) noexcept;
        void unregister_handler(std::uint8_t interrupt) noexcept;
        bool has_handler(std::uint8_t interrupt) const noexcept;

        ISRHandler get_handler(std::uint8_t interrupt) const noexcept;

        void operator()(std::uint8_t interrupt, ISR_RegistersState* regs) noexcept;
    private:
        static void default_handler(ISR_RegistersState* regs) noexcept;
    private:
        klibc::array<ISRHandler, Descriptors::INTERRUPT_COUNT> m_handlers;
    };

    extern ISR g_ISR;
}