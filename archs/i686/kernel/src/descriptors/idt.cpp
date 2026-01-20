#include <descriptors/idt.hpp>
#include <descriptors/gdt.hpp>

namespace NOS::Interrupts::IDT {
    extern "C" bool __kernel_load_IDT__ (
        const Descriptor* const descriptor
    );
    
    klibc::array<Entry, Descriptors::INTERRUPT_COUNT> m_entries{};
    Descriptor m_descriptor;
    
    void Init() noexcept {
        m_descriptor = {
            .limit = static_cast<std::uint16_t>(
                m_entries.size() * sizeof(Entry) - 1
            ),
            .entries = m_entries.data()
        };
        __kernel_load_IDT__(&m_descriptor);
    }
    
    void SetEntry (
        std::uint8_t interrupt,
        std::uint16_t segment,
        void* base,
        std::uint8_t flags
    ) noexcept {
        auto baseAddr = reinterpret_cast<std::uintptr_t>(base);
        m_entries[interrupt].base_lo = baseAddr & 0xFFFF;
        m_entries[interrupt].base_hi = (baseAddr >> 16) & 0xFFFF;
        m_entries[interrupt].segment = segment;
        m_entries[interrupt].reserved = 0;
        m_entries[interrupt].flags = flags;
    }

    void EnableEntry(std::uint8_t interrupt) noexcept {
        Utils::Bits::SetBitMask(
            m_entries[interrupt].flags,
            Descriptors::GDT::AccessType::AT_VALID_SEGMENT
        );
    }
    void DisableEntry(std::uint8_t interrupt) noexcept {
        Utils::Bits::ClearBitMask(
            m_entries[interrupt].flags,
            Descriptors::GDT::AccessType::AT_VALID_SEGMENT
        );
    }
}