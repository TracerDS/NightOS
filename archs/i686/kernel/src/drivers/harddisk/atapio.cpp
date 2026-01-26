#include <drivers/harddisk/atapio.hpp>

#include <utility>

namespace NOS::Drivers::Harddisk::ATAPIO {
    extern "C" std::uint8_t __kernel_serial_read_byte__(Port port) noexcept;
    extern "C" std::uint16_t __kernel_serial_read_word__(Port port) noexcept;
    extern "C" std::uint32_t __kernel_serial_read_dword__(Port port) noexcept;
    
    extern "C" void __kernel_serial_write_byte__(Port port, std::uint8_t data) noexcept;
    extern "C" void __kernel_serial_write_word__(Port port, std::uint16_t data) noexcept;
    extern "C" void __kernel_serial_write_dword__(Port port, std::uint32_t data) noexcept;

    ATAPIODriver g_ataDriver;

    static void ATA_Delay() noexcept {
        __kernel_serial_read_byte__(Port::STATUS);
        __kernel_serial_read_byte__(Port::STATUS);
        __kernel_serial_read_byte__(Port::STATUS);
        __kernel_serial_read_byte__(Port::STATUS);
    }

    void ATAPIODriver::wait_busy() noexcept {
        while (true) {
            std::uint8_t status = __kernel_serial_read_byte__(Port::STATUS);
            if (!Utils::Bits::is_set(status, std::to_underlying(Status::BUSY))) {
                break;
            }
        }
    }

    void ATAPIODriver::wait_data_request() noexcept {
        while (true) {
            std::uint8_t status = __kernel_serial_read_byte__(Port::STATUS);
            if (Utils::Bits::is_set(status, std::to_underlying(Status::DATA_REQUEST))) {
                break;
            }
            if (!Utils::Bits::is_set(status, std::to_underlying(Status::ERROR))) {
                break;
            }
        }
    }

    void ATAPIODriver::init() noexcept {
        // Disable IRQ14
        //__kernel_serial_write_byte__(Port::CONTROL, 0x02);
    }

    bool ATAPIODriver::read(
        void* target,
        std::uint32_t lba,
        std::uint8_t count
    ) noexcept {
        wait_busy();

        __kernel_serial_write_byte__(Port::DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
        __kernel_serial_write_byte__(Port::ERROR, 0x00);
        __kernel_serial_write_byte__(Port::SECTOR_COUNT, count);

        __kernel_serial_write_byte__(Port::LBA_LOW, lba & 0xFF);
        __kernel_serial_write_byte__(Port::LBA_MID, (lba >> 8) & 0xFF);
        __kernel_serial_write_byte__(Port::LBA_HIGH, (lba >> 16) & 0xFF);

        __kernel_serial_write_byte__(Port::STATUS, std::to_underlying(Command::READ_SECTORS));

        std::uint16_t* targetPtr = reinterpret_cast<std::uint16_t*>(target);

        for (std::uint8_t i = 0; i < count; ++i) {
            ATA_Delay();
            wait_busy();
            wait_data_request();

            auto status = __kernel_serial_read_byte__(Port::STATUS);
            if (Utils::Bits::is_set(status, std::to_underlying(Status::ERROR))) {
                return false;
            }

            for (std::size_t j = 0; j < 256; ++j) {
                targetPtr[i * 256 + j] = __kernel_serial_read_word__(Port::DATA);
            }
        }

        return true;
    }

    bool ATAPIODriver::write(
        const void* source,
        std::uint32_t lba,
        std::uint8_t count
    ) noexcept {
        wait_busy();

        __kernel_serial_write_byte__(Port::DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
        __kernel_serial_write_byte__(Port::ERROR, 0x00);
        __kernel_serial_write_byte__(Port::SECTOR_COUNT, count);

        __kernel_serial_write_byte__(Port::LBA_LOW, lba & 0xFF);
        __kernel_serial_write_byte__(Port::LBA_MID, (lba >> 8) & 0xFF);
        __kernel_serial_write_byte__(Port::LBA_HIGH, (lba >> 16) & 0xFF);

        __kernel_serial_write_byte__(Port::STATUS, std::to_underlying(Command::WRITE_SECTORS));

        const std::uint16_t* sourcePtr = reinterpret_cast<const std::uint16_t*>(source);

        for (std::uint8_t i = 0; i < count; ++i) {
            wait_busy();
            wait_data_request();

            for (std::size_t j = 0; j < 256; ++j) {
                __kernel_serial_write_word__(Port::DATA, sourcePtr[i * 256 + j]);
            }

            // Flush cache command
            __kernel_serial_write_byte__(Port::STATUS, 0xE7);
            ATA_Delay();
        }

        return true;
    }
}