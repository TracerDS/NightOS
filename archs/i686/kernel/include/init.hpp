#ifndef __NOS_INIT_HPP__
#define __NOS_INIT_HPP__

#ifndef __cplusplus
#   error "Kernel must be compiled as a C++ target"
#endif

static_assert(sizeof(void*) == 4, "Kernel must be compiled for 32-bit architecture");

#ifdef _DEBUG
#   ifndef __NOS_KERNEL_DEBUG__
#      define __NOS_KERNEL_DEBUG__
#   endif
#endif

#ifdef __NOS_KERNEL_DEBUG__
#   define __NOS_SERIAL_DEBUG__
#endif

#ifdef __clang__
#   define __NOS_KERNEL_COMPILER_CLANG__
#elif defined(__GNUC__) || defined(__GNUG__)
#   define __NOS_KERNEL_COMPILER_GCC__
#endif

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#   define FORCE_INLINE [[gnu::always_inline]] inline
#   define NO_INLINE    [[gnu::noinline]]
#   define PACKED       [[gnu::packed]]
#   define ATTRIBUTE(x) [[gnu::x]]
#else
#   error "Unsupported compiler"
#endif

#include <descriptors/kernel_interrupts.hpp>

#include <concepts>
#include <cstdint>
#include <utility>

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

    /**
     * @namespace Bits
     * @brief A utility namespace providing constexpr bit manipulation functions.
     * 
     * This namespace contains template functions for efficiently checking, setting,
     * and clearing individual bits or bit masks at compile-time. All functions are
     * constexpr and noexcept, making them suitable for use in constant expressions
     * and kernel-level code.
     */
    namespace Bits {
        /**
         * @brief Checks if any bits in the mask are set in the value.
         * @tparam T The type of the value being checked.
         * @tparam U The type of the mask.
         * @param value The value to check.
         * @param mask The bit mask to test against.
         * @return true if any bits in mask are set in value, false otherwise.
         */
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
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
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
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
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
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
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
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
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
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
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
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
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
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
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
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
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
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
        template <typename T, typename U>
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
        template <typename T, typename U> requires std::is_enum_v<U>
        FORCE_INLINE constexpr T clear_bit(
            T&& value,
            U bits
        ) noexcept { return clear_bit(std::forward<T>(value), std::to_underlying(bits)); }
    }
    
    namespace Asm {
        template <std::uint8_t IntNum>
        FORCE_INLINE void CallInterrupt() noexcept {
            asm volatile (
                "int %0"
                :
                : "i"(IntNum)
                : "memory", "cc"
            );
        }
        FORCE_INLINE void KernelPanic() noexcept {
            CallInterrupt<Interrupts::KernelInterrupts::KERNEL_PANIC>();
        }
    }

    template <typename T>
    struct array_view {
        using value_type = std::remove_pointer_t<std::remove_cvref_t<T>>;
        using pointer = std::add_pointer_t<value_type>;
        using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
    public:
        constexpr array_view() noexcept : array_view(nullptr, 0) {}
        constexpr array_view(std::nullptr_t) noexcept : array_view(nullptr, 0) {}
        constexpr array_view(pointer data, std::size_t size) noexcept
            : m_data{data}, m_size{size} {}

        constexpr std::size_t size() const noexcept { return m_size; }
        constexpr T& operator[](std::size_t index) noexcept { return m_data[index]; }
        constexpr const T& operator[](std::size_t index) const noexcept { return m_data[index]; }
        
        constexpr pointer data() noexcept { return m_data; }
        constexpr const_pointer data() const noexcept { return m_data; }

        constexpr operator bool() const noexcept {
            return m_data != nullptr && m_size > 0;
        }
        constexpr bool operator==(std::nullptr_t) const noexcept {
            return m_data == nullptr;
        }
        constexpr bool operator!=(std::nullptr_t) const noexcept {
            return m_data != nullptr;
        }
        constexpr operator pointer() noexcept { return data(); }
        constexpr operator const_pointer() const noexcept { return data(); }

        constexpr std::uintptr_t ToAddress() const noexcept
            requires std::is_same_v<value_type, void>
        {
            return reinterpret_cast<std::uintptr_t>(m_data);
        }
        constexpr operator std::uintptr_t() const noexcept
            requires std::is_same_v<value_type, void>
        {
            return ToAddress();
        }
    protected:
        pointer m_data;
        std::size_t m_size;
    };
}
#endif