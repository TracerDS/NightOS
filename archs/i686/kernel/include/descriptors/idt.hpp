#pragma once

#include <init.hpp>
#include <descriptors/descriptors.hpp>
#include <klibc/array>

#include <cstdint>

namespace IDT {
    class IDT {
    public:
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

    public:
        void init() noexcept;

        void set_entry (
            std::uint8_t interrupt,
            std::uint16_t segment,
            void* base,
            std::uint8_t flags
        ) noexcept;
        
        void enable_entry(std::uint8_t interrupt) noexcept;
        void disable_entry(std::uint8_t interrupt) noexcept;
    private:
        klibc::array<Entry, Descriptors::INTERRUPT_COUNT> m_entries;

        Descriptor m_descriptor;
    };

    extern IDT g_IDT;
}