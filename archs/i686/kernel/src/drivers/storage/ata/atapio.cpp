#include <drivers/storage/ata/atapio.hpp>

#include <core/io.hpp>
#include <core/logger.hpp>
#include <arch/interrupts/isr.hpp>

#include <klibc/cstring>
#include <klibc/string>

#include <utility>

namespace NOS::Drivers::Harddisk::ATAPIO {
    extern "C" std::uint8_t __kernel_serial_read_byte__(std::uint16_t port) noexcept;
    extern "C" std::uint16_t __kernel_serial_read_word__(std::uint16_t port) noexcept;
    extern "C" std::uint32_t __kernel_serial_read_dword__(std::uint16_t port) noexcept;

    extern "C" void __kernel_serial_write_byte__(std::uint16_t port, std::uint8_t data) noexcept;
    extern "C" void __kernel_serial_write_word__(std::uint16_t port, std::uint16_t data) noexcept;
    extern "C" void __kernel_serial_write_dword__(std::uint16_t port, std::uint32_t data) noexcept;

    ATAPIODriver g_ataDriver;

    namespace {
        constexpr std::uint16_t busBases[2] = { Bus::PRIMARY_BASE, Bus::SECONDARY_BASE };
        constexpr std::uint32_t pollTimeout = 1'000'000; // spin-loop iterations before giving up
        
        constexpr auto get_drive_base(DriveType type) noexcept {
            return std::to_underlying(
                type == DriveType::SLAVE
                ? RegisterBits::DriveHeadRegister::DRIVE_SLAVE
                : RegisterBits::DriveHeadRegister::DRIVE_MASTER
            );
        }
        using Status = RegisterBits::StatusRegister;
        using Control = RegisterBits::DeviceControlRegister;
    }

    void ATAPIODriver::init() noexcept {
        std::uint16_t index = 0;

        // Disable interrupts on both buses (nIEN = 1 -> bit 1)
        __kernel_serial_write_byte__(
            Bus::PRIMARY_BASE + Bus::CONTROL,
            std::to_underlying(Control::DISABLE_INTERRUPTS)
        );
        __kernel_serial_write_byte__(
            Bus::SECONDARY_BASE + Bus::CONTROL,
            std::to_underlying(Control::DISABLE_INTERRUPTS)
        );

        for (std::uint_fast8_t bus = 0; bus < 2; ++bus) {
            const auto base = busBases[bus];
            const bool isPrimary = base == Bus::PRIMARY_BASE;

            for (std::uint_fast8_t drv = 0; drv < 2; ++drv) {
                const DriveType driveType = drv == 0 ? DriveType::MASTER : DriveType::SLAVE;

                select_device(base, driveType);
                wait_delay(base);

                __kernel_serial_write_byte__(base + Bus::SECTOR_COUNT, 0);
                __kernel_serial_write_byte__(base + Bus::LBA_LOW, 0);
                __kernel_serial_write_byte__(base + Bus::LBA_MID, 0);
                __kernel_serial_write_byte__(base + Bus::LBA_HIGH, 0);

                __kernel_serial_write_byte__(
                    base + Bus::COMMAND,
                    std::to_underlying(Command::IDENTIFY)
                );

                std::uint8_t status = __kernel_serial_read_byte__(base + Bus::STATUS);

                ATADevice dev;
                dev.id = index;
                dev.driveType = driveType;
                dev.isPrimary = isPrimary;
                dev.isSelected = false;
                dev.supportsLBA48 = false;
                dev.sectorCount = 0;
                dev.model[0] = '\0';

                if (status == 0) {
                    // No device on this bus/position
                    ++index;
                    continue;
                }

                wait_for_device_ready(base);

                std::uint8_t lbaMid = __kernel_serial_read_byte__(base + Bus::LBA_MID);
                std::uint8_t lbaHigh = __kernel_serial_read_byte__(base + Bus::LBA_HIGH);

                if (lbaMid != 0 || lbaHigh != 0) {
                    // Not an ATA device (likely ATAPI) - skip
                    ++index;
                    continue;
                }                

                do {
                    status = __kernel_serial_read_byte__(base + Bus::STATUS);
                    if (Utils::Bits::is_set(status, Status::ERROR)) {
                        break;
                    }
                } while (!Utils::Bits::is_set(status, Status::DATA_REQUEST));

                if (Utils::Bits::is_set(status, Status::ERROR)) {
                    ++index;
                    continue;
                }

                std::uint16_t identifyData[256];
                for (int w = 0; w < 256; ++w) {
                    identifyData[w] = __kernel_serial_read_word__(base + Bus::DATA);
                }

                parse_identify(dev, identifyData);
                dev.isSelected = true;
                IO::kprintf("probe id=%d drv=%d -> %s\r\n", index,
                    dev.driveType == DriveType::MASTER ? 0 : 1, dev.model);
                ++index;
                m_devices.emplace_back(std::move(dev));
            }
        }
    }

