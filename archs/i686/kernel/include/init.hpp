#ifndef __NOS_INIT_HPP__
#define __NOS_INIT_HPP__

#ifndef __cplusplus
#   error "Kernel must be compiled as a C++ target"
#endif

static_assert(sizeof(void*) == 4, "Kernel must be compiled for 32-bit architecture");

#ifdef _DEBUG
#   ifndef __KERNEL_DEBUG__
#      define __KERNEL_DEBUG__
#   endif
#endif

#ifdef __KERNEL_DEBUG__
#   define __NOS_SERIAL_DEBUG__
#endif

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#   define FORCE_INLINE [[gnu::always_inline]] inline
#   define NO_INLINE    [[gnu::noinline]]
#   define PACKED       [[gnu::packed]]
#else
#   error "Unsupported compiler"
#endif

#include <descriptors/kernel_interrupts.hpp>

#include <concepts>
#include <cstdint>

namespace Utils {
    template <typename T>
    concept is_cstring = std::is_same_v<std::remove_cvref_t<std::remove_pointer_t<T>>, char>;

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
        template <typename T, typename U>
        constexpr void SetBitMask(
            T& value,
            U mask
        ) noexcept { value |= mask; }

        template <typename T, typename U>
        constexpr void ClearBitMask(
            T& value,
            U mask
        ) noexcept { value &= ~mask; }

        template <typename T, typename U>
        constexpr bool IsBitMaskSet(
            T value,
            U mask
        ) noexcept { return (value & mask) != 0; }

        template <typename T>
        constexpr void SetBit(
            T& value,
            std::uint8_t bit
        ) noexcept { SetBitMask(value, 1 << bit); }

        template <typename T>
        constexpr void ClearBit(
            T& value,
            std::uint8_t bit
        ) noexcept { ClearBitMask(value, 1 << bit); }

        template <typename T>
        constexpr bool IsBitSet(
            T value,
            std::uint8_t bit
        ) noexcept { return IsBitMaskSet(value, 1 << bit); }
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
            CallInterrupt<ISR::KernelInterrupts::KERNEL_PANIC>();
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
    protected:
        pointer m_data;
        std::size_t m_size;
    };
}
#endif