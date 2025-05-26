#include <paging.hpp>

#include <cstdint>
#include <utility>
#include <type_traits>

namespace Paging {
    extern "C" void __kernel_load_page_directory__(const std::uint32_t* const addr) noexcept;
    extern "C" void __kernel_enable_paging__() noexcept;

    constexpr auto KERNEL_PHYSICAL_BASE = 0x00000000;
    constexpr auto KERNEL_VIRTUAL_BASE = 0xC0000000;

    namespace ByteUnits {
        constexpr std::uint32_t KB = 1024;
        constexpr std::uint32_t MB = 1024 * KB;
        constexpr std::uint32_t GB = 1024 * MB;
    }

    std::uint32_t g_PageDirectory[1024] __attribute__((aligned(4096)));
    std::uint32_t g_PageTable[1024] __attribute__((aligned(4096)));
    std::uint32_t g_KernelPageTable[1024] __attribute__((aligned(4096)));

    void Paging_Initialize() noexcept {
        constexpr auto PAGE4KB = 4 * ByteUnits::KB;

        // Clear page directory
        for (std::size_t i = 0; i < 1024; ++i) {
            g_PageDirectory[i] = 0x00000000 |
                std::to_underlying<>(PagingFlags::PF_NOT_PRESENT) |
                std::to_underlying<>(PagingFlags::PF_READ_WRITE) |
                std::to_underlying<>(PagingFlags::PF_SUPERVISOR);
        }

        // Identity map the first 1 MB of memory
        for (std::size_t i = 0; i < ByteUnits::MB / PAGE4KB; ++i) {
            g_PageTable[i] = i * PAGE4KB | 
                std::to_underlying<>(PagingFlags::PF_PRESENT) |
                std::to_underlying<>(PagingFlags::PF_READ_WRITE) |
                std::to_underlying<>(PagingFlags::PF_SUPERVISOR);
        }

        // Set the first page directory entry to point to the page table
        g_PageDirectory[0] = reinterpret_cast<std::uint32_t>(g_PageTable) |
            std::to_underlying<>(PagingFlags::PF_PRESENT) |
            std::to_underlying<>(PagingFlags::PF_READ_WRITE) |
            std::to_underlying<>(PagingFlags::PF_SUPERVISOR);

        // Map the kernel physical base (0x00000000) to virtual 0xC0000000
        for (std::size_t i = 0; i < ByteUnits::MB / PAGE4KB; i++) {
            // Map kernel: 0xC0000000 → 0x00000000 (physical)
            g_KernelPageTable[i] = (i * PAGE4KB) |
                std::to_underlying<>(PagingFlags::PF_PRESENT) |
                std::to_underlying<>(PagingFlags::PF_READ_WRITE) |
                std::to_underlying<>(PagingFlags::PF_SUPERVISOR);
        }

        // Set Page Directory entry for virtual address 0xC0000000
        g_PageDirectory[KERNEL_VIRTUAL_BASE >> 22] =
            reinterpret_cast<std::uint32_t>(g_KernelPageTable)
            | std::to_underlying<>(PagingFlags::PF_PRESENT)
            | std::to_underlying<>(PagingFlags::PF_READ_WRITE)
            | std::to_underlying<>(PagingFlags::PF_SUPERVISOR);
        
        __kernel_enable_paging__();
        __kernel_load_page_directory__(g_PageDirectory);
    }

    void map_page(void* physaddr, void* virtualaddr, unsigned int flags) {
        // Make sure that both addresses are page-aligned.

        unsigned long pdindex = (unsigned long)virtualaddr >> 22;
        unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

        unsigned long *pd = (unsigned long *)0xFFFFF000;
        // Here you need to check whether the PD entry is present.
        // When it is not present, you need to create a new empty PT and
        // adjust the PDE accordingly.

        unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
        // Here you need to check whether the PT entry is present.
        // When it is, then there is already a mapping present. What do you do now?

        pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present

        // Now you need to flush the entry in the TLB
        // or you might not notice the change.
    }

    void unmap_page(void* virtualaddr) {
        unsigned long pdindex = (unsigned long)virtualaddr >> 22;
        unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

        unsigned long *pd = (unsigned long *)0xFFFFF000;
        // Here you need to check whether the PD entry is present.
        // When it is not present, you cannot unmap anything.

        unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
        // Here you need to check whether the PT entry is present.
        // When it is not present, you cannot unmap anything.

        pt[ptindex] = 0; // Unmap the page

        // Now you need to flush the entry in the TLB
    }
}