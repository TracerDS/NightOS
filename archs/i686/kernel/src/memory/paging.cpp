#include <memory/paging.hpp>
#include <memory/pmm.hpp>
#include <io.hpp>

#include <cstdint>
#include <utility>
#include <type_traits>

namespace Paging {
    extern "C" void __kernel_load_page_directory__(const std::uintptr_t* const addr) noexcept;
    extern "C" void __kernel_enable_paging__() noexcept;
    extern "C" void __kernel_disable_paging__() noexcept;
    extern "C" void __kernel_paging_enable_pse__() noexcept;
    extern "C" void __kernel_flush_tlb_entry__(const void* const addr) noexcept;
    extern "C" void __kernel_flush_tlb_all__() noexcept;

    extern "C" std::uint8_t __kernel_post_boot_start__[];
    extern "C" std::uint8_t __kernel_start__[];
    extern "C" std::uint8_t __kernel_end__[];

    Paging g_paging{};

    void Paging::init() noexcept {
        constexpr auto DEFAULT_FLAGS = PageFlags::PAGE_PRESENT |
            PageFlags::PAGE_READ_WRITE | PageFlags::PAGE_SIZE_ENABLE;

        __kernel_paging_enable_pse__();

        // Reset paging structures (BSS is typically zeroed, but keep this explicit)
        for (std::size_t i = 0; i < 1024; ++i) {
            m_pageDirectory[i] = 0;
        }
        
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

        auto kernel_pages = (kernel_end_phys_aligned - kernel_start_phys_aligned) / ByteUnits::MB4;

        // Identity map the first 4MBs
        map_page(0x0, 0x0, DEFAULT_FLAGS);

        // Map the kernel pages
        for (std::size_t i = 0; i < kernel_pages; ++i) {
            map_page (
                kernel_start_phys_aligned + (i * ByteUnits::MB4),
                kernel_start_virt_aligned + (i * ByteUnits::MB4),
                DEFAULT_FLAGS
            );
        }

        // CR3 must receive the PHYSICAL address of the page directory.
        // We can compute it via the kernel offset (virtual - physical).
        auto pageDirVirt = reinterpret_cast<std::uintptr_t>(m_pageDirectory);
        auto pageDirPhys = pageDirVirt - kernel_offset;

        IO::kprintf("Paging: Page Directory at virt 0x%08lX, phys 0x%08lX\r\n",
            pageDirVirt,
            pageDirPhys
        );

        // Map the page directory into itself (for easy access at 0xFFC00000)
        
        // Map it manually. map_page would try to allocate a new page table
        // but the allocator isnt ready yet.
        m_pageDirectory[1023] = (pageDirPhys & 0xFFFFF000) |
            PageFlags::PAGE_PRESENT | PageFlags::PAGE_READ_WRITE;

        __kernel_load_page_directory__(reinterpret_cast<std::uintptr_t*>(pageDirPhys));
        __kernel_enable_paging__();
        __kernel_flush_tlb_all__();
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

            __kernel_flush_tlb_entry__(
                reinterpret_cast<const void*>(virtualAddr)
            );
            return;
        }

        auto pageDirEntry = m_pageDirectory[pageDirIndex];

        std::uintptr_t* pageTable = nullptr;

        if (pageDirEntry & PageFlags::PAGE_PRESENT) {
            // Page table already exists

            pageTable = reinterpret_cast<std::uintptr_t*>(pageDirEntry & 0xFFFFF000);
        } else {
            // Create new page table
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

            auto* tablePtr = reinterpret_cast<std::uintptr_t*>(newTablePhys.data());
            for (auto i = 0; i < 1024; ++i)
                tablePtr[i] = 0;
            
            m_pageDirectory[pageDirIndex] =
                reinterpret_cast<std::uintptr_t>(newTablePhys.data()) | 
                    PageFlags::PAGE_PRESENT | PageFlags::PAGE_READ_WRITE;

            pageTable = tablePtr;
        }

        pageTable[pageTableIndex] = (physAddr & 0xFFFFF000) | flags;

        // Invalidate TLB
        __kernel_flush_tlb_entry__(reinterpret_cast<const void*>(virtualAddr));
    }

    void Paging::unmap_page(std::uintptr_t virtualAddr) noexcept {
        std::uintptr_t pageDirIndex = virtualAddr >> 22;

        auto pageDirEntry = m_pageDirectory[pageDirIndex];

        if (!(pageDirEntry & PageFlags::PAGE_PRESENT)) {
            // Page not present
            return;
        }

        if (pageDirEntry & PageFlags::PAGE_SIZE_ENABLE) {
            // 4MB page
            m_pageDirectory[pageDirIndex] = 0;
        } else {
            // 4 KB page
            
            auto pageTable = reinterpret_cast<std::uintptr_t*>(pageDirEntry & 0xFFFFF000);
            std::uintptr_t pageTableIndex = (virtualAddr >> 12) & 0x3FF;
            
            pageTable[pageTableIndex] = 0;
        }

        __kernel_flush_tlb_entry__(reinterpret_cast<void*>(virtualAddr));
    }
}