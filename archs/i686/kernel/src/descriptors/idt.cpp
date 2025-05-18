#pragma once

#include <descriptors/idt.hpp>

extern "C" __attribute__((cdecl)) bool __kernel_load_IDT__ (
    const IDT::IDT_Descriptor* const descriptor
);

namespace IDT {
    IDT_Entry g_IDT[256];
    
    const IDT_Descriptor g_IDTDescriptor = {
        .limit = sizeof(g_GDT) - 1,
        .entries = g_IDT
    };
    
    bool IDT_Initialize() noexcept {
        return __kernel_load_IDT__(&g_IDTDescriptor);
    }
    
    void IDT_SetEntry (
        std::uint8_t interrupt,
        std::uint16_t segment,
        void* base,
        std::uint8_t flags
    ) noexcept {
        g_IDT[interrupt].base_lo = reinterpret_cast<std::uint32_t>(base) & 0xFFFF;
        g_IDT[interrupt].base_hi = (reinterpret_cast<std::uint32_t>(base) >> 16) & 0xFFFF;
        g_IDT[interrupt].segment = segment;
        g_IDT[interrupt].reserved = 0;
        g_IDT[interrupt].flags = flags;
    }
}