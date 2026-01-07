#pragma once

#include <init.hpp>
#include <klibc/array>

#include <cstddef>
#include <cstdint>
#include <concepts>

struct multiboot_info;

namespace Memory {
    class PhysicalMemoryAllocator {
    public:
        void init(struct multiboot_info* mb_info) noexcept;

        void mark_page(std::uintptr_t address, bool used) noexcept;
        void mark_page_range(
            std::uintptr_t start,
            std::uintptr_t end,
            bool used
        ) noexcept;

        Utils::array_view<void*> request_pages(std::size_t pages) noexcept;
        void free_pages(Utils::array_view<void*> addr) noexcept;
    private:
        // Bitmap: 1 bit per 4KB page. Each bit: 1 = used, 0 = free
        // 0x20000 bytes = 4GB of memory
        alignas(4096) klibc::array<std::uint8_t, 0x20000> m_bitmap{};
    };

    extern PhysicalMemoryAllocator g_pmmAllocator;
}