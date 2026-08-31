#pragma once

#include <core/init.hpp>

#include <klibc/inplace_vector>

#include <cstdint>

namespace NOS::Drivers::Harddisk::ATAPIO {
    namespace Bus {
        constexpr std::uint_fast16_t DATA         = 0;
        constexpr std::uint_fast16_t ERROR        = 1;
        constexpr std::uint_fast16_t FEATURES     = ERROR;
        constexpr std::uint_fast16_t SECTOR_COUNT = 2;
        constexpr std::uint_fast16_t LBA_LOW      = 3;
        constexpr std::uint_fast16_t LBA_MID      = 4;
        constexpr std::uint_fast16_t LBA_HIGH     = 5;
        constexpr std::uint_fast16_t DRIVE_HEAD   = 6;
        constexpr std::uint_fast16_t STATUS       = 7;
        constexpr std::uint_fast16_t COMMAND      = STATUS;
        constexpr std::uint_fast16_t CONTROL      = 0x206;

        constexpr std::uint_fast16_t PRIMARY_BASE   = 0x1F0; // Primary bus
        constexpr std::uint_fast16_t SECONDARY_BASE = 0x170; // Secondary bus
    }
    namespace RegisterBits {
        enum class DriveHeadRegister : std::uint8_t {
            LBA_MODE     = 0b01000000, // LBA mode
            CHS_MODE     = 0b00000000, // CHS mode
            DRIVE_MASTER = 0b11100000, // Master drive
            DRIVE_SLAVE  = 0b11110000, // Slave drive
        };
        enum class DeviceControlRegister : std::uint8_t {
            DISABLE_INTERRUPTS   = 1 << 1, // Disable interrupts
            ENABLE_INTERRUPTS    = 0 << 1, // Enable interrupts
            SOFTWARE_RESET_START = 1 << 2, // Software reset set
            SOFTWARE_RESET_END   = 0 << 2, // Software reset clear
            LBA_READ_PREVIOUS    = 1 << 7, // LBA read previous value
            LBA_READ_CURRENT     = 0 << 7, // LBA read current value
        };
        enum class StatusRegister : std::uint8_t {
            ERROR           = 1 << 0, // Error
            INDEX           = 1 << 1, // Index (obsolete)
            CORRECTED_DATA  = 1 << 2, // Corrected data (obsolete)
            DATA_REQUEST    = 1 << 3, // Data Request
            OVERLAP_SERVICE = 1 << 4, // "seek complete" / overlap service request (obsolete)
            DRIVE_FAULT     = 1 << 5, // Drive Fault
            READY           = 1 << 6, // Ready
            BUSY            = 1 << 7, // Busy
        };
        enum class ErrorRegister : std::uint8_t {
            NO_ADDRESS_MARK   = 1 << 0, // No address mark
            TRACK_0_NOT_FOUND = 1 << 1, // Track 0 not found
            COMMAND_ABORTED   = 1 << 2, // Command aborted
            MEDIA_CHANGE      = 1 << 3, // Media change request
            ID_NOT_FOUND      = 1 << 4, // ID not found (bad sector address)
            MEDIA_CHANGED     = 1 << 5, // Media changed
            UNCORRECTABLE     = 1 << 6, // Uncorrectable data
            BAD_BLOCK         = 1 << 7, // Bad block
        };
    }

    enum class DriveType : std::uint_fast8_t {
        MASTER = 0x00,
        SLAVE  = 0x01,
    };

    enum class Command : std::uint_fast8_t {
        READ_SECTORS_LBA28  = 0x20,
        READ_SECTORS_LBA48  = 0x24,
        WRITE_SECTORS_LBA28 = 0x30,
        WRITE_SECTORS_LBA48 = 0x34,
        CACHE_FLUSH         = 0xE7,
        CACHE_FLUSH_LBA48   = 0xEA,
        IDENTIFY            = 0xEC,
        RESET               = 0x04,
    };

    enum class ATAResult : std::uint_fast8_t {
        OK = 0,
        NO_DEVICE,
        TIMEOUT,
        DEVICE_FAULT,
        COMMAND_ABORTED,
        INVALID_SECTOR_COUNT,
        NOT_IDENTIFIED,
    };

    struct ATADevice {
        std::uint16_t id;
        DriveType driveType;
        bool isPrimary;
        bool isSelected;
        bool supportsLBA48;
        std::uint64_t sectorCount;
        char model[41];
    };

    /**
     * @class ATAPIODriver
     * @brief ATA PIO (Programmed Input/Output) driver for hard disk operations
     * 
     * Provides low-level access to ATA hard disks using PIO mode, handling
     * read and write operations at the LBA (Logical Block Addressing) level.
     * This driver manages communication with ATA devices through port I/O
     * and includes synchronization primitives for device status monitoring.
     */
    class ATAPIODriver {
    public:
        void init() noexcept;

        ATADevice& get_device(std::uint16_t id) noexcept;

        // Reads `sectorCount` sectors starting at `lba` into `buffer`.
        // `buffer` must be at least sectorCount * 512 bytes.
        // Uses LBA48 automatically if the device supports it and the
        // request exceeds LBA28 range/limits, otherwise LBA28.
        ATAResult read_sectors(
            std::uint16_t deviceId,
            std::uint64_t lba,
            std::uint16_t sectorCount,
            void* buffer
        ) noexcept;

        ATAResult read_bytes(
            std::uint16_t deviceId,
            std::uint64_t lba,
            std::uint64_t count,
            void* buffer
        ) noexcept;

        // Writes `sectorCount` sectors starting at `lba` from `buffer`,
        // then flushes the device cache.
        ATAResult write_sectors(
            std::uint16_t deviceId,
            std::uint64_t lba,
            std::uint16_t sectorCount,
            const void* buffer
        ) noexcept;

        ATAResult flush_cache(std::uint16_t deviceId) noexcept;

        constexpr auto begin(this auto& self) noexcept { return self.m_devices.begin(); }
        constexpr auto end(this auto& self) noexcept { return self.m_devices.end(); }
    private:
        void select_device(std::uint16_t port, DriveType driveType) noexcept;
        void select_device_lba(std::uint16_t port, DriveType driveType, std::uint32_t lbaTopNibble) noexcept;
        std::uint8_t wait_delay(std::uint16_t port) noexcept;
        void wait_for_device_ready(std::uint16_t port) noexcept;
        ATAResult poll_for_transfer(std::uint16_t port) noexcept;
        void parse_identify(ATADevice& dev, const std::uint16_t* identifyData) noexcept;

        ATAResult read_sectors_lba28(
            ATADevice& dev,
            std::uint16_t base,
            std::uint32_t lba,
            std::uint16_t sectorCount,
            void* buffer
        ) noexcept;

        ATAResult read_sectors_lba48(
            ATADevice& dev,
            std::uint16_t base,
            std::uint64_t lba,
            std::uint16_t sectorCount,
            void* buffer
        ) noexcept;

        ATAResult write_sectors_lba28(
            ATADevice& dev,
            std::uint16_t base,
            std::uint32_t lba,
            std::uint16_t sectorCount,
            const void* buffer
        ) noexcept;

        ATAResult write_sectors_lba48(
            ATADevice& dev,
            std::uint16_t base,
            std::uint64_t lba,
            std::uint16_t sectorCount,
            const void* buffer
        ) noexcept;


        std::uint16_t base_for(const ATADevice& dev) const noexcept;

        klibc::inplace_vector<ATADevice, 4> m_devices;
    };

    extern ATAPIODriver g_ataDriver;
}