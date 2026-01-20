#include <memory/paging.hpp>
#include <memory/pmm.hpp>
#include <io.hpp>
#include <logger.hpp>

#include <cstdint>
#include <utility>
#include <type_traits>

namespace NOS::Memory {
    extern "C" void __kernel_load_page_directory__(std::uintptr_t addr) noexcept;
    extern "C" void __kernel_enable_paging__() noexcept;
    extern "C" void __kernel_disable_paging__() noexcept;
    extern "C" void __kernel_paging_enable_pse__() noexcept;
    extern "C" void __kernel_flush_tlb_entry__(uintptr_t addr) noexcept;
    extern "C" void __kernel_flush_tlb_all__() noexcept;

    extern "C" std::uint8_t __kernel_post_boot_start__[];
    extern "C" std::uint8_t __kernel_start__[];
    extern "C" std::uint8_t __kernel_end__[];

    constexpr std::uintptr_t PT_BASE_VIRT = 0xFFC00000;
    constexpr std::uintptr_t PD_BASE_VIRT = 0xFFFFF000;

    Paging g_paging{};

    void Paging::init() noexcept {
        constexpr auto DEFAULT_FLAGS = PageFlags::PAGE_PRESENT |
            PageFlags::PAGE_READ_WRITE | PageFlags::PAGE_SIZE_ENABLE;
        
        auto kernel_boot_int = reinterpret_cast<std::uintptr_t>(__kernel_post_boot_start__);
        auto kernel_start_int = reinterpret_cast<std::uintptr_t>(__kernel_start__);
        auto kernel_end_int = reinterpret_cast<std::uintptr_t>(__kernel_end__);
            
        std::uintptr_t kernel_offset = kernel_start_int - kernel_boot_int;
        std::uintptr_t kernel_start_phys = kernel_start_int - kernel_offset;
        std::uintptr_t kernel_end_phys = kernel_end_int - kernel_offset;

        // 4MB (PSE) mappings require 4MB alignment; map the aligned region containing the kernel.
        auto kernel_start_phys_aligned = Utils::align_down(kernel_start_phys, ByteUnits::MB4);
        auto kernel_start_virt_aligned = Utils::align_down(kernel_start_int, ByteUnits::MB4);
        auto kernel_end_phys_aligned = Utils::align_up(kernel_end_phys, ByteUnits::MB4);
        auto kernel_end_virt_aligned = Utils::align_up(kernel_end_int, ByteUnits::MB4);

        auto kernel_pages = (kernel_end_phys_aligned - kernel_start_phys_aligned) / ByteUnits::MB4;

        Logger::Log("[Paging] Enabling PSE...\r\n");
        __kernel_paging_enable_pse__();

        // Reset paging structures (BSS is typically zeroed, but keep this explicit)
        for (std::size_t i = 0; i < 1024; ++i) {
            m_pageDirectory[i] = 0;
        }
        // CR3 must receive the PHYSICAL address of the page directory.
        // We can compute it via the kernel offset (virtual - physical).
        auto pageDirVirt = reinterpret_cast<std::uintptr_t>(m_pageDirectory);
        auto pageDirPhys = pageDirVirt - kernel_offset;

        Logger::Log(
            "[Paging] Page Directory at virt 0x%08lX, phys 0x%08lX\r\n",
            pageDirVirt,
            pageDirPhys
        );

        // Map the page directory into itself (for easy access at 0xFFC00000)
        Logger::Log("[Paging] Mapping page directory at 0xFFC00000\r\n");

        // Map it manually. map_page would try to allocate a new page table
        // but the allocator isnt ready yet.
        m_pageDirectory[1023] = (pageDirPhys & 0xFFFFF000) |
            PageFlags::PAGE_PRESENT | PageFlags::PAGE_READ_WRITE;

        Logger::Log("[Paging] Identity mapping 0x%08lX - 0x%08lX...\r\n",
            0x0, ByteUnits::MB4
        );
        // Identity map the first 4MBs
        map_page(0x0, 0x0, DEFAULT_FLAGS);

        Logger::Log("[Paging] Mapping kernel pages (0x%08lX - 0x%08lX)...\r\n",
            reinterpret_cast<uintptr_t>(kernel_start_virt_aligned),
            reinterpret_cast<uintptr_t>(kernel_end_virt_aligned)
        );

        // Map the kernel pages
        for (std::size_t i = 0; i < kernel_pages; ++i) {
            map_page (
                kernel_start_phys_aligned + (i * ByteUnits::MB4),
                kernel_start_virt_aligned + (i * ByteUnits::MB4),
                DEFAULT_FLAGS
            );
        }

        Logger::Log("[Paging] Loading paging directory (0x%08lX)...\r\n", pageDirPhys);
        __kernel_load_page_directory__(pageDirPhys);
        __kernel_enable_paging__();
        Logger::Log("[Paging] Paging initialized!\r\n");
    }

