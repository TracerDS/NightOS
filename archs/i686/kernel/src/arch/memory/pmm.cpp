#include <arch/memory/pmm.hpp>
#include <arch/memory/vmm.hpp>
#include <arch/memory/paging.hpp>
#include <boot/protocols/multiboot/multiboot.hpp>
#include <klibc/cstring>
#include <klibc/array>
#include <core/logger.hpp>

#include <utility>
#include <limits>

extern std::uint8_t __kernel_post_boot_start__[];
extern std::uint8_t __kernel_start__[];
extern std::uint8_t __kernel_end__[];

namespace NOS::Memory {
    PhysicalMemoryAllocator g_pmmAllocator{};

    auto remap_memory_sections(multiboot_info* mb_info) noexcept {
        if (!mb_info) {
            return std::make_pair<std::uintptr_t, std::uintptr_t>(0, 0);
        }

        if (
            !Utils::Bits::is_set(mb_info->flags, MULTIBOOT_INFO_MEM_MAP) ||
            mb_info->mmap_addr == 0 ||
            mb_info->mmap_length == 0
        ) {
            return std::make_pair<std::uintptr_t, std::uintptr_t>(0, 0);
        }

        constexpr std::uint64_t PHYS_LIMIT_EXCLUSIVE = 0x1'0000'0000ULL;

        // Compute end‐of‐buffer pointer:
        std::uintptr_t mmap_addr = static_cast<std::uintptr_t>(mb_info->mmap_addr);
        std::uintptr_t mmap_length = static_cast<std::uintptr_t>(mb_info->mmap_length);
        std::uintptr_t buffer_end = mmap_addr;
        if (mmap_addr > std::numeric_limits<std::uintptr_t>::max() - mmap_length) {
            buffer_end = std::numeric_limits<std::uintptr_t>::max();
        } else {
            buffer_end = mmap_addr + mmap_length;
        }

        std::uint64_t nextAvailAddr = 0;
        auto freeMemBeg = nextAvailAddr;

        const auto EntryLoopFunc = [&](multiboot_memory_map_t* entry) {
            std::uint64_t base = entry->addr;
            std::uint64_t length = entry->len;
            std::uint32_t type = entry->type;

#ifdef __NOS_KERNEL_DEBUG__
            const char* strtype;
            switch(type) {
                default: strtype = "Unknown"; break;
                case MULTIBOOT_MEMORY_RESERVED: strtype = "Reserved"; break;
                case MULTIBOOT_MEMORY_AVAILABLE: strtype = "Available"; break;
                case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: strtype = "ACPI Reclaimable"; break;
                case MULTIBOOT_MEMORY_NVS: strtype = "NVS"; break;
                case MULTIBOOT_MEMORY_BADRAM: strtype = "Bad RAM"; break;
            }
            Logger::Log(
                "[PMM] %s: 0x%llX - 0x%llX -> 0x%llX - 0x%llX\r\n",
                strtype,
                base, base + length, nextAvailAddr, nextAvailAddr + length
            );
#endif

            if (type != MULTIBOOT_MEMORY_AVAILABLE)
                return;

            // Ignore memory above 4GB.
            if (base >= PHYS_LIMIT_EXCLUSIVE)
                return;

            std::uint64_t endExclusive;
            if (length > std::numeric_limits<std::uint64_t>::max() - base) {
                endExclusive = std::numeric_limits<std::uint64_t>::max();
            } else {
                endExclusive = base + length;
            }

            if (endExclusive > PHYS_LIMIT_EXCLUSIVE) {
                endExclusive = PHYS_LIMIT_EXCLUSIVE;
            }

            if (endExclusive <= base) {
                return;
            }

            length = endExclusive - base;

            if (freeMemBeg == 0) {
                nextAvailAddr = base;
                freeMemBeg = base;
            }

            auto endForMark = endExclusive == PHYS_LIMIT_EXCLUSIVE
                ? static_cast<std::uintptr_t>(0xFFFFFFFFu)
                : static_cast<std::uintptr_t>(endExclusive);

            Memory::g_pmmAllocator.mark_page_range(
                static_cast<std::uintptr_t>(base),
                endForMark,
                false
            );

            nextAvailAddr += length;
        };

        auto* mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(mb_info->mmap_addr);
        while (reinterpret_cast<std::uintptr_t>(mmap_entry) < buffer_end) {
            std::uint64_t current = reinterpret_cast<std::uintptr_t>(mmap_entry);
            std::uint64_t mapEnd = static_cast<std::uint64_t>(buffer_end);

            if (current + sizeof(multiboot_memory_map_t) > mapEnd) {
                break;
            }

            // Entry size does not include its own size field.
            std::uint64_t fullEntrySize =
                static_cast<std::uint64_t>(mmap_entry->size) + sizeof(mmap_entry->size);
            if (fullEntrySize < sizeof(multiboot_memory_map_t)) {
                break;
            }

            if (current + fullEntrySize > mapEnd) {
                break;
            }

            EntryLoopFunc(mmap_entry);

            mmap_entry = reinterpret_cast<multiboot_memory_map_t*>(
                static_cast<std::uintptr_t>(current + fullEntrySize)
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

#ifndef __NOS_KERNEL_DEBUG__
        Logger::Log(
            "Free memory: 0x%lX -> 0x%lX\r\n",
            memstart,
            memend
        );
#endif
        mark_page_range(0x0, 0x100000, true); // Mark first 1MB as used

        auto kernel_boot_int = reinterpret_cast<std::uintptr_t>(__kernel_post_boot_start__);
        auto kernel_start_int = reinterpret_cast<std::uintptr_t>(__kernel_start__);
        auto kernel_end_int = reinterpret_cast<std::uintptr_t>(__kernel_end__);
        
        auto kernel_offset = kernel_start_int - kernel_boot_int;
        auto kernel_start_phys = kernel_start_int - kernel_offset;
        auto kernel_end_phys = kernel_end_int - kernel_offset;

        // Mark kernel pages as used
        mark_page_range(
            kernel_start_phys,
            kernel_end_phys,
            true
        );

#ifndef __NOS_KERNEL_ALLOCATOR_VIRTUAL_MANUAL_INIT__
	    Memory::g_vmmAllocator.init();
#endif
    }

    void PhysicalMemoryAllocator::mark_page(std::uintptr_t addr, bool used) noexcept {
        std::uint64_t index = addr / ByteUnits::KB4;
        std::uint64_t byte_index = index / 8;
        std::uint8_t bit_index = index % 8;

        // Bounds check to prevent buffer overflow
        if (byte_index >= m_bitmap.size()) {
            return;
        }

        if (used) {
            Utils::Bits::set_bit(m_bitmap[byte_index], bit_index);
        } else {
            Utils::Bits::clear_bit(m_bitmap[byte_index], bit_index);
        }
    }
    
    void PhysicalMemoryAllocator::mark_page_range(
        std::uintptr_t start,
        std::uintptr_t end,
        bool used
    ) noexcept {
        std::uint64_t addr = static_cast<std::uint64_t>(start);
        std::uint64_t end_addr = static_cast<std::uint64_t>(end);
        constexpr std::uint64_t pageSize = ByteUnits::KB4;
        std::uint64_t maxAddrExclusive =
            static_cast<std::uint64_t>(m_bitmap.size()) * 8ULL * ByteUnits::KB4;

        if (addr >= maxAddrExclusive) {
            return;
        }

        if (end_addr > maxAddrExclusive) {
            end_addr = maxAddrExclusive;
        }

        if (end_addr <= addr) {
            return;
        }

        addr = Utils::align_down(addr, pageSize);
        end_addr = Utils::align_up(end_addr, pageSize);
        if (end_addr > maxAddrExclusive) {
            end_addr = maxAddrExclusive;
        }

        Logger::Log("[PMM] Marking page range: 0x%llX - 0x%llX as %s\r\n",
            static_cast<unsigned long long>(addr),
            static_cast<unsigned long long>(end_addr),
            used ? "used" : "free"
        );

        while (addr < end_addr) {
            mark_page(static_cast<std::uintptr_t>(addr), used);
            addr += pageSize;
        }
    }

    Utils::array_view<void*> PhysicalMemoryAllocator::request_pages(
        std::size_t pages
    ) noexcept {
        // Validate input
        if (pages == 0 || pages > m_bitmap.size() * 8) {
            return nullptr;
        }

        auto bitmapPagesCount = m_bitmap.size() * 8;

        for (std::size_t i = 0; i < bitmapPagesCount; ++i) {
            // Check if we have enough remaining bits to satisfy the request
            if (i + pages > bitmapPagesCount) {
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
                if (Utils::Bits::is_bit_set(m_bitmap[byte_index], bit_index)) {
                    block_found = false;
                    break;
                }
            }

            if (block_found) {
                for (std::size_t j = 0; j < pages; ++j) {
                    std::uint64_t index = i + j;
                    std::uint64_t byte_index = index / 8;
                    std::uint8_t bit_index = index % 8;

                    Utils::Bits::set_bit(m_bitmap[byte_index], bit_index);
                }

                return {
                    reinterpret_cast<void*>(i * ByteUnits::KB4),
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

        std::uintptr_t address = addr.ToAddress();
        // Ensure the address is page-aligned
        if (address % ByteUnits::KB4 != 0) {
            return;
        }

        for (std::size_t i = 0; i < addr.size(); ++i) {
            mark_page(address + i * ByteUnits::KB4, false);
        }
    }
}