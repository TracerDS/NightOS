#pragma once

#include <cstdint>
#include <cstddef>

namespace NOS::Storage::Block {
    class BlockDevice {
    public:
        virtual ~BlockDevice() noexcept = default;

        // Reads exactly one block (block_size() bytes) at `lba` into `buffer`.
        virtual bool read_block(std::uint64_t lba, void* buffer) noexcept = 0;

        // Writes exactly one block (block_size() bytes) from `buffer` at `lba`.
        virtual bool write_block(std::uint64_t lba, const void* buffer) noexcept = 0;

        virtual std::size_t block_size() const noexcept = 0;
        virtual std::uint64_t block_count() const noexcept = 0;
    };
}