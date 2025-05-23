#include <init.hpp>
#include <descriptors/gdt.hpp>
#include <descriptors/descriptors.hpp>
#include <cstdint>

extern "C" __attribute__((cdecl)) bool __kernel_load_GDT__ (
    const GDT::GDT_Descriptor* const descriptor
);

namespace GDT {
    using Descriptors::PrivilegeType;

    constexpr GDT_Entry GDT_CreateEntry (
        const uint32_t base,
        const uint32_t limit,
        const PrivilegeType privilegeType,
        const uint8_t accessType,
        const uint8_t flags
    ) noexcept {
        GDT_Entry entry = {};

        // Set the limit's low and high parts
        entry.limit_lo = limit & 0xFFFF;          // Lower 16 bits of limit
        entry.limit_hi = (limit >> 16) & 0x0F;    // Upper 4 bits of limit

        // Set the base's low, middle, and high parts
        entry.base_lo = base & 0xFFFFFF;          // Lower 24 bits of base
        entry.base_hi = (base >> 24) & 0xFF;      // Upper 8 bits of base

        // Set the access and flags bytes
        entry.access = (privilegeType << 5) | accessType;
        entry.flags = flags & 0x0F;               // Only use the lower 4 bits of flags

        return entry;
    }

    const GDT_Entry g_GDT[] = {
         // null descriptor
        GDT_CreateEntry(0, 0, PrivilegeType::PT_KERNEL, 0, 0),

        // Kernel 32-bit code segment
        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_EXECUTABLE | AccessType::AT_NON_CONFORMING
                | AccessType::AT_READABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ),

        // Kernel 32-bit data segment
        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_NOT_EXECUTABLE | AccessType::AT_DIRECTION_UP
                | AccessType::AT_WRITEABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ),
    };
    static_assert(sizeof(g_GDT) / sizeof(GDT_Entry) > 1, "Not defined any GDT entries!");

    std::uint8_t g_CodeSegmentOffset = 0x8 * 1;
    std::uint8_t g_DataSegmentOffset = 0x8 * 2;

    const GDT_Descriptor g_GDTDescriptor = {
        .limit = sizeof(g_GDT) - 1,
        .entry = g_GDT
    };

    bool GDT_Initialize() noexcept {
        return __kernel_load_GDT__(&g_GDTDescriptor);
    }
}