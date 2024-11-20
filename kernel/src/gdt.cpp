#include <cpp.hpp>
#include <gdt.hpp>
#include <stdint.h>

__CPP_START__

extern bool _loadGDT(const GDT::GDT_Descriptor* descriptor);

namespace GDT {
    constexpr GDT_Entry GDT_CreateEntry(const uint32_t base, const uint32_t limit, const uint8_t access, const uint8_t flags) noexcept {
        GDT_Entry entry = {};

        // Set the limit's low and high parts
        entry.limit_lo = limit & 0xFFFF;          // Lower 16 bits of limit
        entry.limit_hi = (limit >> 16) & 0x0F;    // Upper 4 bits of limit

        // Set the base's low, middle, and high parts
        entry.base_lo = base & 0xFFFFFF;          // Lower 24 bits of base
        entry.base_hi = (base >> 24) & 0xFF;      // Upper 8 bits of base

        // Set the access and flags bytes
        entry.access = access;
        entry.flags = flags & 0x0F;               // Only use the lower 4 bits of flags

        return entry;
    }

    constexpr GDT_Entry g_GDT[] = {
        GDT_CreateEntry(0, 0, AT_INVALID_SEGMENT, 0), // null descriptor
        
#ifdef __NOS_KERNEL_GDT_INCLUDE_16BIT_SEGMENT__
        GDT_CreateEntry(0, 0x0FFFF,
            AT_VALID_SEGMENT | PT_KERNEL | AT_NONSYSTEM_SEGMENT | AT_EXECUTABLE | AT_READABLE | AT_ACCESSED,
            F_GRANULARITY_BYTE | F_SIZE_16BIT | F_32BIT_MODE
        ), // Kernel 16-bit code segment

        GDT_CreateEntry(0, 0x0FFFF,
            AT_VALID_SEGMENT | PT_KERNEL | AT_NONSYSTEM_SEGMENT | AT_NOT_EXECUTABLE | AT_WRITEABLE | AT_ACCESSED,
            F_GRANULARITY_BYTE | F_SIZE_16BIT | F_32BIT_MODE
        ), // Kernel 16-bit data segment
#endif

#ifndef __NOS_KERNEL_GDT_NOT_INCLUDE_32BIT_SEGMENT__
        GDT_CreateEntry(0, 0xFFFFF,
            AT_VALID_SEGMENT | PT_KERNEL | AT_NONSYSTEM_SEGMENT | AT_EXECUTABLE | AT_READABLE | AT_ACCESSED,
            F_GRANULARITY_PAGE | F_SIZE_32BIT | F_32BIT_MODE
        ), // Kernel 32-bit code segment

        GDT_CreateEntry(0, 0xFFFFF,
            AT_VALID_SEGMENT | PT_KERNEL | AT_NONSYSTEM_SEGMENT | AT_NOT_EXECUTABLE | AT_WRITEABLE | AT_ACCESSED,
            F_GRANULARITY_PAGE | F_SIZE_32BIT | F_32BIT_MODE
        ), // Kernel 32-bit data segment
#endif

#ifdef __NOS_KERNEL_GDT_INCLUDE_64BIT_SEGMENT__
        GDT_CreateEntry(0, 0xFFFFF,
            AT_VALID_SEGMENT | PT_KERNEL | AT_NONSYSTEM_SEGMENT | AT_EXECUTABLE | AT_READABLE | AT_ACCESSED,
            F_GRANULARITY_PAGE | F_SIZE_16BIT | F_64BIT_MODE
        ), // Kernel 64-bit code segment

        GDT_CreateEntry(0, 0xFFFFF,
            AT_VALID_SEGMENT | PT_KERNEL | AT_NONSYSTEM_SEGMENT | AT_NOT_EXECUTABLE | AT_WRITEABLE | AT_ACCESSED,
            F_GRANULARITY_PAGE | F_SIZE_16BIT | F_64BIT_MODE
        ), // Kernel 64-bit data segment
        
        GDT_CreateEntry(0, 0xFFFFF,
            AT_VALID_SEGMENT | PT_USER | AT_NONSYSTEM_SEGMENT | AT_EXECUTABLE | AT_READABLE | AT_ACCESSED,
            F_GRANULARITY_PAGE | F_64BIT_MODE
        ), // Usermode 64-bit code segment

        GDT_CreateEntry(0, 0xFFFFF,
            AT_VALID_SEGMENT | PT_USER | AT_NONSYSTEM_SEGMENT | AT_NOT_EXECUTABLE | AT_WRITEABLE | AT_ACCESSED,
            F_GRANULARITY_PAGE | F_64BIT_MODE
        ), // Usermode 64-bit data segment
#endif
    };

    static_assert(sizeof(g_GDT) / sizeof(GDT_Entry) > 1, "Not defined any GDT entries!");

    constexpr GDT_Descriptor g_GDTDescriptor = {
        .limit = sizeof(g_GDT) - 1,
        .entry = g_GDT
    };
    
    bool GDT_Initialize() noexcept {
        return _loadGDT(&g_GDTDescriptor);
    }

    const GDT_Descriptor* GDT_GetDescriptor() noexcept {
        return &g_GDTDescriptor;
    }
}

__CPP_END__