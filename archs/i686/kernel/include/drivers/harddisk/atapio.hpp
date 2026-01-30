#pragma once

#include <init.hpp>

#include <cstdint>

namespace NOS::Drivers::Harddisk::ATAPIO {
    enum class Port : std::uint16_t {
        DATA         = 0x1F0,
        ERROR        = 0x1F1, // Read: Error, Write: Features
        SECTOR_COUNT = 0x1F2,
        LBA_LOW      = 0x1F3,
        LBA_MID      = 0x1F4,
        LBA_HIGH     = 0x1F5,
        DRIVE_HEAD   = 0x1F6,
        STATUS       = 0x1F7, // Read: Status, Write: Command
        CONTROL      = 0x3F6,
    };

    enum class Status : std::uint8_t {
        ERROR           = 0x01, // Error
        DATA_REQUEST    = 0x08, // Data Request (gotowy do transferu)
        SERVICE_REQUEST = 0x10, // Overlapped Mode Service Request
        DRIVE_FAULT     = 0x20, // Drive Fault
        READY           = 0x40, // Ready
        BUSY            = 0x80  // Busy
    };

    enum class Command : std::uint8_t {
        READ_SECTORS  = 0x20,
        WRITE_SECTORS = 0x30,
        IDENTIFY      = 0xEC
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
        using Callback = void(*)(void* buffer, bool success);
    public:
        /**
         * @brief Initializes the ATA PIO driver
         * 
         * Sets up necessary driver structures and prepares the ATA controller
         * for read/write operations. Must be called before any disk I/O operations.
         * 
         * @noexcept Does not throw exceptions
         */
        void init() noexcept;

        /**
         * @brief Reads data from the hard disk
         * 
         * Reads one or more sectors from the specified LBA address into the
         * target buffer using PIO mode.
         * 
         * @param target Pointer to the destination buffer for read data
         * @param lba Logical Block Address (sector number) to read from
         * @param count Number of sectors to read
         * @return true if read operation succeeded, false otherwise
         * @noexcept Does not throw exceptions
         */
        bool read(void* target, std::uint32_t lba, std::uint8_t count) noexcept;

        void readSync(
            void* buffer,
            std::uint32_t address,
            std::uint32_t size
        ) noexcept;

        void readAsync(
            void* buffer,
            std::uint32_t address,
            std::uint32_t size,
            Callback callback
        ) noexcept;

        void onInterrupt() noexcept;

        /**
         * @brief Writes data to the hard disk
         * 
         * Writes one or more sectors to the specified LBA address from the
         * source buffer using PIO mode.
         * 
         * @param source Pointer to the source buffer containing data to write
         * @param lba Logical Block Address (sector number) to write to
         * @param count Number of sectors to write
         * @return true if write operation succeeded, false otherwise
         * @noexcept Does not throw exceptions
         */
        bool write(const void* source, std::uint32_t lba, std::uint8_t count) noexcept;
    private:
        /**
         * @brief Waits until the ATA device is not busy
         * 
         * Polls the device status register until the busy flag is cleared,
         * indicating the device is ready for commands.
         * 
         * @noexcept Does not throw exceptions
         * @private
         */
        void wait_busy() noexcept;

        /**
         * @brief Waits for the ATA device to request data
         * 
         * Polls the device status register until the data request flag is set,
         * indicating data is ready to be read or the device is ready for writing.
         * 
         * @noexcept Does not throw exceptions
         * @private
         */
        void wait_data_request() noexcept;

        void start_read(std::uint32_t address, std::uint32_t size) noexcept;
        
    private:
        struct {
            bool active = false;
            bool isAsync = false;
            std::uint8_t* buffer = nullptr;

            // For sync mode
            volatile bool* syncCompleteFlag = nullptr;
            Callback callback = nullptr;
        } m_request;
    };

    extern ATAPIODriver g_ataDriver;
}