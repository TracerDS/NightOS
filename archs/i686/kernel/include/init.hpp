#ifndef __NOS_INIT_HPP__
#define __NOS_INIT_HPP__

#ifndef __cplusplus
#   error "Kernel must be compiled as a C++ target"
#endif

#ifdef _DEBUG
#   ifndef __KERNEL_DEBUG__
#      define __KERNEL_DEBUG__
#   endif
#endif

#ifdef __KERNEL_DEBUG__
#   define __NOS_SERIAL_DEBUG__
#endif

static_assert(sizeof(void*) == 4, "Kernel must be compiled for 32-bit architecture");

#include <concepts>
#include <cstdint>

namespace Utils {
    constexpr auto align_up(std::integral auto value, std::integral auto alignment) noexcept {
        return (value + alignment - 1) & ~(alignment - 1);
    }

    template <typename Type>
    constexpr auto align_up(Type* value, std::integral auto alignment) noexcept {
        return align_up(reinterpret_cast<std::uintptr_t>(value), alignment);
    }

    constexpr auto align_down(std::integral auto value, std::integral auto alignment) noexcept {
        return value & ~(alignment - 1);
    }

    template <typename Type>
    constexpr auto align_down(Type* value, std::integral auto alignment) noexcept {
        return align_down(reinterpret_cast<std::uintptr_t>(value), alignment);
    }

    template <std::integral T>
    constexpr T ceil(T value, T divisor) noexcept {
        if (divisor == 0)
            return 0;

        return (value + divisor - 1) / divisor;
    }
    template <std::integral T>
    constexpr T floor(T value, T divisor) noexcept {
        if (divisor == 0)
            return 0;

        return value / divisor;
    }

    namespace Bits {
        template <typename T>
        constexpr void SetBit(
            T& value,
            std::uint8_t bit
        ) noexcept { value |= (1 << bit); }

        template <typename T>
        constexpr void ClearBit(
            T& value,
            std::uint8_t bit
        ) noexcept { value &= ~(1 << bit); }

        template <typename T>
        constexpr bool IsBitSet(
            T value,
            std::uint8_t bit
        ) noexcept { return (value & (1 << bit)) != 0; }
    }
}
#endif