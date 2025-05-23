#pragma once

#include <cstdint>

namespace Paging {
    enum class Paging_Flags : std::uint16_t {
        PF_PRESENT = 1 << 0,
        
        PF_READ_ONLY = 0 << 1,
        PF_READ_WRITE = 1 << 1,

        PF_SUPERVISOR = 0 << 2,
        PF_USER = 1 << 2,

        PF_WRITE_THROUGH = 1 << 3,

        PF_CACHE_DISABLE = 1 << 4,
        PF_CACHE_ENABLE = 0 << 4,

        PF_ACCESSED = 1 << 5,
        PF_DIRTY = 1 << 6,

        PF_PAGE_SIZE_4KB = 0 << 7,
        PF_PAGE_SIZE_4MB = 1 << 7,

        PF_GLOBAL = 1 << 8,
    };
    
    void Paging_Initialize() noexcept;
}