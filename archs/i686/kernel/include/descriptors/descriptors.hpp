#pragma once

#include <cstdint>

namespace Descriptors {
    enum PrivilegeType : std::uint8_t {
        PT_KERNEL       = 0b00, // Kernel mode
        PT_DRIVER       = 0b01, // kernel driver mode
        PT_DRIVER_SAFE  = 0b10, // kernel user mode 
        PT_USER         = 0b11  // User mode
    };

    struct SegmentSelector {
        std::uint16_t segment : 13;
        std::uint8_t reserved : 1;
        PrivilegeType privilegeLevel : 2;
    } __attribute__((packed));

    static_assert(sizeof(SegmentSelector) == 2, "SegmentSelector size mismatch");

    constexpr std::uint16_t INTERRUPT_COUNT = 256;
}