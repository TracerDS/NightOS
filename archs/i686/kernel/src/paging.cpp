#include <paging.hpp>

#include <cstdint>
#include <utility>
#include <type_traits>

namespace Paging {
    extern "C" void __kernel_load_page_directory__(const std::uint32_t* const addr) noexcept;
    extern "C" void __kernel_enable_paging__() noexcept;
    extern "C" void __kernel_flush_tlb_entry__(const void* const addr) noexcept;
    extern "C" void __kernel_flush_tlb_all__() noexcept;

    extern "C" std::uint8_t __kernel_start__[];
    extern "C" std::uint8_t __kernel_end__[];

    constexpr auto KERNEL_VIRTUAL_BASE = __kernel_start__;
    constexpr auto KERNEL_PHYSICAL_BASE = KERNEL_VIRTUAL_BASE - 0xC0000000;

    namespace ByteUnits {
        constexpr std::uint32_t KB = 1024;
        constexpr std::uint32_t MB = 1024 * KB;
        constexpr std::uint32_t GB = 1024 * MB;
    }

    void map_page(void* physAddr, void* virtualAddr, std::uint32_t flags) noexcept {
        // Make sure that both addresses are page-aligned.

        auto pdindex = reinterpret_cast<std::uint32_t>(virtualAddr) >> 22;
        auto ptindex = reinterpret_cast<std::uint32_t>(virtualAddr) >> 12 & 0x03FF;

        auto pd = (std::uint32_t*)0xFFFFF000;
        // Here you need to check whether the PD entry is present.
        // When it is not present, you need to create a new empty PT and
        // adjust the PDE accordingly.

        auto pt = ((std::uint32_t*)0xFFC00000) + (0x400 * pdindex);
        // Here you need to check whether the PT entry is present.
        // When it is, then there is already a mapping present. What do you do now?

        pt[ptindex] = ((std::uint32_t)physAddr) | (flags & 0xFFF) | 0x01; // Present

        __kernel_flush_tlb_entry__(virtualAddr);
    }

    void unmap_page(void* virtualAddr) noexcept {
        auto pdindex = reinterpret_cast<std::uint32_t>(virtualAddr) >> 22;
        auto ptindex = reinterpret_cast<std::uint32_t>(virtualAddr) >> 12 & 0x03FF;

        auto pd = reinterpret_cast<std::uint32_t*>(0xFFFFF000);
        // Here you need to check whether the PD entry is present.
        // When it is not present, you cannot unmap anything.

        auto pt = (reinterpret_cast<std::uint32_t*>(0xFFC00000)) + (0x400 * pdindex);
        // Here you need to check whether the PT entry is present.
        // When it is not present, you cannot unmap anything.

        pt[ptindex] = 0; // Unmap the page

        __kernel_flush_tlb_entry__(virtualAddr);
    }
}