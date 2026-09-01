#pragma once

#include <core/init.hpp>

#include <type_traits>
#include <utility>

/**
* @namespace Bits
* @brief A utility namespace providing constexpr bit manipulation functions.
* 
* This namespace contains template functions for efficiently checking, setting,
* and clearing individual bits or bit masks at compile-time. All functions are
* constexpr and noexcept, making them suitable for use in constant expressions
* and kernel-level code.
*/
namespace NOS::Bits {
    /**
        * @brief Checks if any bits in the mask are set in the value.
        * @tparam T The type of the value being checked.
        * @tparam U The type of the mask.
        * @param value The value to check.
        * @param mask The bit mask to test against.
        * @return true if any bits in mask are set in value, false otherwise.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr bool is_set(
        T value,
        U mask
    ) noexcept { return (value & mask) != 0; }
    
    /**
        * @brief Checks if any bits in the mask are set in the value.
        * @tparam T The type of the value being checked.
        * @tparam U The type of the mask.
        * @param value The value to check.
        * @param mask The bit mask to test against as an enum.
        * @return true if any bits in mask are set in value, false otherwise.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr bool is_set(
        T value,
        U mask
    ) noexcept { return is_set(value, std::to_underlying(mask)); }

    /**
        * @brief Checks if a specific bit at position is set in the value.
        * @tparam T The type of the value being checked.
        * @tparam U The type representing the bit position.
        * @param value The value to check.
        * @param bits The bit position (0-indexed) to test.
        * @return true if the bit at position is set, false otherwise.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr bool is_bit_set(
        T value,
        U bits
    ) noexcept { return is_set(value, 1 << bits); }
    
    /**
        * @brief Checks if a specific bit at position is set in the value.
        * @tparam T The type of the value being checked.
        * @tparam U The type representing the bit position.
        * @param value The value to check.
        * @param bits The bit position (0-indexed) to test as an enum.
        * @return true if the bit at position is set, false otherwise.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr bool is_bit_set(
        T value,
        U mask
    ) noexcept { return is_bit_set(value, std::to_underlying(mask)); }

    /**
        * @brief Sets bits in value according to the provided mask (reference version).
        * @tparam T The type of the value to modify.
        * @tparam U The type of the mask.
        * @param value Reference to the value to modify.
        * @param mask The bit mask to set.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr void set (
        T& value,
        U mask
    ) noexcept { value |= mask; }

    /**
        * @brief Sets bits in value according to the provided mask (reference version).
        * @tparam T The type of the value to modify.
        * @tparam U The type of the mask.
        * @param value Reference to the value to modify.
        * @param mask The bit mask to set as an enum.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr void set (
        T& value,
        U mask
    ) noexcept { set(value, std::to_underlying(mask)); }

    /**
        * @brief Sets bits in value according to the provided mask (rvalue version).
        * @tparam T The type of the value to modify.
        * @tparam U The type of the mask.
        * @param value Rvalue reference to the value.
        * @param mask The bit mask to set.
        * @return A new value with the mask bits set.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr T set (
        T&& value,
        U mask
    ) noexcept { return value | mask; }

    /**
        * @brief Sets bits in value according to the provided mask (rvalue version).
        * @tparam T The type of the value to modify.
        * @tparam U The type of the mask.
        * @param value Rvalue reference to the value.
        * @param mask The bit mask to set as an enum.
        * @return A new value with the mask bits set.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr T set (
        T&& value,
        U mask
    ) noexcept { return set(std::forward<T>(value), std::to_underlying(mask)); }

    /**
        * @brief Sets a specific bit at the given position (reference version).
        * @tparam T The type of the value to modify.
        * @tparam U The type representing the bit position.
        * @param value Reference to the value to modify.
        * @param bits The bit position (0-indexed) to set.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr void set_bit (
        T& value,
        U bits
    ) noexcept { set(value, 1 << bits); }

    /**
        * @brief Sets a specific bit at the given position (reference version).
        * @tparam T The type of the value to modify.
        * @tparam U The type representing the bit position.
        * @param value Reference to the value to modify.
        * @param bits The bit position (0-indexed) to set as an enum.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr void set_bit (
        T& value,
        U bits
    ) noexcept { set_bit(value, std::to_underlying(bits)); }

    /**
        * @brief Sets a specific bit at the given position (rvalue version).
        * @tparam T The type of the value to modify.
        * @tparam U The type representing the bit position.
        * @param value Rvalue reference to the value.
        * @param bits The bit position (0-indexed) to set.
        * @return A new value with the bit set.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr T set_bit (
        T&& value,
        U bits
    ) noexcept { return set(std::forward<T>(value), 1 << bits); }
    
    /**
        * @brief Sets a specific bit at the given position (rvalue version).
        * @tparam T The type of the value to modify.
        * @tparam U The type representing the bit position.
        * @param value Rvalue reference to the value.
        * @param bits The bit position (0-indexed) to set as an enum.
        * @return A new value with the bit set.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr T set_bit (
        T&& value,
        U bits
    ) noexcept { return set_bit(std::forward<T>(value), std::to_underlying(bits)); }

    /**
        * @brief Clears bits in value according to the provided mask (reference version).
        * @tparam T The type of the value to modify.
        * @tparam U The type of the mask.
        * @param value Reference to the value to modify.
        * @param mask The bit mask to clear.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr void clear(
        T& value,
        U mask
    ) noexcept { value &= ~mask; }

    /**
        * @brief Clears bits in value according to the provided mask (reference version).
        * @tparam T The type of the value to modify.
        * @tparam U The type of the mask.
        * @param value Reference to the value to modify.
        * @param mask The bit mask to clear as an enum.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr void clear(
        T& value,
        U mask
    ) noexcept { clear(value, std::to_underlying(mask)); }

    /**
        * @brief Clears bits in value according to the provided mask (rvalue version).
        * @tparam T The type of the value to modify.
        * @tparam U The type of the mask.
        * @param value Rvalue reference to the value.
        * @param mask The bit mask to clear.
        * @return A new value with the mask bits cleared.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr T clear(
        T&& value,
        U mask
    ) noexcept { return value & ~mask; }
    
    /**
        * @brief Clears bits in value according to the provided mask (rvalue version).
        * @tparam T The type of the value to modify.
        * @tparam U The type of the mask.
        * @param value Rvalue reference to the value.
        * @param mask The bit mask to clear as an enum.
        * @return A new value with the mask bits cleared.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr T clear(
        T&& value,
        U mask
    ) noexcept { return clear(std::forward<T>(value), std::to_underlying(mask)); }
    
    /**
        * @brief Clears a specific bit at the given position (reference version).
        * @tparam T The type of the value to modify.
        * @tparam U The type representing the bit position.
        * @param value Reference to the value to modify.
        * @param bits The bit position (0-indexed) to clear.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr void clear_bit(
        T& value,
        U bits
    ) noexcept { clear(value, 1 << bits); }
    
    /**
        * @brief Clears a specific bit at the given position (reference version).
        * @tparam T The type of the value to modify.
        * @tparam U The type representing the bit position.
        * @param value Reference to the value to modify.
        * @param bits The bit position (0-indexed) to clear as an enum.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr void clear_bit(
        T& value,
        U bits
    ) noexcept { clear_bit(value, std::to_underlying(bits)); }

    /**
        * @brief Clears a specific bit at the given position (rvalue version).
        * @tparam T The type of the value to modify.
        * @tparam U The type representing the bit position.
        * @param value Rvalue reference to the value.
        * @param bits The bit position (0-indexed) to clear.
        * @return A new value with the bit cleared.
        */
    template <typename T, typename U> requires (!std::is_enum_v<U> && std::is_integral_v<U>)
    FORCE_INLINE constexpr T clear_bit(
        T&& value,
        U bits
    ) noexcept { return clear(std::forward<T>(value), 1 << bits); }
    
    /**
        * @brief Clears a specific bit at the given position (rvalue version).
        * @tparam T The type of the value to modify.
        * @tparam U The type representing the bit position.
        * @param value Rvalue reference to the value.
        * @param bits The bit position (0-indexed) to clear as an enum.
        * @return A new value with the bit cleared.
        */
    template <typename T, typename U> requires (std::is_enum_v<U>)
    FORCE_INLINE constexpr T clear_bit(
        T&& value,
        U bits
    ) noexcept { return clear_bit(std::forward<T>(value), std::to_underlying(bits)); }
}