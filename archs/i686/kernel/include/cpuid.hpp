#pragma once

#include <cstdint>

namespace CPUID {
    struct CPUID_Data {
        std::uint32_t eax;
        std::uint32_t ebx;
        std::uint32_t ecx;
        std::uint32_t edx;
    };

    void GetVendor(char vendor[13]) noexcept;
    bool LAPIC_Supported() noexcept;
}