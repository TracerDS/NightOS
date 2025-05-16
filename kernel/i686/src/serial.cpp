#include <serial.hpp>

extern "C" std::uint8_t __kernel_read_byte__(std::uint16_t port) noexcept;
extern "C" std::uint16_t __kernel_read_word__(std::uint16_t port) noexcept;
extern "C" std::uint32_t __kernel_read_dword__(std::uint16_t port) noexcept;

extern "C" void __kernel_write_byte__(std::uint16_t port, std::uint8_t data) noexcept;
extern "C" void __kernel_write_word__(std::uint16_t port, std::uint16_t data) noexcept;
extern "C" void __kernel_write_dword__(std::uint16_t port, std::uint32_t data) noexcept;

namespace Serial {
    std::uint8_t ReadByte(std::uint16_t port) noexcept {
        return __kernel_read_byte__(port);
    }
    std::uint16_t ReadWord(std::uint16_t port) noexcept {
        return __kernel_read_word__(port);
    }
    std::uint32_t ReadDword(std::uint16_t port) noexcept {
        return __kernel_read_dword__(port);
    }
    
    void WriteByte(std::uint16_t port, std::uint8_t data) noexcept {
        __kernel_write_byte__(port, data);
    }
    void WriteWord(std::uint16_t port, std::uint16_t data) noexcept {
        __kernel_write_word__(port, data);
    }
    void WriteDword(std::uint16_t port, std::uint32_t data) noexcept {
        __kernel_write_dword__(port, data);
    }
}