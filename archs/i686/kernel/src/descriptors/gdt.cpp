#include <descriptors/gdt.hpp>
#include <descriptors/descriptors.hpp>
#include <klibc/array>

#include <cstdint>
#include <bit>

namespace TSS {
    Entry g_TSS {};
}

namespace GDT {
    extern "C" bool __kernel_load_GDT__(const Descriptor* const descriptor) noexcept;
    extern "C" bool __kernel_load_TSS__(const std::uint16_t selector) noexcept;

    using Descriptors::PrivilegeType;

    constexpr Entry CreateEntry (
        const std::uintptr_t base,
        const std::uintptr_t limit,
        const PrivilegeType privilegeType,
        const std::uint8_t accessType,
        const std::uint8_t flags
    ) noexcept {
        Entry entry;
        
        entry.limit_lo = limit & 0xFFFF;        // Lower 16 bits of limit
        entry.limit_hi = (limit >> 16) & 0x0F;  // Upper 4 bits of limit
        entry.base_lo = base & 0xFFFFFF;        // Lower 24 bits of base
        entry.base_hi = (base >> 24) & 0xFF;    // Upper 8 bits of base

        entry.access = (privilegeType << 5) | accessType;
        entry.flags = flags & 0x0F;             // Only use the lower 4 bits of flags
        
        return entry;
    }

    klibc::array g_GDT = {
        // null descriptor
        CreateEntry(0, 0, PrivilegeType::PT_KERNEL, 0, 0),

        // Kernel 32-bit code segment
        CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_EXECUTABLE | AccessType::AT_NON_CONFORMING
                | AccessType::AT_READABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ),

        // Kernel 32-bit data segment
        CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_NOT_EXECUTABLE | AccessType::AT_DIRECTION_UP
                | AccessType::AT_WRITEABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ),

        // User 32-bit code segment
        CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_USER,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT | 
                AccessType::AT_EXECUTABLE | AccessType::AT_NON_CONFORMING
                | AccessType::AT_READABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ),
        
        // User 32-bit data segment
        CreateEntry(0, 0xFFFFF,
            PrivilegeType::PT_USER,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_NONSYSTEM_SEGMENT
                | AccessType::AT_NOT_EXECUTABLE | AccessType::AT_DIRECTION_UP
                | AccessType::AT_WRITEABLE | AccessType::AT_ACCESSED,
            Flags::F_GRANULARITY_PAGE | Flags::F_SIZE_32BIT | Flags::F_32BIT_MODE
        ),

        // Task State Segment (TSS)
        CreateEntry(
            std::bit_cast<std::uintptr_t>(&TSS::g_TSS),
            sizeof(TSS::Entry) - 1,
            PrivilegeType::PT_KERNEL,
            AccessType::AT_VALID_SEGMENT | AccessType::AT_SYSTEM_SEGMENT
                | TSS::AccessType::AT_32BIT_TSS_AVAILABLE,
            Flags::F_GRANULARITY_BYTE
        ),
    };
    static_assert(g_GDT.size() > 1, "Not defined any GDT entries!");

    std::uint8_t g_codeSegmentOffset = 0x8 * 1;
    std::uint8_t g_dataSegmentOffset = 0x8 * 2;
    std::uint8_t g_tssSegmentOffset  = 0x8 * (g_GDT.size() - 1);

    const GDT::Descriptor g_GDTDescriptor = {
        .limit = sizeof(g_GDT) - 1,
        .entry = &g_GDT[0]
    };

    void Init() noexcept {
        // 2. Configure critical TSS fields
        // SS0 is the Stack Segment for Ring 0. When an interrupt happens in Ring 3,
        // the CPU switches to this segment. Must match Kernel Data Segment (0x10).
        TSS::g_TSS.ss0 = g_dataSegmentOffset;
        
        // ESP0 needs to point to the top of the Kernel Stack. 
        // For now, we leave it as 0 or set it to a boot stack if available.
        // IMPORTANT: The Task Scheduler must update this field every time it switches tasks!
        TSS::g_TSS.esp0 = 0;

        // Disable I/O Permission Bitmap by pointing it outside the TSS limit
        TSS::g_TSS.iomap = sizeof(Entry);

        __kernel_load_GDT__(&g_GDTDescriptor);
        __kernel_load_TSS__(g_tssSegmentOffset);
    }
}