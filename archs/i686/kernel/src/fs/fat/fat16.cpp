#include <fs/fat/fat16.hpp>
#include <core/logger.hpp>
#include <klibc/cstring>
#include <klibc/cctype>

namespace NOS::Filesystem::FAT {
    FAT16Filesystem::FAT16Filesystem(Storage::Block::BlockDevice& device) noexcept
        : m_device(device)
    {}

    bool FAT16Filesystem::init() noexcept {
        std::uint8_t sectorBuf[SECTOR_SIZE];

        if (!m_device.read_block(0, sectorBuf)) {
            Logger::LogError("[FAT16] Failed to read boot sector\r\n");
            return false;
        }
        klibc::memcpy(&m_bootSector, sectorBuf, sizeof(BootSector));

        if (m_bootSector.bytesPerSector != SECTOR_SIZE) {
            Logger::LogError("[FAT16] Unsupported sector size: %d\r\n", m_bootSector.bytesPerSector);
            return false;
        }
        if (m_bootSector.sectorsPerCluster == 0) {
            Logger::LogError("[FAT16] Invalid sectors-per-cluster\r\n");
            return false;
        }

        m_sectorsPerCluster = m_bootSector.sectorsPerCluster;
        m_fatStartLBA = m_bootSector.reservedSectors;
        m_rootDirSectors = static_cast<std::uint32_t>(
            (m_bootSector.rootEntryCount * sizeof(DirEntry) + SECTOR_SIZE - 1) / SECTOR_SIZE
        );
        m_rootDirLBA = m_fatStartLBA + static_cast<std::uint32_t>(m_bootSector.fatCount)
            * m_bootSector.sectorsPerFat;
        m_dataStartLBA = m_rootDirLBA + m_rootDirSectors;

        Logger::Log(
            "[FAT16] fatStart=%lu rootDir=%lu (%lu sectors) dataStart=%lu spc=%lu\r\n",
            static_cast<unsigned long>(m_fatStartLBA),
            static_cast<unsigned long>(m_rootDirLBA),
            static_cast<unsigned long>(m_rootDirSectors),
            static_cast<unsigned long>(m_dataStartLBA),
            static_cast<unsigned long>(m_sectorsPerCluster)
        );

        m_initialized = true;
        return true;
    }

    std::uint32_t FAT16Filesystem::cluster_to_lba(std::uint16_t cluster) const noexcept {
        return m_dataStartLBA + (static_cast<std::uint32_t>(cluster) - 2) * m_sectorsPerCluster;
    }

    std::uint16_t FAT16Filesystem::read_fat_entry(std::uint16_t cluster) noexcept {
        std::uint32_t byteOffset = static_cast<std::uint32_t>(cluster) * 2;
        std::uint32_t sector = m_fatStartLBA + byteOffset / SECTOR_SIZE;
        std::uint32_t offsetInSector = byteOffset % SECTOR_SIZE;

        std::uint8_t buf[SECTOR_SIZE];
        if (!m_device.read_block(sector, buf)) {
            Logger::LogError(
                "[FAT16] Failed to read FAT sector %lu\r\n",
                static_cast<unsigned long>(sector)
            );
            return 0xFFFF;
        }

        std::uint16_t value;
        klibc::memcpy(&value, buf + offsetInSector, sizeof(value));
        return value;
    }

    void FAT16Filesystem::to_8_3(std::string_view name, char out[11]) noexcept {
        for (int i = 0; i < 11; ++i) out[i] = ' ';

        std::size_t dot = name.find('.');
        std::string_view base = dot == std::string_view::npos ? name : name.substr(0, dot);
        std::string_view ext  = dot == std::string_view::npos ? std::string_view{} : name.substr(dot + 1);

        for (std::size_t i = 0; i < base.size() && i < 8; ++i)
            out[i] = static_cast<char>(klibc::toupper(static_cast<unsigned char>(base[i])));
        for (std::size_t i = 0; i < ext.size() && i < 3; ++i)
            out[8 + i] = static_cast<char>(klibc::toupper(static_cast<unsigned char>(ext[i])));
    }