    void ATAPIODriver::select_device(std::uint16_t port, DriveType driveType) noexcept {
        __kernel_serial_write_byte__(port + Bus::DRIVE_HEAD, get_drive_base(driveType));
    }

    // For LBA28: drive/head byte carries bits 24-27 of the LBA.
    void ATAPIODriver::select_device_lba(
        std::uint16_t port,
        DriveType driveType,
        std::uint32_t lbaTopNibble
    ) noexcept {
        auto base = get_drive_base(driveType);
        std::uint8_t value = static_cast<std::uint8_t>(base | (lbaTopNibble & 0x0F));
        __kernel_serial_write_byte__(port + Bus::DRIVE_HEAD, value);
    }

    std::uint8_t ATAPIODriver::wait_delay(std::uint16_t port) noexcept {
        std::uint8_t status = 0;

        // 400ns delay: 4 reads of the alternate status register
        for (int i = 0; i < 4; ++i) {
            status = __kernel_serial_read_byte__(port + Bus::CONTROL);
        }
        return status;
    }

    void ATAPIODriver::wait_for_device_ready(std::uint16_t port) noexcept {
        std::uint8_t status;
        do {
            status = __kernel_serial_read_byte__(port + Bus::STATUS);
        } while (Utils::Bits::is_set(status, Status::BUSY));
    }

    // Polls BSY clear then DRQ set (or ERR/DF set) for a data transfer.
    ATAResult ATAPIODriver::poll_for_transfer(std::uint16_t port) noexcept {
        std::uint32_t spins = 0;
        std::uint8_t status;

        do {
            status = __kernel_serial_read_byte__(port + Bus::STATUS);
            if (++spins > pollTimeout) {
                return ATAResult::TIMEOUT;
            }
        } while (Utils::Bits::is_set(status, Status::BUSY));

        if (Utils::Bits::is_set(status, Status::DRIVE_FAULT)) {
            return ATAResult::DEVICE_FAULT;
        }
        if (Utils::Bits::is_set(status, Status::ERROR)) {
            return ATAResult::COMMAND_ABORTED;
        }

        spins = 0;
        while (!Utils::Bits::is_set(status, Status::DATA_REQUEST)) {
            status = __kernel_serial_read_byte__(port + Bus::STATUS);
            if (Utils::Bits::is_set(status, Status::ERROR)) {
                return ATAResult::COMMAND_ABORTED;
            }
            if (++spins > pollTimeout) {
                return ATAResult::TIMEOUT;
            }
        }

        return ATAResult::OK;
    }

