#include <cpuid.hpp>
#include <klibc/cstring>

namespace CPUID {
    extern "C" CPUID::CPUID_Data __kernel_call_cpuid__(
        std::uint32_t eax,
        std::uint32_t ecx
    ) noexcept;

    void GetVendor(char vendor[13]) noexcept
    {
        CPUID_Data data = __kernel_call_cpuid__(0, 0);
        klibc::memcpy(vendor, &data.ebx, 4);
        klibc::memcpy(vendor + 4, &data.edx, 4);
        klibc::memcpy(vendor + 8, &data.ecx, 4);
    }
    
    bool LAPIC_Supported() noexcept
    {
        CPUID_Data data = __kernel_call_cpuid__(1, 0);
        return (data.edx & (1 << 9)) != 0; // Check if bit 9 of EDX is set
    }
}