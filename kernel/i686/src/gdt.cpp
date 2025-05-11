#include <init.hpp>
#include <gdt.hpp>
#include <stdint.h>

__CPP_START__

__attribute__((cdecl)) extern bool __kernel_loadGDT__(const GDT::GDT_Descriptor* descriptor);

namespace GDT {
    constexpr GDT_Entry GDT_CreateEntry(
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
        entry.access = privilegeType | accessType;
        entry.flags = flags & 0x0F;               // Only use the lower 4 bits of flags

        return entry;
    }

    const GDT_Entry g_GDT[] = {
        GDT_CreateEntry(0, 0, PT_KERNEL, 0, 0), // null descriptor
        
#ifdef __NOS_KERNEL_GDT_INCLUDE_16BIT_SEGMENT__
        GDT_CreateEntry(0, 0xFFFFF,
            AT_VALID_SEGMENT | PT_KERNEL | AT_NONSYSTEM_SEGMENT | AT_EXECUTABLE | AT_READABLE | AT_ACCESSED,
            F_GRANULARITY_BYTE | F_SIZE_16BIT | F_32BIT_MODE
        ), // Kernel 16-bit code segment

        GDT_CreateEntry(0, 0xFFFFF,
            AT_VALID_SEGMENT | PT_KERNEL | AT_NONSYSTEM_SEGMENT | AT_NOT_EXECUTABLE | AT_WRITEABLE | AT_ACCESSED,
            F_GRANULARITY_BYTE | F_SIZE_16BIT | F_32BIT_MODE
        ), // Kernel 16-bit data segment
#endif

#ifdef __NOS_KERNEL_GDT_INCLUDE_32BIT_SEGMENT__
        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_EXECUTABLE | AccessType::AT_NON_CONFORMING
                | AccessType::AT_READABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ), // Kernel 32-bit code segment

        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_NOT_EXECUTABLE | AccessType::AT_DIRECTION_UP
                | AccessType::AT_WRITEABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ), // Kernel 32-bit data segment
#endif

#ifdef __NOS_KERNEL_GDT_INCLUDE_64BIT_SEGMENT__
        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_EXECUTABLE | AccessType::AT_NON_CONFORMING
                | AccessType::AT_READABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_16BIT | Flags::F_64BIT_MODE
        ), // Kernel 64-bit code segment

        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_NOT_EXECUTABLE | AccessType::AT_DIRECTION_UP
                | AccessType::AT_WRITEABLE | AccessType::AT_ACCESSED,
                Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_16BIT | Flags::F_64BIT_MODE
        ), // Kernel 64-bit data segment
        
        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_USER,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_EXECUTABLE | AccessType::AT_NON_CONFORMING
                | AccessType::AT_READABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_16BIT | Flags::F_64BIT_MODE
        ), // Usermode 64-bit code segment

        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_USER,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_NOT_EXECUTABLE | AccessType::AT_DIRECTION_UP
                | AccessType::AT_WRITEABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_16BIT | Flags::F_64BIT_MODE
        ), // Usermode 64-bit data segment
#endif
    };

    static_assert(sizeof(g_GDT) / sizeof(GDT_Entry) > 1, "Not defined any GDT entries!");

    const GDT_Descriptor g_GDTDescriptor = {
        .limit = sizeof(g_GDT) - 1,
        .entry = g_GDT
    };

    const GDT_Entry* GDT_GetEntries() noexcept {
        return g_GDT;
    }
    
    bool GDT_Initialize() noexcept {
        auto var = &g_GDTDescriptor;
        return __kernel_loadGDT__(var);
    }

    const GDT_Descriptor* GDT_GetCurrentDescriptor() noexcept {
        return &g_GDTDescriptor;
    }
}

__CPP_END__