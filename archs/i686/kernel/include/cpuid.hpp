#pragma once

#include <cstdint>

namespace CPUID {
    struct CPUID_Data {
        std::uint32_t eax;
        std::uint32_t ebx;
        std::uint32_t ecx;
        std::uint32_t edx;
    };

    CPUID_Data get_cpuid(std::uint32_t eax, std::uint32_t ecx = 0) noexcept;
}