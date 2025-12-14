#include <memory/pmm.hpp>
#include <memory/paging.hpp>
#include <grub/multiboot.hpp>
#include <klibc/cstring>
#include <klibc/array>
#include <io.hpp>

#include <utility>

extern std::uint8_t __kernel_start__[];
extern std::uint8_t __kernel_end__[];

namespace Memory {
    alignas(4096) klibc::array<char, 0x20000> pmm_bitmap{};

    void mark_page(std::uintptr_t addr, bool used) noexcept {
        std::uint64_t index = addr / Paging::ByteUnits::KB4;
        std::uint64_t byte_index = index / 8;
        std::uint8_t bit_index = index % 8;

        if (used) {
            Utils::Bits::SetBit(pmm_bitmap[byte_index], bit_index);
        } else {
            Utils::Bits::ClearBit(pmm_bitmap[byte_index], bit_index);
        }
    }
    
    void mark_page_range(std::uintptr_t start, std::uintptr_t end, bool used) noexcept {
        std::uintptr_t addr = Utils::align_down(start, Paging::ByteUnits::KB4);
        std::uintptr_t end_addr = Utils::align_up(end, Paging::ByteUnits::KB4);

        while (addr < end_addr) {
            mark_page(addr, used);
            addr += Paging::ByteUnits::KB4;
        }
    }

    auto remap_memory_sections(multiboot_info* mb_info) noexcept {
        auto* mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(mb_info->mmap_addr);

        // Compute end‐of‐buffer pointer:
        std::uintptr_t buffer_end = mb_info->mmap_addr + mb_info->mmap_length;

        std::uint64_t nextAvailAddr = 0;

        while (reinterpret_cast<std::uintptr_t>(mmap_entry) < buffer_end) {
            std::uint64_t base = mmap_entry->addr;
            std::uint64_t length = mmap_entry->len;
            std::uint32_t type = mmap_entry->type;

            if (type == MULTIBOOT_MEMORY_RESERVED) {
#ifdef __NOS_DEBUG__
                IO::kprintf("Reserved: 0x%08llX - 0x%08llX -> 0x%08llX - 0x%08llX\r\n",
                    base, base + length - 1, nextAvailAddr, nextAvailAddr + length - 1
                );
#endif

                mark_page_range(
                    nextAvailAddr,
                    nextAvailAddr + length,
                    true
                );
                
                nextAvailAddr = Utils::align_up(
                    nextAvailAddr + length,
                    Paging::ByteUnits::KB4
                );
            }

            mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(
                reinterpret_cast<std::uint8_t*>(mmap_entry)
                + mmap_entry->size + sizeof(mmap_entry->size)
            );
        }

        auto freeMemBeg = nextAvailAddr;

        mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(mb_info->mmap_addr);
        while (reinterpret_cast<std::uintptr_t>(mmap_entry) < buffer_end) {
            std::uint64_t base = mmap_entry->addr;
            std::uint64_t length = mmap_entry->len;
            std::uint32_t type = mmap_entry->type;

            if (type == MULTIBOOT_MEMORY_AVAILABLE) {
#ifdef __NOS_DEBUG__
                IO::kprintf("Free: 0x%08llX - 0x%08llX -> 0x%08llX - 0x%08llX\r\n",
                    base, base + length, nextAvailAddr, nextAvailAddr + length
                );
#endif

                nextAvailAddr += length;
            }

            mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(
                reinterpret_cast<std::uint8_t*>(mmap_entry)
                + mmap_entry->size + sizeof(mmap_entry->size)
            );
        }

        auto __free_memory_beginning__ = reinterpret_cast<std::uint8_t*>(freeMemBeg);
        auto __free_memory_end__ = reinterpret_cast<std::uint8_t*>(nextAvailAddr);

        return std::make_pair(
            reinterpret_cast<std::uintptr_t>(__free_memory_beginning__),
            reinterpret_cast<std::uintptr_t>(__free_memory_end__)
        );
    }
    
    void Init(struct multiboot_info* mb_info) noexcept {
        auto [memstart, memend] = remap_memory_sections(mb_info);

#ifdef __NOS_DEBUG__
        IO::kprintf(
            "Free memory: 0x%X -> 0x%X\r\n",
            memstart,
            memend
        );
#endif

        mark_page_range(
            reinterpret_cast<std::uintptr_t>(__kernel_start__),
            reinterpret_cast<std::uintptr_t>(__kernel_end__),
            true
        );
    }

    void* request_pages(std::size_t pages) noexcept {
        for (std::size_t i = 0; i < (pmm_bitmap.size() * 8); ++i) {
            bool block_found = false;

            for (std::size_t j = 0; j < pages; ++j) {
                std::uint64_t index = i + j;
                std::uint64_t byte_index = index / 8;
                std::uint8_t bit_index = index % 8;

                if (byte_index >= pmm_bitmap.size()) {
                    block_found = false;
                    break;
                }

                if (Utils::Bits::IsBitSet(pmm_bitmap[byte_index], bit_index)) {
                    block_found = false;
                    break;
                }

                block_found = true;
            }

            if (block_found) {
                for (std::size_t j = 0; j < pages; ++j) {
                    std::uint64_t index = i + j;
                    std::uint64_t byte_index = index / 8;
                    std::uint8_t bit_index = index % 8;

                    Utils::Bits::SetBit(pmm_bitmap[byte_index], bit_index);
                }

                return reinterpret_cast<void*>(i * Paging::ByteUnits::KB4);
            }
        }
        return nullptr;
    }

    void free_pages(void* addr, std::size_t pages) noexcept {
        std::uintptr_t address = reinterpret_cast<std::uintptr_t>(addr);

        for (std::size_t i = 0; i < pages; ++i) {
            mark_page(address + i * Paging::ByteUnits::KB4, false);
        }
    }
}