    // IDENTIFY word layout: 60-61 = LBA28 sector count, 83 bit10 = LBA48 support,
    // 100-103 = LBA48 sector count, 27-46 = model string (word-swapped ASCII).
    void ATAPIODriver::parse_identify(ATADevice& dev, const std::uint16_t* identifyData) noexcept {
        dev.supportsLBA48 = (identifyData[83] & (1 << 10)) != 0;

        if (dev.supportsLBA48) {
            dev.sectorCount =
                (static_cast<std::uint64_t>(identifyData[100])) |
                (static_cast<std::uint64_t>(identifyData[101]) << 16) |
                (static_cast<std::uint64_t>(identifyData[102]) << 32) |
                (static_cast<std::uint64_t>(identifyData[103]) << 48);
        } else {
            dev.sectorCount =
                (static_cast<std::uint64_t>(identifyData[60])) |
                (static_cast<std::uint64_t>(identifyData[61]) << 16);
        }

        for (int i = 0; i < 20; ++i) {
            std::uint16_t word = identifyData[27 + i];
            dev.model[i * 2]     = static_cast<char>(word >> 8);
            dev.model[i * 2 + 1] = static_cast<char>(word & 0xFF);
        }
        dev.model[40] = '\0';

        // Trim trailing spaces
        for (int i = 39; i >= 0 && dev.model[i] == ' '; --i) {
            dev.model[i] = '\0';
        }
    }

    ATADevice& ATAPIODriver::get_device(std::uint16_t id) noexcept {
        for (auto& dev : m_devices) {
            if (dev.id == id) {
                return dev;
            }
        }
        return m_devices[0];
    }

    std::uint16_t ATAPIODriver::base_for(const ATADevice& dev) const noexcept {
        return dev.isPrimary ? Bus::PRIMARY_BASE : Bus::SECONDARY_BASE;
    }

