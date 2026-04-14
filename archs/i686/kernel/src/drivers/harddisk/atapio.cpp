#include <drivers/harddisk/atapio.hpp>
#include <logger.hpp>
#include <descriptors/isr.hpp>
#include <klibc/cstring>

#include <utility>

namespace NOS::Drivers::Harddisk::ATAPIO {
    namespace {
        constexpr std::uint32_t ATA_POLL_TIMEOUT = 1'000'000;
        constexpr std::uint32_t ATA_LBA28_MAX = 0x0FFFFFFF;

        FORCE_INLINE bool interrupts_enabled() noexcept {
            std::uint32_t eflags = 0;
            __asm__ volatile(
                "pushf\n"
                "pop %0\n"
                : "=r"(eflags)
                :
                : "memory"
            );
            return (eflags & (1u << 9)) != 0;
        }

        bool compute_sector_count(std::uint32_t size, std::uint8_t& outCount) noexcept {
            if (size == 0) {
                return false;
            }

            auto sectors = (static_cast<std::uint64_t>(size) +
                (ATAPIODriver::SECTOR_SIZE - 1)) / ATAPIODriver::SECTOR_SIZE;

            if (
                sectors == 0 ||
                sectors > ATAPIODriver::MAX_SECTORS_PER_COMMAND
            ) {
                return false;
            }

            outCount = static_cast<std::uint8_t>(sectors);
            return true;
        }
    }

    extern "C" std::uint8_t __kernel_serial_read_byte__(Port port) noexcept;
    extern "C" std::uint16_t __kernel_serial_read_word__(Port port) noexcept;
    extern "C" std::uint32_t __kernel_serial_read_dword__(Port port) noexcept;
    
    extern "C" void __kernel_serial_write_byte__(Port port, std::uint8_t data) noexcept;
    extern "C" void __kernel_serial_write_word__(Port port, std::uint16_t data) noexcept;
    extern "C" void __kernel_serial_write_dword__(Port port, std::uint32_t data) noexcept;

    ATAPIODriver g_ataDriver;

    void ATAPIODriver::wait_delay() noexcept {
        __kernel_serial_read_byte__(Port::CONTROL);
        __kernel_serial_read_byte__(Port::CONTROL);
        __kernel_serial_read_byte__(Port::CONTROL);
        __kernel_serial_read_byte__(Port::CONTROL);
    }

    bool ATAPIODriver::wait_busy() noexcept {
        for (std::uint32_t timeout = 0; timeout < ATA_POLL_TIMEOUT; ++timeout) {
            std::uint8_t status = __kernel_serial_read_byte__(Port::CONTROL);
            if (!Utils::Bits::is_set(status, Status::BUSY)) {
                return true;
            }
        }

        Logger::LogError("ATA Driver: Timed out waiting for BUSY to clear\r\n");
        return false;
    }

    bool ATAPIODriver::wait_data_request() noexcept {
        for (std::uint32_t timeout = 0; timeout < ATA_POLL_TIMEOUT; ++timeout) {
            std::uint8_t status = __kernel_serial_read_byte__(Port::CONTROL);

            if (Utils::Bits::is_set(status, Status::DATA_REQUEST)) {
                return true;
            }

            if (
                Utils::Bits::is_set(status, Status::ERROR) ||
                Utils::Bits::is_set(status, Status::DRIVE_FAULT)
            ) {
                Logger::LogError("ATA Driver: Error while waiting for DRQ\r\n");
                return false;
            }
        }

        Logger::LogError("ATA Driver: Timed out waiting for DRQ\r\n");
        return false;
    }

    void ATAPIODriver::init() noexcept {
        // Disable interrupts
        __kernel_serial_write_byte__(Port::CONTROL, 0x02);
        
        Interrupts::ISR::RegisterHandler(
            0x2E,
            []([[maybe_unused]] Interrupts::ISR::InterruptState* regs)
        {
            g_ataDriver.onInterrupt();
        });

        // Enable interrupts
        __kernel_serial_write_byte__(Port::CONTROL, 0x00);
    }

