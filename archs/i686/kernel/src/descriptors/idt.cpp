#include <descriptors/idt.hpp>
#include <descriptors/gdt.hpp>

namespace IDT {
    extern "C" bool __kernel_load_IDT__ (
        const IDT::Descriptor* const descriptor
    );

    IDT g_IDT {};
    
    void IDT::init() noexcept {
        m_descriptor = {
            .limit = static_cast<std::uint16_t>(m_entries.size() - 1),
            .entries = m_entries.data()
        };
        __kernel_load_IDT__(&m_descriptor);
    }
    
    void IDT::set_entry (
        std::uint8_t interrupt,
        std::uint16_t segment,
        void* base,
        std::uint8_t flags
    ) noexcept {
        auto baseAddr = reinterpret_cast<std::uint32_t>(base);
        m_entries[interrupt].base_lo = baseAddr & 0xFFFF;
        m_entries[interrupt].base_hi = (baseAddr >> 16) & 0xFFFF;
        m_entries[interrupt].segment = segment;
        m_entries[interrupt].reserved = 0;
        m_entries[interrupt].flags = flags;
    }

    void IDT::enable_entry(std::uint8_t interrupt) noexcept {
        Utils::Bits::SetBitMask(
            m_entries[interrupt].flags,
            GDT::AccessType::AT_VALID_SEGMENT
        );
    }
    void IDT::disable_entry(std::uint8_t interrupt) noexcept {
        Utils::Bits::ClearBitMask(
            m_entries[interrupt].flags,
            GDT::AccessType::AT_VALID_SEGMENT
        );
    }
}