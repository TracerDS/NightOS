#pragma once

#include <init.hpp>
#include <cstdint>
#include <type_traits>
#include <concepts>

namespace Serial {
    std::uint8_t ReadByte(std::uint16_t port) noexcept;
    std::uint16_t ReadWord(std::uint16_t port) noexcept;
    std::uint32_t ReadDword(std::uint16_t port) noexcept;
    
    void WriteByte(std::uint16_t port, std::uint8_t data) noexcept;
    void WriteWord(std::uint16_t port, std::uint16_t data) noexcept;
    void WriteDword(std::uint16_t port, std::uint32_t data) noexcept;

    template <std::unsigned_integral T>
    T Read(std::uint16_t port) noexcept {
        if constexpr (sizeof(T) == 1) {
            return ReadByte(port);
        } else if constexpr (sizeof(T) == 2) {
            return ReadWord(port);
        } else if constexpr (sizeof(T) == 4) {
            return ReadDword(port);
        } else {
            static_assert(false, "Unsupported size for Serial::Read");
        }
    }
    
    template <std::unsigned_integral T>
    void Write(std::uint16_t port, T data) noexcept {
        if constexpr (sizeof(T) == 1) {
            WriteByte(port, static_cast<std::uint8_t>(data));
        } else if constexpr (sizeof(T) == 2) {
            WriteWord(port, static_cast<std::uint16_t>(data));
        } else if constexpr (sizeof(T) == 4) {
            WriteDword(port, static_cast<std::uint32_t>(data));
        } else {
            static_assert(false, "Unsupported size for Serial::Write");
        }
    }
}