#include <cpuid.hpp>
#include <klibc/string.h>

namespace CPUID {
    extern "C" CPUID::CPUID_Data __kernel_call_cpuid__(std::uint32_t eax, std::uint32_t ecx) noexcept;

    CPUID_Data get_cpuid(std::uint32_t eax, std::uint32_t ecx) noexcept
    {
        return __kernel_call_cpuid__(eax, ecx);
    }

    void GetVendor(char vendor[13]) noexcept
    {
        vendor[12] = 0;
        CPUID_Data data = get_cpuid(0, 0);
        memcpy(vendor, &data.ebx, 4);
        memcpy(vendor + 4, &data.edx, 4);
        memcpy(vendor + 8, &data.ecx, 4);
    }
    
    bool LAPIC_Supported() noexcept
    {
        CPUID_Data data = get_cpuid(1, 0);
        return (data.edx & (1 << 9)) != 0; // Check if bit 9 of EDX is set
    }
}