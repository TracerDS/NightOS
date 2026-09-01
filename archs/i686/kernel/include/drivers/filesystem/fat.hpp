#pragma once

#include <core/init.hpp>

#include <drivers/storage/ata/atapio.hpp>

#include <cstddef>
#include <cstdint>

namespace NOS::Filesystem::FAT16 {
    struct PACKED BootSector {
        std::uint8_t  jump[3];
        char          oem[8];
        std::uint16_t bytesPerSector;
        std::uint8_t  sectorsPerCluster;
        std::uint16_t reservedSectors;
        std::uint8_t  fatCount;
        std::uint16_t rootEntryCount;
        std::uint16_t totalSectors16;
        std::uint8_t  mediaType;
        std::uint16_t sectorsPerFat;
        std::uint16_t sectorsPerTrack;
        std::uint16_t headCount;
        std::uint32_t hiddenSectors;
        std::uint32_t totalSectors32;
        // ... EBPB fields (drive number, signature, volume label, fs type string)
    };

    struct PACKED DirEntry {
        char          name[8];
        char          ext[3];
        std::uint8_t  attributes;
        std::uint8_t  reserved[10];
        std::uint16_t modTime;
        std::uint16_t modDate;
        std::uint16_t firstCluster;
        std::uint32_t fileSize;
    };
    static_assert(sizeof(DirEntry) == 32);
}
