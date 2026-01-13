#pragma once

#include <init.hpp>

#include <cstdint>
#include <type_traits>
#include <concepts>

namespace NOS::Serial {
    using Port = std::uint16_t;
    constexpr Port COM1 = 0x3F8;
    constexpr Port COM2 = 0x2F8;
    constexpr Port COM3 = 0x3E8;
    constexpr Port COM4 = 0x2E8;
    constexpr Port COM5 = 0x5F8;
    constexpr Port COM6 = 0x4F8;
    constexpr Port COM7 = 0x5E8;
    constexpr Port COM8 = 0x4E8;

    class Serial {
    public:
        void init(Port port) noexcept;

        std::uint8_t read_byte(Port port) noexcept;
        std::uint16_t read_word(Port port) noexcept;
        std::uint32_t read_dword(Port port) noexcept;
        
        template <std::integral T>
        T read(Port port) noexcept {
            if constexpr (sizeof(T) == 1) {
                return read_byte(port);
            } else if constexpr (sizeof(T) == 2) {
                return read_word(port);
            } else if constexpr (sizeof(T) == 4) {
                return read_dword(port);
            } else {
                static_assert(false, "Unsupported size for Serial::Read");
            }
        }
        
        void write_byte(Port port, std::uint8_t data) noexcept;
        void write_word(Port port, std::uint16_t data) noexcept;
        void write_dword(Port port, std::uint32_t data) noexcept;

        void write_string(Port port, const char* const str) noexcept;
        
        template <std::integral T>
        void write(Port port, T data) noexcept {
            if constexpr (sizeof(T) == 1) {
                return write_byte(port, data);
            } else if constexpr (sizeof(T) == 2) {
                return write_word(port, data);
            } else if constexpr (sizeof(T) == 4) {
                return write_dword(port, data);
            } else {
                static_assert(false, "Unsupported size for Serial::write");
            }
        }

        template <typename T, Port port>
        void write_com(T data) noexcept {
            if constexpr (std::is_integral_v<T>) {
                write<T>(port, data);
            } else if constexpr (Utils::is_cstring<T>) {
                return write_string(port, data);
            } else {
                static_assert(false, "Unsupported type for Serial::write_com1");
            }
        }

        template <typename T>
        void write_com1(T data) noexcept { write_com<T, COM1>(data); }
        
        template <typename T>
        void write_com2(T data) noexcept { write_com<T, COM2>(data); }
        
        template <typename T>
        void write_com3(T data) noexcept { write_com<T, COM3>(data); }
        
        template <typename T>
        void write_com4(T data) noexcept { write_com<T, COM4>(data); }
        
        template <typename T>
        void write_com5(T data) noexcept { write_com<T, COM5>(data); }
        
        template <typename T>
        void write_com6(T data) noexcept { write_com<T, COM6>(data); }
        
        template <typename T>
        void write_com7(T data) noexcept { write_com<T, COM7>(data); }
        
        template <typename T>
        void write_com8(T data) noexcept { write_com<T, COM8>(data); }
    };

    extern Serial g_serial;
}