    void Paging::map_page(
        std::uintptr_t physAddr,
        std::uintptr_t virtualAddr,
        std::uint32_t flags
    ) noexcept {
        std::uintptr_t pageDirIndex = virtualAddr >> 22;
        std::uintptr_t pageTableIndex = (virtualAddr >> 12) & 0x3FF;

        flags |= PageFlags::PAGE_PRESENT;

        // 4MB page mapping
        if (Utils::Bits::IsBitMaskSet(flags, PageFlags::PAGE_SIZE_ENABLE)) {
            // Large pages require 4MB alignment.
            m_pageDirectory[pageDirIndex] = (physAddr & 0xFFC00000) | flags;
            __kernel_flush_tlb_entry__(virtualAddr);
            return;
        }

        auto pageDirEntry = m_pageDirectory[pageDirIndex];

        if (
            Utils::Bits::IsBitMaskSet(pageDirEntry, PageFlags::PAGE_PRESENT) && 
            Utils::Bits::IsBitMaskSet(pageDirEntry, PageFlags::PAGE_SIZE_ENABLE)
        ) {
            IO::kprintf(
                "PANIC: Trying to map 4KB page at 0x%08lX but 4MB page already exists!\r\n",
                virtualAddr
            );
            Utils::Asm::KernelPanic();
            return;
        }

        std::uintptr_t* pageTableVirt = reinterpret_cast<std::uintptr_t*>(
            PT_BASE_VIRT + (pageDirIndex * 0x1000)
        );
        
        if (!Utils::Bits::IsBitMaskSet(pageDirEntry, PageFlags::PAGE_PRESENT)) {
            auto newTablePhys = Memory::g_pmmAllocator.request_pages(1);
            if (!newTablePhys) {
                // Out of memory. PANIC
#ifdef __KERNEL_DEBUG__
                IO::kprintf(
                    "PANIC: Out of memory while mapping address %08lX to %08lX\r\n",
                    physAddr, virtualAddr
                );
#endif
                Utils::Asm::KernelPanic();
                return;
            }

#ifdef __KERNEL_DEBUG__
            IO::kprintf(
                "[Paging] Created new page table for virtual address 0x%08lX at phys 0x%08lX\r\n",
                virtualAddr,
                newTablePhys.ToAddress()
            );
#endif

            m_pageDirectory[pageDirIndex] = newTablePhys.ToAddress() |
                PageFlags::PAGE_PRESENT | PageFlags::PAGE_READ_WRITE;

            __kernel_flush_tlb_entry__(reinterpret_cast<std::uintptr_t>(pageTableVirt));
            klibc::memset(pageTableVirt, 0, ByteUnits::KB4);
        }

        // Set the page table entry
        pageTableVirt[pageTableIndex] = (physAddr & 0xFFFFF000) | flags;
        // Invalidate TLB
        __kernel_flush_tlb_entry__(virtualAddr);
    }

    void Paging::unmap_page(std::uintptr_t virtualAddr) noexcept {
        std::uintptr_t pageDirIndex = virtualAddr >> 22;

        auto pageDirEntry = m_pageDirectory[pageDirIndex];

        if (!Utils::Bits::IsBitMaskSet(pageDirEntry, PageFlags::PAGE_PRESENT)) {
            // Page not present
            return;
        }

        // 4MB page
        if (Utils::Bits::IsBitMaskSet(pageDirEntry, PageFlags::PAGE_SIZE_ENABLE)) {
            m_pageDirectory[pageDirIndex] = 0;
        } else {
            // 4 KB page
            std::uintptr_t* pageTableVirt = reinterpret_cast<std::uintptr_t*>(
                PT_BASE_VIRT + (pageDirIndex * 0x1000)
            );
            
            std::uintptr_t pageTableIndex = (virtualAddr >> 12) & 0x3FF;            
            pageTableVirt[pageTableIndex] = 0;
        }

        __kernel_flush_tlb_entry__(virtualAddr);
    }
}