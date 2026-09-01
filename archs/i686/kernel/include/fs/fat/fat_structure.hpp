#pragma once

#include <core/init.hpp>
#include <cstdint>

namespace NOS::Filesystem::FAT {
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

        // FAT12/16 Extended BIOS Parameter Block
        std::uint8_t  driveNumber;
        std::uint8_t  reserved1;
        std::uint8_t  bootSignature;
        std::uint32_t volumeId;
        char          volumeLabel[11];
        char          fsType[8];
    };
    static_assert(sizeof(BootSector) == 62, "FAT16 boot sector size mismatch");

    namespace Attribute {
        constexpr std::uint8_t READ_ONLY = 0x01;
        constexpr std::uint8_t HIDDEN    = 0x02;
        constexpr std::uint8_t SYSTEM    = 0x04;
        constexpr std::uint8_t VOLUME_ID = 0x08;
        constexpr std::uint8_t DIRECTORY = 0x10;
        constexpr std::uint8_t ARCHIVE   = 0x20;
        // Exactly these bits set marks a VFAT long-name entry (unsupported here).
        constexpr std::uint8_t LONG_NAME = READ_ONLY | HIDDEN | SYSTEM | VOLUME_ID;
    }

    struct PACKED DirEntry {
        char          name[8];
        char          ext[3];
        std::uint8_t  attributes;
        std::uint8_t  reservedNT;
        std::uint8_t  createTimeTenths;
        std::uint16_t createTime;
        std::uint16_t createDate;
        std::uint16_t lastAccessDate;
        std::uint16_t firstClusterHigh; // always 0 on FAT16
        std::uint16_t modTime;
        std::uint16_t modDate;
        std::uint16_t firstCluster;
        std::uint32_t fileSize;
    };
    static_assert(sizeof(DirEntry) == 32, "FAT directory entry size mismatch");
}