#include <cpuid.hpp>
#include <klibc/cstring>

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
        std::memcpy(vendor, &data.ebx, 4);
        std::memcpy(vendor + 4, &data.edx, 4);
        std::memcpy(vendor + 8, &data.ecx, 4);
    }
}