    void FAT16Filesystem::for_each_entry(
        std::uint16_t startCluster,
        bool (*visitor)(const DirEntry&, void*),
        void* userData
    ) noexcept {
        std::uint8_t buf[SECTOR_SIZE];
        constexpr std::size_t entriesPerSector = SECTOR_SIZE / sizeof(DirEntry);

        if (startCluster == 0) {
            // Fixed-size root directory region (not part of the cluster area).
            for (std::uint32_t s = 0; s < m_rootDirSectors; ++s) {
                if (!m_device.read_block(m_rootDirLBA + s, buf)) return;

                auto* entries = reinterpret_cast<DirEntry*>(buf);
                for (std::size_t i = 0; i < entriesPerSector; ++i) {
                    const DirEntry& e = entries[i];
                    if (static_cast<std::uint8_t>(e.name[0]) == 0x00) return; // end of directory
                    if (static_cast<std::uint8_t>(e.name[0]) == 0xE5) continue; // deleted
                    if (e.attributes == Attribute::LONG_NAME) continue;

                    if (!visitor(e, userData)) return;
                }
            }
            return;
        }

        // Subdirectory: data lives in a normal cluster chain, same as a file.
        std::uint16_t cluster = startCluster;
        while (cluster >= 0x0002 && cluster < 0xFFF8) {
            std::uint32_t lba = cluster_to_lba(cluster);

            for (std::uint32_t s = 0; s < m_sectorsPerCluster; ++s) {
                if (!m_device.read_block(lba + s, buf)) return;

                auto* entries = reinterpret_cast<DirEntry*>(buf);
                for (std::size_t i = 0; i < entriesPerSector; ++i) {
                    const DirEntry& e = entries[i];
                    if (static_cast<std::uint8_t>(e.name[0]) == 0x00) return;
                    if (static_cast<std::uint8_t>(e.name[0]) == 0xE5) continue;
                    if (e.attributes == Attribute::LONG_NAME) continue;

                    if (!visitor(e, userData)) return;
                }
            }

            cluster = read_fat_entry(cluster);
        }
    }

    namespace {
        struct FindContext {
            const char* target; // 11 bytes, 8.3-padded
            DirEntry* out;
            bool found;
        };

        bool find_visitor(const DirEntry& entry, void* userData) noexcept {
            auto* ctx = static_cast<FindContext*>(userData);
            char combined[11];
            klibc::memcpy(combined, entry.name, 8);
            klibc::memcpy(combined + 8, entry.ext, 3);

            if (klibc::memcmp(combined, ctx->target, 11) == 0) {
                *ctx->out = entry;
                ctx->found = true;
                return false; // stop
            }
            return true;
        }

        struct ListContext {
            VFS::DirEntryCallback callback;
            void* userData;
        };

        bool list_visitor(const DirEntry& entry, void* userData) noexcept {
            auto* ctx = static_cast<ListContext*>(userData);

            char nameBuf[13];
            std::size_t n = 0;
            for (int i = 0; i < 8 && entry.name[i] != ' '; ++i) nameBuf[n++] = entry.name[i];
            if (entry.ext[0] != ' ') {
                nameBuf[n++] = '.';
                for (int i = 0; i < 3 && entry.ext[i] != ' '; ++i) nameBuf[n++] = entry.ext[i];
            }
            nameBuf[n] = '\0';

            VFS::FileInfo info;
            info.name = klibc::string(nameBuf, n);
            info.size = entry.fileSize;
            info.isDirectory = (entry.attributes & Attribute::DIRECTORY) != 0;

            ctx->callback(info, ctx->userData);
            return true;
        }
    }

    bool FAT16Filesystem::find_in_directory(
        std::uint16_t startCluster,
        const char name83[11],
        DirEntry& outEntry
    ) noexcept {
        FindContext ctx{ name83, &outEntry, false };
        for_each_entry(startCluster, find_visitor, &ctx);
        return ctx.found;
    }

