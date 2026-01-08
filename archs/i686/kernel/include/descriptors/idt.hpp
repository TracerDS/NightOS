#pragma once

#include <init.hpp>
#include <descriptors/descriptors.hpp>
#include <klibc/array>

#include <cstdint>

namespace NOS::Interrupts::IDT {
    enum class GateType : std::uint8_t {
        GATE_TASK = 0b0101,
        GATE_INT16 = 0b0110,
        GATE_TRAP16 = 0b0111,
        GATE_INT32 = 0b1110,
        GATE_TRAP32 = 0b1111,
    };

    struct PACKED Entry {
        std::uint16_t base_lo;
        std::uint16_t segment;
        std::uint8_t reserved;
        std::uint8_t flags;
        std::uint16_t base_hi;
    };

    struct PACKED Descriptor {
        std::uint16_t limit;
        const Entry* entries;
    };

    void Init() noexcept;

    void SetEntry (
        std::uint8_t interrupt,
        std::uint16_t segment,
        void* base,
        std::uint8_t flags
    ) noexcept;
    
    void EnableEntry(std::uint8_t interrupt) noexcept;
    void DisableEntry(std::uint8_t interrupt) noexcept;
}