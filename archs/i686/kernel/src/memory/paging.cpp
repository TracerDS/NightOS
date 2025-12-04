#include <memory/paging.hpp>

#include <cstdint>
#include <utility>
#include <type_traits>

namespace Paging {
    extern "C" void __kernel_load_page_directory__(const std::uint32_t* const addr) noexcept;
    extern "C" void __kernel_enable_paging__() noexcept;
    extern "C" void __kernel_disable_paging__() noexcept;
    extern "C" void __kernel_flush_tlb_entry__(const void* const addr) noexcept;
    extern "C" void __kernel_flush_tlb_all__() noexcept;

    extern "C" std::uint8_t __kernel_post_boot_start__[];
    extern "C" std::uint8_t __kernel_start__[];
    extern "C" std::uint8_t __kernel_end__[];

    alignas(ByteUnits::KB4) static std::uint32_t gs_pageDirectory[1024];

    void Paging_Initialize() noexcept {
        constexpr auto DEFAULT_FLAGS = PageFlags::PAGE_PRESENT |
            PageFlags::PAGE_READ_WRITE | PageFlags::PAGE_SIZE_ENABLE;

        
        auto kernel_boot_int = reinterpret_cast<std::uintptr_t>(__kernel_post_boot_start__);
        auto kernel_start_int = reinterpret_cast<std::uintptr_t>(__kernel_start__);
        auto kernel_end_int = reinterpret_cast<std::uintptr_t>(__kernel_end__);
            
        auto kernel_offset = kernel_start_int - kernel_boot_int;
        auto kernel_start_phys = kernel_start_int - kernel_offset;
        auto kernel_end_phys = kernel_end_int - kernel_offset;
        auto kernel_size = kernel_end_phys - kernel_start_phys;

        auto kernel_pages = Utils::align_up(
            kernel_size,
            ByteUnits::MB4
        ) / ByteUnits::MB4;

        // Memory map first 4MBs
        map_page(0x0, 0x0, DEFAULT_FLAGS);

        // Map the kernel pages
        for (auto i = 0; i < kernel_pages; ++i) {
            map_page (
                kernel_start_phys + (i * ByteUnits::MB4),
                kernel_offset,
                DEFAULT_FLAGS
            );
        }

        __kernel_load_page_directory__(
            reinterpret_cast<std::uint32_t*>(virtualToPhysical(
                reinterpret_cast<std::uint32_t>(gs_pageDirectory)
            ))
        );
    }

    void map_page(std::uint32_t physAddr, std::uint32_t virtualAddr, std::uint32_t flags) noexcept {
        std::uint32_t pageDirIndex = virtualAddr >> 22;
        std::uint32_t pageTableIndex;

        // Check if 4MB page
        if (flags & PageFlags::PAGE_SIZE_ENABLE) {
            // 4 MB page
            pageTableIndex = (physAddr & 0xFFC00000);
        } else {
            // 4 KB page
            pageTableIndex = (virtualAddr >> 12) & 0x3FF;
        }
        gs_pageDirectory[pageDirIndex] = pageTableIndex | flags;
    }

    void unmap_page(std::uint32_t virtualAddr) noexcept {
        std::uint32_t pageDirIndex = virtualAddr >> 22;
        auto pageDirEntry = gs_pageDirectory[pageDirIndex];

        if (!(pageDirEntry & PageFlags::PAGE_PRESENT)) {
            // Page not present
            return;
        }

        if (pageDirEntry & PageFlags::PAGE_SIZE_ENABLE) {
            // 4MB page
            gs_pageDirectory[pageDirIndex] = 0;
        } else {
            // 4 KB page
            
            auto pageTable = reinterpret_cast<std::uint32_t*>(pageDirEntry & 0xFFFFF000);
            std::uint32_t pageTableIndex = (virtualAddr >> 12) & 0x3FF;
            
            pageTable[pageTableIndex] = 0;
        }

        __kernel_flush_tlb_entry__(reinterpret_cast<void*>(virtualAddr));
    }
    
    std::uint32_t virtualToPhysical(std::uint32_t virtualAddr) noexcept {
        auto virtualAddrInt = reinterpret_cast<std::uint32_t>(virtualAddr);

        std::uint32_t pageDirIndex = virtualAddrInt >> 22; // top 10 bits
        std::uint32_t pageTableIndex = (virtualAddrInt >> 12) & 0x3FF; // next 10 bits
        std::uint32_t offset = virtualAddrInt & 0xFFF; // last 12 bits

        std::uint32_t pde = gs_pageDirectory[pageDirIndex];

        if (!(pde & PageFlags::PAGE_PRESENT)) {
            // Page not present
            return static_cast<std::uint32_t>(-1);
        }

        if (pde & PageFlags::PAGE_SIZE_ENABLE) { // PS bit = 1 -> 4MB page
            auto phys_base = pde & 0xFFC00000; // top 10 bits for 4MB page
            return phys_base + (virtualAddrInt & 0x3FFFFF);    // offset within 4MB
        } else {
            // 4KB page, follow the page table
            auto pageTable = reinterpret_cast<std::uint32_t*>(pde & 0xFFFFF000);
            std::uint32_t pte = pageTable[pageTableIndex];

            if (!(pte & PageFlags::PAGE_PRESENT)) {
                return static_cast<std::uint32_t>(-1); // Page not present
            }

            auto phys_base = pte & 0xFFFFF000;
            return phys_base + offset;
        }
    }

    std::uint32_t physicalToVirtual(std::uint32_t physicalAddr) noexcept {
        for (std::uint32_t pd_index = 0; pd_index < 1024; ++pd_index) {
            auto pde = gs_pageDirectory[pd_index];
            if (!(pde & PageFlags::PAGE_PRESENT))
                continue; // PDE not present

            if (pde & PageFlags::PAGE_SIZE_ENABLE) { // 4 MB page
                auto base = pde & 0xFFC00000;
                if (physicalAddr >= base && physicalAddr < base + 0x400000) {
                    return (pd_index << 22) + (physicalAddr - base);
                }
            } else { // 4 KB pages
                auto pageTable = reinterpret_cast<std::uint32_t*>(pde & 0xFFFFF000);
                for (std::uint32_t pt_index = 0; pt_index < 1024; ++pt_index) {
                    auto pte = pageTable[pt_index];
                    if (!(pte & PageFlags::PAGE_PRESENT))
                        continue; // PTE not present
                        
                    auto base = pte & 0xFFFFF000;
                    if (physicalAddr >= base && physicalAddr < base + 0x1000) {
                        return (pd_index << 22) | (pt_index << 12) | (physicalAddr - base);
                    }
                }
            }
        }
        return static_cast<std::uint32_t>(-1); // not mapped
    }
}