    bool ATAPIODriver::read(
        void* buffer,
        std::uint32_t address,
        std::uint32_t size
    ) noexcept {
        if (!buffer || size == 0) {
            return false;
        }

        std::uint8_t count = 0;
        if (!compute_sector_count(size, count)) {
            Logger::LogError("ATA Driver: Invalid read size\r\n");
            return false;
        }

        if (!start_read(address, count)) {
            return false;
        }

        auto* targetPtr = static_cast<std::uint8_t*>(buffer);
        std::size_t bytesRemaining = size;
        alignas(2) std::uint8_t bounceBuffer[SECTOR_SIZE]{};

        for (std::uint8_t i = 0; i < count; ++i) {
            wait_delay();
            if (!wait_busy() || !wait_data_request()) {
                return false;
            }

            auto status = __kernel_serial_read_byte__(Port::STATUS);
            if (
                Utils::Bits::is_set(status, Status::ERROR) ||
                Utils::Bits::is_set(status, Status::DRIVE_FAULT)
            ) {
                return false;
            }

            bool useBounce = bytesRemaining < SECTOR_SIZE;
            auto* sectorPtr = useBounce
                ? reinterpret_cast<std::uint16_t*>(bounceBuffer)
                : reinterpret_cast<std::uint16_t*>(targetPtr);

            for (std::size_t j = 0; j < 256; ++j) {
                sectorPtr[j] = __kernel_serial_read_word__(Port::DATA);
            }

            auto consumed = bytesRemaining > SECTOR_SIZE
                ? SECTOR_SIZE
                : bytesRemaining;

            if (useBounce) {
                klibc::memcpy(targetPtr, bounceBuffer, consumed);
            }

            targetPtr += consumed;
            bytesRemaining -= consumed;
        }

        return true;
    }
    
    bool ATAPIODriver::start_read(
        std::uint32_t address,
        std::uint8_t sectorCount
    ) noexcept {
        if (sectorCount == 0) {
            return false;
        }

        std::uint64_t endSector = static_cast<std::uint64_t>(address) + sectorCount - 1;
        if (address > ATA_LBA28_MAX || endSector > ATA_LBA28_MAX) {
            Logger::LogError("ATA Driver: LBA out of 28-bit range\r\n");
            return false;
        }

        if (!wait_busy()) {
            return false;
        }

        __kernel_serial_write_byte__(Port::DRIVE_HEAD, 0xE0 | ((address >> 24) & 0x0F));

        // Wait for the drive to be ready
        wait_delay();

        // Wait for the drive to be available
        if (!wait_busy()) {
            return false;
        }

        __kernel_serial_write_byte__(Port::ERROR, 0x00);
        __kernel_serial_write_byte__(Port::SECTOR_COUNT, sectorCount);

        __kernel_serial_write_byte__(Port::LBA_LOW,  (address >> 0) & 0xFF);
        __kernel_serial_write_byte__(Port::LBA_MID,  (address >> 8) & 0xFF);
        __kernel_serial_write_byte__(Port::LBA_HIGH, (address >> 16) & 0xFF);

        // Send the read command
        __kernel_serial_write_byte__(Port::STATUS, std::to_underlying(Command::READ_SECTORS));
        return true;
    }

