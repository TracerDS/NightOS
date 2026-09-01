#include <drivers/storage/block/ata_block_device.hpp>
#include <drivers/storage/ata/atapio.hpp>

namespace NOS::Storage::Block {
    namespace ATAPIO = NOS::Drivers::Harddisk::ATAPIO;

    ATABlockDevice::ATABlockDevice(std::uint16_t ataDeviceId) noexcept
        : m_ataDeviceId(ataDeviceId)
    {}

    bool ATABlockDevice::read_block(std::uint64_t lba, void* buffer) noexcept {
        return ATAPIO::g_ataDriver.read_sectors(
            m_ataDeviceId,
            lba,
            1,
            buffer
        ) == ATAPIO::ATAResult::OK;
    }

    bool ATABlockDevice::write_block(std::uint64_t lba, const void* buffer) noexcept {
        return ATAPIO::g_ataDriver.write_sectors(
            m_ataDeviceId,
            lba,
            1,
            buffer
        ) == ATAPIO::ATAResult::OK;
    }

    std::size_t ATABlockDevice::block_size() const noexcept {
        return 512;
    }

    std::uint64_t ATABlockDevice::block_count() const noexcept {
        return ATAPIO::g_ataDriver.get_device(m_ataDeviceId).sectorCount;
    }
}