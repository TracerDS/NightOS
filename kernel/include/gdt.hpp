#pragma once

#include <cpp.hpp>
#include <stdint.h>

__CPP_START__

namespace GDT {    
    enum PrivilegeType {
        PT_KERNEL      = (0 << 5), // ring 0 (highest privilege).
        PT_DRIVER      = (1 << 5), // ring 1.
        PT_DRIVER_SAFE = (2 << 5), // ring 2.
        PT_USER        = (3 << 5), // ring 3 (lowest privilege).
    };

    enum AccessType {
        AT_VALID_SEGMENT     = (1 << 7), // Allows an entry to refer to a valid segment
        AT_INVALID_SEGMENT   = (0 << 7), // Prevents an entry from referring to a valid segment

        // PrivilegeType

        AT_NONSYSTEM_SEGMENT = (1 << 4), // Code or Data segment.
        AT_SYSTEM_SEGMENT    = (0 << 4), // System segment.

        AT_EXECUTABLE        = (1 << 3), // Code segment.
        AT_NOT_EXECUTABLE    = (0 << 3), // Data segment.

        AT_CONFORMING_SET    = (1 << 2), // Executable from an equal or lower privilege level.
        AT_CONFORMING_CLEAR  = (0 << 2), // Executable only from the ring set in DPL.

        AT_DIRECTION_DOWN    = (1 << 2), // Segment grows down (like a stack).
        AT_DIRECTION_UP      = (0 << 2), // Segment grows up.

        AT_READABLE          = (1 << 1), // Read access for this segment is allowed (always allowed for data segment).
        AT_NOT_READABLE      = (0 << 1), // Read access for this segment is not allowed (always allowed for data segment).

        AT_WRITEABLE         = (1 << 1), // Write access for this segment is allowed (never allowed for code segment).
        AT_NOT_WRITEABLE     = (0 << 1), // Write access for this segment is not allowed (never allowed for code segment).

        AT_ACCESSED          = (1 << 0),
        AT_NOT_ACCESSED      = (0 << 0),
    };

    enum Flags {
        F_GRANULARITY_PAGE = (1 << 3), // The Limit is in 4 KiB blocks.
        F_GRANULARITY_BYTE = (0 << 3), // The Limit is in 1 Byte blocks.

        F_SIZE_32BIT       = (1 << 2), // 32-bit segment.
        F_SIZE_16BIT       = (0 << 2), // 16-bit segment.

        F_64BIT_MODE       = (1 << 1), // 64-bit segment.
        F_32BIT_MODE       = (0 << 1), // Non 64bit segment.
    };

    struct GDT_Entry {
        uint16_t limit_lo;
        uint32_t base_lo:24;
        uint8_t access;
        uint8_t limit_hi:4;
        uint8_t flags:4;
        uint8_t base_hi;
    } __attribute__((packed));

    struct GDT_Descriptor
    {
        uint16_t limit; // sizeof(gdt) - 1
        const GDT_Entry* entry; // address of GDT
    } __attribute__((packed));

    bool GDT_Initialize() noexcept;
    const GDT_Descriptor* GDT_GetDescriptor() noexcept;
}
__CPP_END__