    // ---- LBA28 ----
    ATAResult ATAPIODriver::read_sectors_lba28(
        ATADevice& dev,
        std::uint16_t base,
        std::uint32_t lba,
        std::uint16_t sectorCount,
        void* buffer
    ) noexcept {
        if (sectorCount == 0 || sectorCount > 256) {
            return ATAResult::INVALID_SECTOR_COUNT;
        }
        std::uint8_t countByte = static_cast<std::uint8_t>(sectorCount == 256 ? 0 : sectorCount);
        std::uint16_t* out = static_cast<std::uint16_t*>(buffer);

        select_device_lba(base, dev.driveType, (lba >> 24) & 0x0F);
        wait_delay(base);

        __kernel_serial_write_byte__(base + Bus::SECTOR_COUNT, countByte);
        __kernel_serial_write_byte__(base + Bus::LBA_LOW, static_cast<std::uint8_t>(lba & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_MID, static_cast<std::uint8_t>((lba >> 8) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_HIGH, static_cast<std::uint8_t>((lba >> 16) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::COMMAND, static_cast<std::uint8_t>(Command::READ_SECTORS_LBA28));

        for (std::uint16_t s = 0; s < sectorCount; ++s) {
            ATAResult res = poll_for_transfer(base);
            if (res != ATAResult::OK)
                return res;

            for (int w = 0; w < 256; ++w) {
                out[s * 256 + w] = __kernel_serial_read_word__(base + Bus::DATA);
            }
        }
        return ATAResult::OK;
    }

    ATAResult ATAPIODriver::write_sectors_lba28(
        ATADevice& dev,
        std::uint16_t base,
        std::uint32_t lba,
        std::uint16_t sectorCount,
        const void* buffer
    ) noexcept {
        if (sectorCount == 0 || sectorCount > 256) {
            return ATAResult::INVALID_SECTOR_COUNT;
        }
        std::uint8_t countByte = static_cast<std::uint8_t>(sectorCount == 256 ? 0 : sectorCount);
        const std::uint16_t* in = static_cast<const std::uint16_t*>(buffer);

        select_device_lba(base, dev.driveType, (lba >> 24) & 0x0F);
        wait_delay(base);

        __kernel_serial_write_byte__(static_cast<std::uint16_t>(base + Bus::SECTOR_COUNT), countByte);
        __kernel_serial_write_byte__(static_cast<std::uint16_t>(base + Bus::LBA_LOW), static_cast<std::uint8_t>(lba & 0xFF));
        __kernel_serial_write_byte__(static_cast<std::uint16_t>(base + Bus::LBA_MID), static_cast<std::uint8_t>((lba >> 8) & 0xFF));
        __kernel_serial_write_byte__(static_cast<std::uint16_t>(base + Bus::LBA_HIGH), static_cast<std::uint8_t>((lba >> 16) & 0xFF));
        __kernel_serial_write_byte__(static_cast<std::uint16_t>(base + Bus::COMMAND), static_cast<std::uint8_t>(Command::WRITE_SECTORS_LBA28));

        for (std::uint16_t s = 0; s < sectorCount; ++s) {
            ATAResult res = poll_for_transfer(base);
            if (res != ATAResult::OK) return res;

            for (int w = 0; w < 256; ++w) {
                __kernel_serial_write_word__(static_cast<std::uint16_t>(base + Bus::DATA), in[s * 256 + w]);
            }
            wait_delay(base);
        }
        return ATAResult::OK;
    }

    // ---- LBA48 ----

    ATAResult ATAPIODriver::read_sectors_lba48(
        ATADevice& dev,
        std::uint16_t base,
        std::uint64_t lba,
        std::uint16_t sectorCount,
        void* buffer
    ) noexcept {
        if (sectorCount == 0)
            return ATAResult::INVALID_SECTOR_COUNT;
        
        std::uint16_t* out = static_cast<std::uint16_t*>(buffer);

        std::uint8_t driveSel = get_drive_base(dev.driveType);

        // For LBA48, drive/head register carries only the drive select bit (no LBA bits).
        __kernel_serial_write_byte__(base + Bus::DRIVE_HEAD, driveSel & 0xF0);
        wait_delay(base);

        // sectorCount == 0 (as sent to controller) means 65536 sectors
        std::uint16_t countReg = sectorCount; // caller-facing max is 65535 anyway (uint16_t)

        // High bytes first
        __kernel_serial_write_byte__(base + Bus::SECTOR_COUNT, static_cast<std::uint8_t>((countReg >> 8) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_LOW, static_cast<std::uint8_t>((lba >> 24) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_MID, static_cast<std::uint8_t>((lba >> 32) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_HIGH, static_cast<std::uint8_t>((lba >> 40) & 0xFF));
        // Low bytes
        __kernel_serial_write_byte__(base + Bus::SECTOR_COUNT, static_cast<std::uint8_t>(countReg & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_LOW, static_cast<std::uint8_t>(lba & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_MID, static_cast<std::uint8_t>((lba >> 8) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_HIGH, static_cast<std::uint8_t>((lba >> 16) & 0xFF));

        __kernel_serial_write_byte__(base + Bus::COMMAND, std::to_underlying(Command::READ_SECTORS_LBA48));

        for (std::uint32_t s = 0; s < sectorCount; ++s) {
            ATAResult res = poll_for_transfer(base);
            if (res != ATAResult::OK)
                return res;

            for (int w = 0; w < 256; ++w) {
                out[s * 256 + w] = __kernel_serial_read_word__(base + Bus::DATA);
            }
        }
        return ATAResult::OK;
    }

    ATAResult ATAPIODriver::write_sectors_lba48(
        ATADevice& dev,
        std::uint16_t base,
        std::uint64_t lba,
        std::uint16_t sectorCount,
        const void* buffer
    ) noexcept {
        if (sectorCount == 0)
            return ATAResult::INVALID_SECTOR_COUNT;
        
        const std::uint16_t* in = static_cast<const std::uint16_t*>(buffer);

        std::uint8_t driveSel = get_drive_base(dev.driveType);

        __kernel_serial_write_byte__(base + Bus::DRIVE_HEAD, driveSel & 0xF0);
        wait_delay(base);

        std::uint16_t countReg = sectorCount;

        __kernel_serial_write_byte__(base + Bus::SECTOR_COUNT, static_cast<std::uint8_t>((countReg >> 8) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_LOW, static_cast<std::uint8_t>((lba >> 24) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_MID, static_cast<std::uint8_t>((lba >> 32) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_HIGH, static_cast<std::uint8_t>((lba >> 40) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::SECTOR_COUNT, static_cast<std::uint8_t>(countReg & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_LOW, static_cast<std::uint8_t>(lba & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_MID, static_cast<std::uint8_t>((lba >> 8) & 0xFF));
        __kernel_serial_write_byte__(base + Bus::LBA_HIGH, static_cast<std::uint8_t>((lba >> 16) & 0xFF));

        __kernel_serial_write_byte__(base + Bus::COMMAND, std::to_underlying(Command::WRITE_SECTORS_LBA48));

        for (std::uint32_t s = 0; s < sectorCount; ++s) {
            ATAResult res = poll_for_transfer(base);
            if (res != ATAResult::OK)
                return res;

            for (int w = 0; w < 256; ++w) {
                __kernel_serial_write_word__(base + Bus::DATA, in[s * 256 + w]);
            }
            wait_delay(base);
        }
        return ATAResult::OK;
    }

    // ---- Public API ----

    ATAResult ATAPIODriver::read_sectors(
        std::uint16_t deviceId,
        std::uint64_t lba,
        std::uint16_t sectorCount,
        void* buffer
    ) noexcept {
        ATADevice& dev = get_device(deviceId);
        if (!dev.isSelected)
            return ATAResult::NOT_IDENTIFIED;

        std::uint16_t base = base_for(dev);

        bool needs48 = dev.supportsLBA48 && ((lba + sectorCount) > 0x0FFFFFFFULL || sectorCount > 256);
        if (needs48)
            return read_sectors_lba48(dev, base, lba, sectorCount, buffer);
        
        if (lba > 0x0FFFFFFFULL || sectorCount > 256)
            return ATAResult::INVALID_SECTOR_COUNT;
        
        return read_sectors_lba28(dev, base, static_cast<std::uint32_t>(lba), sectorCount, buffer);
    }

    ATAResult ATAPIODriver::read_bytes(
        std::uint16_t deviceId,
        std::uint64_t lba,
        std::uint64_t count,
        void* buffer
    ) noexcept {
        auto sectorsNeeded = static_cast<std::uint16_t>((count + 511) / 512);
        klibc::string buff(sectorsNeeded * 512, '\0');
        auto status = read_sectors(
            deviceId, lba,
            sectorsNeeded,
            buff.data()
        );
        if (status != ATAResult::OK)
            return status;

        klibc::memcpy(buffer, buff.data(), count);
        return ATAResult::OK;
    }

    ATAResult ATAPIODriver::write_sectors(
        std::uint16_t deviceId,
        std::uint64_t lba,
        std::uint16_t sectorCount,
        const void* buffer
    ) noexcept {
        ATADevice& dev = get_device(deviceId);
        if (!dev.isSelected)
            return ATAResult::NOT_IDENTIFIED;

        std::uint16_t base = base_for(dev);

        bool needs48 = dev.supportsLBA48 && ((lba + sectorCount) > 0x0FFFFFFFULL || sectorCount > 256);

        ATAResult res = needs48
            ? write_sectors_lba48(dev, base, lba, sectorCount, buffer)
            : (lba > 0x0FFFFFFFULL || sectorCount > 256
                ? ATAResult::INVALID_SECTOR_COUNT
                : write_sectors_lba28(dev, base, static_cast<std::uint32_t>(lba), sectorCount, buffer));

        return res != ATAResult::OK ? res : flush_cache(deviceId);
    }

    ATAResult ATAPIODriver::flush_cache(std::uint16_t deviceId) noexcept {
        ATADevice& dev = get_device(deviceId);
        if (!dev.isSelected)
            return ATAResult::NOT_IDENTIFIED;

        std::uint16_t base = base_for(dev);

        select_device(base, dev.driveType);
        wait_delay(base);

        Command cmd = dev.supportsLBA48 ? Command::CACHE_FLUSH_LBA48 : Command::CACHE_FLUSH;
        __kernel_serial_write_byte__(base + Bus::COMMAND, std::to_underlying(cmd));

        std::uint32_t spins = 0;
        std::uint8_t status;
        do {
            status = __kernel_serial_read_byte__(base + Bus::STATUS);
            if (++spins > pollTimeout)
                return ATAResult::TIMEOUT;
        } while (Utils::Bits::is_bit_set(status, Status::BUSY));

        if (Utils::Bits::is_bit_set(status, Status::ERROR)) {
            return ATAResult::COMMAND_ABORTED;
        }
        return ATAResult::OK;
    }
}