#pragma once

#include <cstdint>
#include <descriptors/descriptors.hpp>

namespace IDT {
    enum class IDT_GateType : std::uint8_t {
        GATE_TASK = 0b0101,
        GATE_INT16 = 0b0110,
        GATE_TRAP16 = 0b0111,
        GATE_INT32 = 0b1110,
        GATE_TRAP32 = 0b1111,
    };

    struct IDT_Entry {
        std::uint16_t base_lo;
        std::uint16_t segment;
        std::uint8_t reserved;
        std::uint8_t flags;
        std::uint16_t base_hi;
    } __attribute__((packed));

    struct IDT_Descriptor {
        std::uint16_t limit;
        const IDT_Entry* entries;
    } __attribute__((packed));
    
    bool IDT_Initialize() noexcept;
    void IDT_SetEntry (
        std::uint8_t interrupt,
        std::uint16_t segment,
        void* base,
        std::uint8_t flags
    ) noexcept;
    void IDT_EnableEntry(std::uint8_t interrupt) noexcept;
    void IDT_DisableEntry(std::uint8_t interrupt) noexcept;
}