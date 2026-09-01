#pragma once

#include <core/init.hpp>

#include <type_traits>
#include <concepts>
#include <cstdint>

namespace NOS::Utils {
    template <typename T>
    concept is_cstring = std::is_same_v<std::remove_cvref_t<std::remove_pointer_t<T>>, char>;

    FORCE_INLINE constexpr auto align_up(
        std::integral auto value,
        std::integral auto alignment
    ) noexcept {
        return (value + alignment - 1) & ~(alignment - 1);
    }

    template <typename Type>
    FORCE_INLINE constexpr auto align_up(
        Type* value,
        std::integral auto alignment
    ) noexcept {
        return align_up(reinterpret_cast<std::uintptr_t>(value), alignment);
    }

    FORCE_INLINE constexpr auto align_down(
        std::integral auto value,
        std::integral auto alignment
    ) noexcept {
        return value & ~(alignment - 1);
    }

    template <typename Type>
    FORCE_INLINE constexpr auto align_down(
        Type* value,
        std::integral auto alignment
    ) noexcept {
        return align_down(reinterpret_cast<std::uintptr_t>(value), alignment);
    }

    FORCE_INLINE constexpr auto ceil(
        std::integral auto value,
        std::integral auto divisor
    ) noexcept {
        if (divisor == 0)
            return 0;

        return (value + divisor - 1) / divisor;
    }

    FORCE_INLINE constexpr auto floor(
        std::integral auto value,
        std::integral auto divisor
    ) noexcept {
        if (divisor == 0)
            return 0;

        return value / divisor;
    }
}