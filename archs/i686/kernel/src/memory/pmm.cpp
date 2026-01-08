#include <memory/pmm.hpp>
#include <memory/vmm.hpp>
#include <memory/paging.hpp>
#include <grub/multiboot.hpp>
#include <klibc/cstring>
#include <klibc/array>
#include <io.hpp>

#include <utility>

extern std::uint8_t __kernel_start__[];
extern std::uint8_t __kernel_end__[];

namespace Memory {
    PhysicalMemoryAllocator g_pmmAllocator{};

    auto remap_memory_sections(multiboot_info* mb_info) noexcept {
        // Compute end‐of‐buffer pointer:
        std::uintptr_t buffer_end = mb_info->mmap_addr + mb_info->mmap_length;

        std::uint64_t nextAvailAddr = 0;
        auto freeMemBeg = nextAvailAddr;

        const auto EntryLoopFunc = [&](multiboot_memory_map_t* entry) {
            [[maybe_unused]] std::uint64_t base = entry->addr;
            std::uint64_t length = entry->len;
            std::uint32_t type = entry->type;

#ifdef __KERNEL_DEBUG__
            const char* strtype;
            switch(type) {
                default: strtype = "Unknown"; break;
                case MULTIBOOT_MEMORY_RESERVED: strtype = "Reserved"; break;
                case MULTIBOOT_MEMORY_AVAILABLE: strtype = "Available"; break;
                case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: strtype = "ACPI Reclaimable"; break;
                case MULTIBOOT_MEMORY_NVS: strtype = "NVS"; break;
                case MULTIBOOT_MEMORY_BADRAM: strtype = "Bad RAM"; break;
            }
            IO::kprintf("%s: 0x%llX - 0x%llX -> 0x%llX - 0x%llX\r\n",
                strtype,
                base, base + length, nextAvailAddr, nextAvailAddr + length
            );
#endif

            if (type != MULTIBOOT_MEMORY_AVAILABLE)
                return;

            // Ignore memory above 4GB
            if (base > 0xFFFFFFFF) 
                return;

            // Clamp length to not exceed 4GB
            if (base + length > 0xFFFFFFFF) {
                length = 0xFFFFFFFF - base;
            }

            if (freeMemBeg == 0) {
                nextAvailAddr = base;
                freeMemBeg = base;
            }

            Memory::g_pmmAllocator.mark_page_range(
                static_cast<std::uintptr_t>(base),
                static_cast<std::uintptr_t>(base + length),
                false
            );

            nextAvailAddr += length;
        };

        auto* mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(mb_info->mmap_addr);
        while(reinterpret_cast<std::uintptr_t>(mmap_entry) < buffer_end) {
            EntryLoopFunc(mmap_entry);

            mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(
                reinterpret_cast<std::uint8_t*>(mmap_entry)
                + mmap_entry->size + sizeof(mmap_entry->size)
            );
        }

        return std::make_pair(
            static_cast<std::uintptr_t>(freeMemBeg),
            static_cast<std::uintptr_t>(nextAvailAddr)
        );
    }
    
    void PhysicalMemoryAllocator::init(struct multiboot_info* mb_info) noexcept {
        klibc::memset(m_bitmap.data(), 0xFF, m_bitmap.size());

        [[maybe_unused]] auto [memstart, memend] = remap_memory_sections(mb_info);

#ifndef __KERNEL_DEBUG__
        IO::kprintf(
            "Free memory: 0x%lX -> 0x%lX\r\n",
            memstart,
            memend
        );
#endif
        Memory::g_pmmAllocator.mark_page_range(
            reinterpret_cast<std::uintptr_t>(__kernel_start__),
            reinterpret_cast<std::uintptr_t>(__kernel_end__),
            true
        );

	    Memory::g_vmmAllocator.init(memstart, memend - memstart);
    }

    void PhysicalMemoryAllocator::mark_page(std::uintptr_t addr, bool used) noexcept {
        std::uint64_t index = addr / Paging::ByteUnits::KB4;
        std::uint64_t byte_index = index / 8;
        std::uint8_t bit_index = index % 8;

        // Bounds check to prevent buffer overflow
        if (byte_index >= m_bitmap.size()) {
            return;
        }

        if (used) {
            Utils::Bits::SetBit(m_bitmap[byte_index], bit_index);
        } else {
            Utils::Bits::ClearBit(m_bitmap[byte_index], bit_index);
        }
    }
    
    void PhysicalMemoryAllocator::mark_page_range(
        std::uintptr_t start,
        std::uintptr_t end,
        bool used
    ) noexcept {
        std::uintptr_t addr = Utils::align_down(start, Paging::ByteUnits::KB4);
        std::uintptr_t end_addr = Utils::align_up(end, Paging::ByteUnits::KB4);

        while (addr < end_addr) {
            mark_page(addr, used);
            addr += Paging::ByteUnits::KB4;
        }
    }

    Utils::array_view<void*> PhysicalMemoryAllocator::request_pages(
        std::size_t pages
    ) noexcept {
        // Validate input
        if (pages == 0 || pages > m_bitmap.size() * 8) {
            return nullptr;
        }

        for (std::size_t i = 0; i < (m_bitmap.size() * 8); ++i) {
            // Check if we have enough remaining bits to satisfy the request
            if (i + pages > m_bitmap.size() * 8) {
                break;
            }

            bool block_found = true;

            // Check if all requested pages are free
            for (std::size_t j = 0; j < pages; ++j) {
                std::uint64_t index = i + j;
                std::uint64_t byte_index = index / 8;
                std::uint8_t bit_index = index % 8;

                if (byte_index >= m_bitmap.size()) {
                    block_found = false;
                    break;
                }

                // If ANY bit is set (page is used), this block won't work
                if (Utils::Bits::IsBitSet(m_bitmap[byte_index], bit_index)) {
                    block_found = false;
                    break;
                }
            }

            if (block_found) {
                for (std::size_t j = 0; j < pages; ++j) {
                    std::uint64_t index = i + j;
                    std::uint64_t byte_index = index / 8;
                    std::uint8_t bit_index = index % 8;

                    Utils::Bits::SetBit(m_bitmap[byte_index], bit_index);
                }

                return {
                    reinterpret_cast<void*>(i * Paging::ByteUnits::KB4),
                    pages
                };
            }
        }
        return nullptr;
    }

    void PhysicalMemoryAllocator::free_pages(Utils::array_view<void*> addr) noexcept {
        if (!addr) {
            return;
        }

        std::uintptr_t address = reinterpret_cast<std::uintptr_t>(addr.data());
        // Ensure the address is page-aligned
        if (address % Paging::ByteUnits::KB4 != 0) {
            return;
        }

        for (std::size_t i = 0; i < addr.size(); ++i) {
            mark_page(address + i * Paging::ByteUnits::KB4, false);
        }
    }
}