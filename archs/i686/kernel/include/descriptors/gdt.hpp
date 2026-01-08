#pragma once

#include <init.hpp>

#include <cstdint>

namespace TSS {
    enum AccessType : std::uint8_t {
        AT_16BIT_TSS_AVAILABLE = 0b0001,
        AT_LDT                 = 0b0010,
        AT_16BIT_TSS_BUSY      = 0b0011,
        AT_32BIT_TSS_AVAILABLE = 0b1001,
        AT_32BIT_TSS_BUSY      = 0b1011,
#ifdef __NOS_64BIT__
        AT_64BIT_TSS_AVAILABLE = 0b1001,
        AT_64BIT_TSS_BUSY      = 0b1011,
#endif
    };
    
    struct PACKED Entry {
        std::uint32_t prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
        std::uint32_t esp0;       // The stack pointer to load when we change to kernel mode.
        std::uint32_t ss0;        // The stack segment to load when we change to kernel mode.
        std::uint32_t esp1;
        std::uint32_t ss1;
        std::uint32_t esp2;
        std::uint32_t ss2;
        std::uint32_t cr3;
        std::uint32_t eip;
        std::uint32_t eflags;
        std::uint32_t eax;
        std::uint32_t ecx;
        std::uint32_t edx;
        std::uint32_t ebx;
        std::uint32_t esp;
        std::uint32_t ebp;
        std::uint32_t esi;
        std::uint32_t edi;
        std::uint32_t es;
        std::uint32_t cs;
        std::uint32_t ss;
        std::uint32_t ds;
        std::uint32_t fs;
        std::uint32_t gs;
        std::uint32_t ldtr;
        std::uint16_t reserved;
        std::uint16_t iomap;
        std::uint32_t ssp;
    };
}

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
    
    struct PACKED Entry {
        std::uint16_t limit_lo;      // Lower 16 bits of the limit
        std::uint32_t base_lo : 24;  // Lower 24 bits of the base
        std::uint8_t access;         // Access flags, determine what ring this segment can be accessed from
        std::uint8_t limit_hi : 4;   // Upper 4 bits of the limit
        std::uint8_t flags : 4;      // Granularity and size
        std::uint8_t base_hi;        // Upper 8 bits of the base
    };

    struct PACKED Descriptor {
        std::uint16_t limit;    // sizeof(gdt) - 1
        const Entry* entry; // address of GDT
    };

    void Init() noexcept;
}