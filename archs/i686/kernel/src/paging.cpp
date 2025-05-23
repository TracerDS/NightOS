#include <paging.hpp>

#include <cstdint>
#include <utility>

namespace Paging {
    std::uint32_t g_PageDirectory[1024] __attribute__((aligned(4096)));
    std::uint32_t g_PageTable[1024] __attribute__((aligned(4096)));

    void Paging_Initialize() noexcept {
        for (std::size_t i = 0; i < 1024; ++i) {
            g_PageTable[i] = i * 0x1000 | 
            std::to_underlying<>(Paging_Flags::PF_PRESENT) |
            std::to_underlying<>(Paging_Flags::PF_READ_WRITE);
        }
    }
}