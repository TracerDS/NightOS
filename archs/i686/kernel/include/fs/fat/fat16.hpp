#pragma once

#include <core/init.hpp>
#include <fs/vfs/vfs.hpp>
#include <fs/fat/fat_structure.hpp>
#include <drivers/storage/block/blockdevice.hpp>

#include <string_view>
#include <cstdint>
#include <cstddef>

namespace NOS::Filesystem::FAT {
    class FAT16Filesystem final : public VFS::Filesystem {
    public:
        explicit FAT16Filesystem(Storage::Block::BlockDevice& device) noexcept;

        bool init() noexcept override;

        bool find(
            std::string_view path,
            VFS::FileHandle& out
        ) noexcept override;

        std::size_t read(
            VFS::FileHandle& handle,
            void* buffer,
            std::size_t count
        ) noexcept override;

        bool list_dir(
            std::string_view path,
            VFS::DirEntryCallback callback,
            void* userData
        ) noexcept override;

    private:
        static constexpr std::size_t SECTOR_SIZE = 512;

        Storage::Block::BlockDevice& m_device;
        BootSector m_bootSector{};

        std::uint32_t m_fatStartLBA = 0;
        std::uint32_t m_rootDirLBA = 0;
        std::uint32_t m_rootDirSectors = 0;
        std::uint32_t m_dataStartLBA = 0;
        std::uint32_t m_sectorsPerCluster = 0;
        bool m_initialized = false;

        std::uint32_t cluster_to_lba(std::uint16_t cluster) const noexcept;
        std::uint16_t read_fat_entry(std::uint16_t cluster) noexcept;

        // startCluster == 0 means "the fixed-size root directory region".
        bool find_in_directory(
            std::uint16_t startCluster,
            const char name83[11],
            DirEntry& outEntry
        ) noexcept;

        // Visits every valid (non-deleted, non-LFN) entry of a directory.
        // Return false from `visitor` to stop early.
        using DirEntryVisitor = bool (*)(const DirEntry&, void*);
        void for_each_entry(
            std::uint16_t startCluster,
            DirEntryVisitor visitor,
            void* userData
        ) noexcept;

        static void to_8_3(std::string_view name, char out[11]) noexcept;
    };
}