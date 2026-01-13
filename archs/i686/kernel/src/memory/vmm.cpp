#include <memory/vmm.hpp>
#include <memory/pmm.hpp>
#include <memory/paging.hpp>

#include <io.hpp>

#include <cstddef>
#include <cstdint>

namespace NOS::Memory {
    VirtualMemoryAllocator g_vmmAllocator{};

    class LinkedListAllocator {
        struct NodeHeader {
            std::size_t size;
            bool isFree;
            NodeHeader* next;
        };
        void* baseAddr;
        std::size_t totalSize;
        NodeHeader* head;
    public:
        void Initialize(std::uintptr_t startPhysAddr, std::size_t size) noexcept;
        void* Allocate(std::size_t size) noexcept;
        void Free(void* ptr) noexcept;
    };
    LinkedListAllocator g_linkedListAllocator{};

    void LinkedListAllocator::Initialize(std::uintptr_t startPhysAddr, std::size_t size) noexcept {
        // Assume we are mapped in already
        auto pagesNeeded = Utils::align_up(size, ByteUnits::KB4) / ByteUnits::KB4;
    
#ifdef __KERNEL_DEBUG__
        IO::kprintf("VMM: Initializing with %08lX bytes (%lu pages)\r\n",
            size,
            pagesNeeded
        );
#endif

        for (std::size_t i = 0; i < pagesNeeded; ++i) {
            // Get the physical page
            auto physAddr = g_pmmAllocator.request_pages(1);
            if (!physAddr) {
                // No memory. Panic
                IO::kprintf_color(
                    "VMM: Out of memory during initialization!\r\n",
                    Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
                    Terminal::VGAColor::VGA_COLOR_BLACK
                );
                Utils::Asm::KernelPanic();
                return;
            }

            // Map it
            g_paging.map_page(
                reinterpret_cast<std::uintptr_t>(physAddr.data()), 
                startPhysAddr + (i * ByteUnits::KB4), 
                PageFlags::PAGE_PRESENT | PageFlags::PAGE_READ_WRITE
            );
        }

        baseAddr = reinterpret_cast<void*>(startPhysAddr);
        totalSize = size;

        // Create the first, large, free block
        head = reinterpret_cast<NodeHeader*>(startPhysAddr);
        head->size = size - sizeof(NodeHeader);
        head->isFree = true;
        head->next = nullptr;
    }
    
    void* LinkedListAllocator::Allocate(std::size_t size) noexcept {
        std::size_t alignedSize = Utils::align_up(size, alignof(std::max_align_t));

#ifdef __KERNEL_DEBUG__
        IO::kprintf("VMM: Allocating %lu bytes (aligned to %lu bytes)\r\n",
            size,
            alignedSize
        );
#endif

        NodeHeader* current = head;
        while (current) {
            if (current->isFree && current->size >= alignedSize) {
                // Found a suitable block
                if (current->size >= alignedSize + sizeof(NodeHeader) + alignof(std::max_align_t)) {
                    // Split the block
                    NodeHeader* newBlock = reinterpret_cast<NodeHeader*>(
                        reinterpret_cast<std::uintptr_t>(current) + sizeof(NodeHeader) + alignedSize
                    );
                    newBlock->size = current->size - alignedSize - sizeof(NodeHeader);
                    newBlock->isFree = true;
                    newBlock->next = current->next;

                    current->size = alignedSize;
                    current->next = newBlock;
                }

                current->isFree = false;
                return reinterpret_cast<void*>(
                    reinterpret_cast<std::uintptr_t>(current) + sizeof(NodeHeader)
                );

            }
            current = current->next;
        }
        return nullptr;
    }
    
    void LinkedListAllocator::Free(void* ptr) noexcept {
        if (!ptr)
            return;

        NodeHeader* header = reinterpret_cast<NodeHeader*>(
            reinterpret_cast<std::uintptr_t>(ptr) - sizeof(NodeHeader)
        );
        header->isFree = true;

        // Coalesce adjacent free blocks
        NodeHeader* current = head;
        while (current && current->next) {
            if (current->isFree && current->next->isFree) {
                current->size += sizeof(NodeHeader) + current->next->size;
                current->next = current->next->next;
            } else {
                current = current->next;
            }
        }
    }

    void VirtualMemoryAllocator::init(std::uintptr_t startVirtAddr, std::size_t size) noexcept {
        g_linkedListAllocator.Initialize(startVirtAddr, size);
    }

    void* VirtualMemoryAllocator::allocate(std::size_t size) noexcept {
        return g_linkedListAllocator.Allocate(size);
    }

    void VirtualMemoryAllocator::free(void* ptr) noexcept {
        g_linkedListAllocator.Free(ptr);
    }
}

void* operator new(std::size_t size) noexcept {
    return NOS::Memory::g_vmmAllocator.allocate(size);
}
void* operator new[](std::size_t size) noexcept {
    return NOS::Memory::g_vmmAllocator.allocate(size);
}
void operator delete(void* ptr) noexcept {
    NOS::Memory::g_vmmAllocator.free(ptr);
}
void operator delete(void* ptr, [[maybe_unused]] std::size_t size) noexcept {
    NOS::Memory::g_vmmAllocator.free(ptr);
}
void operator delete[](void* ptr) noexcept {
    NOS::Memory::g_vmmAllocator.free(ptr);
}
void operator delete[](void* ptr, [[maybe_unused]] std::size_t size) noexcept {
    NOS::Memory::g_vmmAllocator.free(ptr);
}