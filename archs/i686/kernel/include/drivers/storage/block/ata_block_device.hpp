#pragma once

#include <drivers/storage/block/blockdevice.hpp>

#include <cstdint>

namespace NOS::Storage::Block {
    class ATABlockDevice final : public BlockDevice {
    public:
        explicit ATABlockDevice(std::uint16_t ataDeviceId) noexcept;

        bool read_block(std::uint64_t lba, void* buffer) noexcept override;
        bool write_block(std::uint64_t lba, const void* buffer) noexcept override;

        std::size_t block_size() const noexcept override;
        std::uint64_t block_count() const noexcept override;

    private:
        std::uint16_t m_ataDeviceId;
    };
}