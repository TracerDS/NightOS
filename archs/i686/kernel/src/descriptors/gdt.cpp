#include <init.hpp>
#include <descriptors/gdt.hpp>
#include <descriptors/descriptors.hpp>
#include <cstdint>

extern "C" __attribute__((cdecl))
bool __kernel_load_GDT__(const GDT::GDT_Descriptor* const descriptor) noexcept;

extern "C" __attribute__((cdecl))
bool __kernel_load_TSS__(const std::uint16_t selector) noexcept;

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

    GDT_Entry g_GDT[] = {
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

        // User 32-bit code segment
        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_USER,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT | 
                AccessType::AT_EXECUTABLE | AccessType::AT_NON_CONFORMING
                | AccessType::AT_READABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ),
        
        // User 32-bit data segment
        GDT_CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_USER,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_NOT_EXECUTABLE | AccessType::AT_DIRECTION_UP
                | AccessType::AT_WRITEABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ),

        // Task State Segment (TSS)
        GDT_CreateEntry(0, 0,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_SYSTEM_SEGMENT
                | TSS::AccessType::AT_32BIT_TSS_AVAILABLE,
            Flags::F_GRANULARITY_BYTE
        )
    };
    static_assert(sizeof(g_GDT) / sizeof(GDT_Entry) > 1, "Not defined any GDT entries!");

    std::uint8_t g_CodeSegmentOffset = 0x8 * 1;
    std::uint8_t g_DataSegmentOffset = 0x8 * 2;
    std::uint8_t g_TSSSegmentOffset  = 0x8 * 5;

    const GDT_Descriptor g_GDTDescriptor = {
        .limit = sizeof(g_GDT) - 1,
        .entry = g_GDT
    };

    TSS::TSS_Entry g_TSS{};

    bool GDT_Initialize() noexcept {
        g_GDT[5] = GDT_CreateEntry(
            reinterpret_cast<std::uint32_t>(&g_TSS),
            sizeof(TSS::TSS_Entry) - 1,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_SYSTEM_SEGMENT
                | TSS::AccessType::AT_32BIT_TSS_AVAILABLE,
            Flags::F_GRANULARITY_BYTE
        );

        // 2. Configure critical TSS fields
        // SS0 is the Stack Segment for Ring 0. When an interrupt happens in Ring 3,
        // the CPU switches to this segment. Must match Kernel Data Segment (0x10).
        g_TSS.ss0 = g_DataSegmentOffset;
        
        // ESP0 needs to point to the top of the Kernel Stack. 
        // For now, we leave it as 0 or set it to a boot stack if available.
        // IMPORTANT: The Task Scheduler must update this field every time it switches tasks!
        g_TSS.esp0 = 0; 

        // Disable I/O Permission Bitmap by pointing it outside the TSS limit
        g_TSS.iomap = sizeof(TSS::TSS_Entry);

        if (!__kernel_load_GDT__(&g_GDTDescriptor))
            return false;
            
        return __kernel_load_TSS__(g_TSSSegmentOffset);
    }
}