    void ATAPIODriver::readSync(
        void* buffer,
        std::uint32_t address,
        std::uint32_t size
    ) noexcept {
        if (m_request.active) {
            Logger::LogError("ATA Driver is busy!\r\n");
            return;
        }

        if (!buffer || size == 0) {
            Logger::LogError("ATA Driver: Invalid sync read request\r\n");
            return;
        }

        std::uint8_t sectorCount = 0;
        if (!compute_sector_count(size, sectorCount)) {
            Logger::LogError("ATA Driver: Invalid sync read size\r\n");
            return;
        }

        // If interrupts are globally disabled, IRQ-driven completion cannot work.
        if (!interrupts_enabled()) {
            Logger::Log(
                "ATA Driver: readSync called with IF=0, using polling fallback\r\n"
            );
            if (!read(buffer, address, size)) {
                Logger::LogError("ATA Driver: Polling fallback read failed\r\n");
            }
            return;
        }

        const auto complete_by_polling = [this]() noexcept -> bool {
            while (m_request.sectorsLeft > 0) {
                wait_delay();
                if (!wait_busy() || !wait_data_request()) {
                    return false;
                }

                auto status = __kernel_serial_read_byte__(Port::STATUS);
                if (
                    Utils::Bits::is_set(status, Status::ERROR) ||
                    Utils::Bits::is_set(status, Status::DRIVE_FAULT)
                ) {
                    return false;
                }

                bool useBounce = m_request.bytesRemaining < SECTOR_SIZE;
                auto* target = useBounce
                    ? reinterpret_cast<std::uint16_t*>(m_request.bounceBuffer)
                    : reinterpret_cast<std::uint16_t*>(m_request.currentBuffer);

                for (std::size_t i = 0; i < 256; ++i) {
                    target[i] = __kernel_serial_read_word__(Port::DATA);
                }

                auto consumed = m_request.bytesRemaining > SECTOR_SIZE
                    ? SECTOR_SIZE
                    : m_request.bytesRemaining;

                if (useBounce && consumed > 0) {
                    klibc::memcpy(m_request.currentBuffer, m_request.bounceBuffer, consumed);
                }

                m_request.currentBuffer += consumed;
                if (m_request.bytesRemaining >= consumed) {
                    m_request.bytesRemaining -= consumed;
                } else {
                    m_request.bytesRemaining = 0;
                }

                --m_request.sectorsLeft;
            }

            return true;
        };

        volatile bool completed = false;
        m_request.active = true;
        m_request.isAsync = false;
        m_request.buffer = static_cast<std::uint8_t*>(buffer);
        m_request.currentBuffer = m_request.buffer;
        m_request.size = size;
        m_request.bytesRemaining = size;
        m_request.sectorsLeft = sectorCount;
        m_request.callback = nullptr;
        m_request.syncCompleteFlag = &completed;

        if (!start_read(address, sectorCount)) {
            m_request.active = false;
            m_request.syncCompleteFlag = nullptr;
            return;
        }

        constexpr std::uint64_t MAX_SPIN_COUNT = 100'000'000ULL;
        std::uint64_t spinCount = 0;
        while (!completed && spinCount < MAX_SPIN_COUNT) {
            ++spinCount;
            __asm__ volatile("pause");
        }

        if (!completed) {
            Logger::Log(
                "ATA Driver: Sync read timed out waiting for IRQ, falling back to polling\r\n"
            );

            // Ignore late IRQ completions while we drain the in-flight request by polling.
            m_request.active = false;
            m_request.syncCompleteFlag = nullptr;

            if (!complete_by_polling()) {
                Logger::LogError("ATA Driver: Polling fallback after IRQ timeout failed\r\n");
                return;
            }

            Logger::Log("ATA Driver: Polling fallback completed sync read\r\n");
            return;
        }

        m_request.active = false;
        m_request.syncCompleteFlag = nullptr;
    }


    void ATAPIODriver::readAsync(
        void* buffer,
        std::uint32_t address,
        std::uint32_t size,
        Callback callback
    ) noexcept {
        if (m_request.active) {
            Logger::LogError("ATA Driver is busy!\r\n");
            return;
        }

        if (!buffer || size == 0) {
            Logger::LogError("ATA Driver: Invalid async read request\r\n");
            if (callback) {
                callback(buffer, size, false);
            }
            return;
        }

        std::uint8_t sectorCount = 0;
        if (!compute_sector_count(size, sectorCount)) {
            Logger::LogError("ATA Driver: Invalid async read size\r\n");
            if (callback) {
                callback(buffer, size, false);
            }
            return;
        }

        m_request.active = true;
        m_request.isAsync = true;
        m_request.buffer = static_cast<std::uint8_t*>(buffer);
        m_request.currentBuffer = m_request.buffer;
        m_request.size = size;
        m_request.bytesRemaining = size;
        m_request.sectorsLeft = sectorCount;
        m_request.syncCompleteFlag = nullptr;
        m_request.callback = callback;

        if (!start_read(address, sectorCount)) {
            m_request.active = false;
            if (m_request.callback) {
                m_request.callback(m_request.buffer, m_request.size, false);
            }
        }
    }

