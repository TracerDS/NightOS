#include <cpuid.hpp>

namespace CPUID {
    extern "C" CPUID::CPUID_Data __kernel_call_cpuid__(std::uint32_t eax, std::uint32_t ecx) noexcept;

    CPUID_Data get_cpuid(std::uint32_t eax, std::uint32_t ecx) noexcept
    {
        return __kernel_call_cpuid__(eax, ecx);
    }

    
}