#pragma once

#include <init.hpp>
#include <cstdint>

namespace Paging {
    namespace PageFlags {
        constexpr auto PAGE_PRESENT     = 1 << 0;
        constexpr auto PAGE_NOT_PRESENT = 0 << 0;
        
        constexpr auto PAGE_READ_WRITE = 1 << 1;
        constexpr auto PAGE_READ_ONLY  = 0 << 1;
        
        constexpr auto PAGE_USER_SUPERVISOR = 1 << 2;
        constexpr auto PAGE_SUPERVISOR_ONLY = 0 << 2;

        constexpr auto PAGE_WRITE_THROUGH = 1 << 3;
        constexpr auto PAGE_WRITE_BACK    = 0 << 3;
        
        constexpr auto PAGE_CACHE_DISABLE = 1 << 4;
        constexpr auto PAGE_CACHE_ENABLE  = 0 << 4;
        
        constexpr auto PAGE_ACCESSED     = 1 << 5;
        constexpr auto PAGE_NOT_ACCESSED = 0 << 5;

        constexpr auto PAGE_DIRTY     = 1 << 6;
        constexpr auto PAGE_NOT_DIRTY = 0 << 6;

        constexpr auto PAGE_SIZE_ENABLE  = 1 << 7;
        constexpr auto PAGE_SIZE_DISABLE = 0 << 7;

        constexpr auto PAGE_GLOBAL     = 1 << 8;
        constexpr auto PAGE_NOT_GLOBAL = 0 << 8;

        namespace PageDirectory {
            constexpr auto PAGE_PAT_ENABLE  = 1 << 12;
            constexpr auto PAGE_PAT_DISABLE = 0 << 12;
        }

        namespace PageTable {            
            constexpr auto PAGE_PAT_ENABLE  = 1 << 7;
            constexpr auto PAGE_PAT_DISABLE = 0 << 7;
        }
    }
    
    namespace ByteUnits {
        constexpr std::uint32_t KB = 1024;
        constexpr std::uint32_t MB = 1024 * KB;
        constexpr std::uint32_t GB = 1024 * MB;

        constexpr auto KB4 = 4 * KB;
        constexpr auto MB4 = 4 * MB;
        constexpr auto GB4 = 4 * GB;
    }

    void Paging_Initialize() noexcept;

    void map_page(std::uint32_t physAddr, std::uint32_t virtualAddr, std::uint32_t flags) noexcept;
    void unmap_page(std::uint32_t virtualAddr) noexcept;

    std::uint32_t virtualToPhysical(std::uint32_t virtualAddr) noexcept;
    std::uint32_t physicalToVirtual(std::uint32_t physicalAddr) noexcept;
}