    bool ATAPIODriver::write(
        const void* source,
        std::uint32_t lba,
        std::uint8_t count
    ) noexcept {
        if (!source || count == 0) {
            return false;
        }

        std::uint64_t endSector = static_cast<std::uint64_t>(lba) + count - 1;
        if (lba > ATA_LBA28_MAX || endSector > ATA_LBA28_MAX) {
            Logger::LogError("ATA Driver: Write LBA out of 28-bit range\r\n");
            return false;
        }

        if (!wait_busy()) {
            return false;
        }

        __kernel_serial_write_byte__(Port::DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
        __kernel_serial_write_byte__(Port::ERROR, 0x00);
        __kernel_serial_write_byte__(Port::SECTOR_COUNT, count);

        __kernel_serial_write_byte__(Port::LBA_LOW, lba & 0xFF);
        __kernel_serial_write_byte__(Port::LBA_MID, (lba >> 8) & 0xFF);
        __kernel_serial_write_byte__(Port::LBA_HIGH, (lba >> 16) & 0xFF);

        __kernel_serial_write_byte__(Port::STATUS, std::to_underlying(Command::WRITE_SECTORS));

        const std::uint16_t* sourcePtr = reinterpret_cast<const std::uint16_t*>(source);

        for (std::uint8_t i = 0; i < count; ++i) {
            if (!wait_busy() || !wait_data_request()) {
                return false;
            }

            auto status = __kernel_serial_read_byte__(Port::STATUS);
            if (
                Utils::Bits::is_set(status, Status::ERROR) ||
                Utils::Bits::is_set(status, Status::DRIVE_FAULT)
            ) {
                return false;
            }

            for (std::size_t j = 0; j < 256; ++j) {
                __kernel_serial_write_word__(Port::DATA, sourcePtr[i * 256 + j]);
            }

            // Flush cache command
            __kernel_serial_write_byte__(Port::STATUS, 0xE7);
            wait_delay();
        }

        return true;
    }

    void ATAPIODriver::onInterrupt() noexcept {
        auto status = __kernel_serial_read_byte__(Port::STATUS);

        if (!m_request.active) {
            return;
        }

        if (
            Utils::Bits::is_set(status, Status::ERROR) ||
            Utils::Bits::is_set(status, Status::DRIVE_FAULT)
        ) {
            // Error occurred
            Logger::LogError("ATA Driver: Error during read operation\r\n");

            if (m_request.isAsync && m_request.callback) {
                m_request.callback(m_request.buffer, m_request.size, false);
            }
            if (!m_request.isAsync && m_request.syncCompleteFlag) {
                *m_request.syncCompleteFlag = true;
            }

            m_request.active = false;
            return;
        }

        if (!Utils::Bits::is_set(status, Status::DATA_REQUEST)) {
            Logger::LogError("ATA Driver: IRQ without DRQ\r\n");

            if (m_request.isAsync && m_request.callback) {
                m_request.callback(m_request.buffer, m_request.size, false);
            }
            if (!m_request.isAsync && m_request.syncCompleteFlag) {
                *m_request.syncCompleteFlag = true;
            }

            m_request.active = false;
            return;
        }

        bool useBounce = m_request.bytesRemaining < SECTOR_SIZE;
        auto* target = useBounce
            ? reinterpret_cast<std::uint16_t*>(m_request.bounceBuffer)
            : reinterpret_cast<std::uint16_t*>(m_request.currentBuffer);

        for (std::size_t i = 0; i < 256; ++i) {
            target[i] = __kernel_serial_read_word__(Port::DATA);
        }

        auto consumed = m_request.bytesRemaining > SECTOR_SIZE
            ? SECTOR_SIZE
            : m_request.bytesRemaining;

        if (useBounce && consumed > 0) {
            klibc::memcpy(m_request.currentBuffer, m_request.bounceBuffer, consumed);
        }

        m_request.currentBuffer += consumed;
        if (m_request.bytesRemaining >= consumed) {
            m_request.bytesRemaining -= consumed;
        } else {
            m_request.bytesRemaining = 0;
        }

        if (m_request.sectorsLeft > 0) {
            m_request.sectorsLeft--;
        }

        if (m_request.sectorsLeft > 0) {
            return;
        }

        if (m_request.isAsync) {
            // Async operation complete
            if (m_request.callback) {
                m_request.callback(m_request.buffer, m_request.size, true);
            }
            m_request.active = false;
        } else {
            if (m_request.syncCompleteFlag) {
                *m_request.syncCompleteFlag = true;
            }
            m_request.active = false;
        }
    }
}