    bool FAT16Filesystem::find(std::string_view path, VFS::FileHandle& out) noexcept {
        if (!m_initialized) return false;

        std::uint16_t currentCluster = 0; // 0 == root directory
        DirEntry entry{};
        bool haveEntry = false;
        std::string_view lastComponent;

        std::size_t pos = 0;
        while (pos < path.size()) {
            while (pos < path.size() && path[pos] == '/') ++pos;
            if (pos >= path.size()) break;

            std::size_t next = path.find('/', pos);
            std::string_view component = (next == std::string_view::npos)
                ? path.substr(pos)
                : path.substr(pos, next - pos);
            pos = (next == std::string_view::npos) ? path.size() : next;

            if (haveEntry && !(entry.attributes & Attribute::DIRECTORY)) {
                return false; // e.g. "/file.txt/x" — file.txt isn't a directory
            }

            char name83[11];
            to_8_3(component, name83);

            if (!find_in_directory(currentCluster, name83, entry)) {
                return false;
            }

            haveEntry = true;
            lastComponent = component;
            currentCluster = entry.firstCluster;
        }

        if (!haveEntry) return false; // path was "/" or empty

        out.info.name = klibc::string(lastComponent.data(), lastComponent.size());
        out.info.size = entry.fileSize;
        out.info.isDirectory = (entry.attributes & Attribute::DIRECTORY) != 0;
        out.startCluster = entry.firstCluster;
        out.position = 0;
        return true;
    }

    std::size_t FAT16Filesystem::read(VFS::FileHandle& handle, void* buffer, std::size_t count) noexcept {
        if (!m_initialized || handle.info.isDirectory) return 0;

        std::size_t remainingInFile = handle.info.size > handle.position
            ? handle.info.size - handle.position : 0;
        std::size_t toRead = count < remainingInFile ? count : remainingInFile;
        if (toRead == 0) return 0;

        std::size_t bytesPerCluster = static_cast<std::size_t>(m_sectorsPerCluster) * SECTOR_SIZE;
        std::size_t clustersToSkip = handle.position / bytesPerCluster;
        std::size_t offsetInCluster = handle.position % bytesPerCluster;

        std::uint16_t cluster = static_cast<std::uint16_t>(handle.startCluster);
        for (std::size_t i = 0; i < clustersToSkip && cluster >= 0x0002 && cluster < 0xFFF8; ++i) {
            cluster = read_fat_entry(cluster);
        }

        std::uint8_t sectorBuf[SECTOR_SIZE];
        std::size_t bytesRead = 0;
        auto* dest = static_cast<std::uint8_t*>(buffer);

        while (bytesRead < toRead && cluster >= 0x0002 && cluster < 0xFFF8) {
            std::uint32_t lba = cluster_to_lba(cluster);

            for (std::uint32_t s = 0; s < m_sectorsPerCluster && bytesRead < toRead; ++s) {
                std::size_t clusterByteOffset = static_cast<std::size_t>(s) * SECTOR_SIZE;
                if (clusterByteOffset + SECTOR_SIZE <= offsetInCluster) continue;

                if (!m_device.read_block(lba + s, sectorBuf)) {
                    handle.position += static_cast<std::uint32_t>(bytesRead);
                    return bytesRead;
                }

                std::size_t startInSector = (offsetInCluster > clusterByteOffset)
                    ? (offsetInCluster - clusterByteOffset) : 0;
                std::size_t availableInSector = SECTOR_SIZE - startInSector;
                std::size_t chunk = availableInSector < (toRead - bytesRead)
                    ? availableInSector : (toRead - bytesRead);

                klibc::memcpy(dest + bytesRead, sectorBuf + startInSector, chunk);
                bytesRead += chunk;
            }

            offsetInCluster = 0; // only the first cluster has a nonzero start offset
            cluster = read_fat_entry(cluster);
        }

        handle.position += static_cast<std::uint32_t>(bytesRead);
        return bytesRead;
    }

    bool FAT16Filesystem::list_dir(std::string_view path, VFS::DirEntryCallback callback, void* userData) noexcept {
        if (!m_initialized) return false;

        std::uint16_t dirCluster = 0; // root by default

        if (!(path.empty() || path == "/")) {
            VFS::FileHandle handle;
            if (!find(path, handle)) return false;
            if (!handle.info.isDirectory) return false;
            dirCluster = static_cast<std::uint16_t>(handle.startCluster);
        }

        ListContext ctx{ callback, userData };
        for_each_entry(dirCluster, list_visitor, &ctx);
        return true;
    }
}