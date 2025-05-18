#pragma once

#include <cstdint>
#include <descriptors/descriptors.hpp>

namespace IDT {
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
}