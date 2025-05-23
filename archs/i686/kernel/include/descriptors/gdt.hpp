#pragma once

#include <init.hpp>

#include <cstdint>

namespace GDT {    
    enum AccessType : std::uint8_t {
        AT_VALID_SEGMENT    = 1 << 7,
        AT_INVALID_SEGMENT  = 0 << 7,

        // PrivilegeType

        AT_NONSYSTEM_SEGMENT = 1 << 4, // Code or Data segment.
        AT_SYSTEM_SEGMENT    = 0 << 4, // System segment.

        AT_EXECUTABLE       = 1 << 3, // Code segment.
        AT_NOT_EXECUTABLE   = 0 << 3, // Data segment.

        // for data segments
        AT_DIRECTION_DOWN   = 1 << 2, // Segment grows down (like a stack).
        AT_DIRECTION_UP     = 0 << 2, // Segment grows up.

        // for code segments
        AT_CONFORMING_SET   = 1 << 2, // Executable from an equal or lower privilege level.
        AT_NON_CONFORMING   = 0 << 2, // Executable only from the ring set in DPL.

        // for data segments
        AT_WRITEABLE        = 1 << 1, // Write access for this segment is allowed
        AT_NOT_WRITEABLE    = 0 << 1, // Write access for this segment is not allowed

        // for code segments
        AT_READABLE     = 1 << 1, // Read access for this segment is allowed
        AT_NOT_READABLE = 0 << 1, // Read access for this segment is not allowed

        AT_ACCESSED     = 1 << 0,
        AT_NOT_ACCESSED = 0 << 0,
    };

    enum Flags : std::uint8_t {
        F_GRANULARITY_PAGE  = 1 << 3, // The Limit is in 4 KiB blocks.
        F_GRANULARITY_BYTE  = 0 << 3, // The Limit is in 1 Byte blocks.

        F_SIZE_32BIT = 1 << 2, // 32-bit segment.
        F_SIZE_16BIT = 0 << 2, // 16-bit segment.

        F_64BIT_MODE = 1 << 1, // 64-bit segment.
        F_32BIT_MODE = 0 << 1, // Non 64bit segment.
    };

    struct GDT_Entry {
        std::uint16_t limit_lo;      // Lower 16 bits of the limit
        std::uint32_t base_lo : 24;  // Lower 24 bits of the base
        std::uint8_t access;         // Access flags, determine what ring this segment can be accessed from
        std::uint8_t limit_hi : 4;   // Upper 4 bits of the limit
        std::uint8_t flags : 4;      // Granularity and size
        std::uint8_t base_hi;        // Upper 8 bits of the base
    } __attribute__((packed));

    struct GDT_Descriptor {
        std::uint16_t limit;         // sizeof(gdt) - 1
        const GDT_Entry* entry; // address of GDT
    } __attribute__((packed));
    
    bool GDT_Initialize() noexcept;
    
    extern std::uint8_t g_CodeSegmentOffset;
    extern std::uint8_t g_DataSegmentOffset;

}