#include <memory/vmm.hpp>
#include <memory/pmm.hpp>
#include <memory/paging.hpp>

#include <io.hpp>

#include <cstddef>
#include <cstdint>

namespace Memory {
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
        void Initialize(std::uintptr_t startVirtAddr, std::size_t initialSize) noexcept;
        void* Allocate(std::size_t size) noexcept;
        void Free(void* ptr) noexcept;
    };
    LinkedListAllocator g_linkedListAllocator{};

    void LinkedListAllocator::Initialize(std::uintptr_t startVirtAddr, std::size_t initialSize) noexcept {
        // Assume we are mapped in already
        auto pagesNeeded = Utils::align_up(initialSize, Paging::ByteUnits::KB4) / Paging::ByteUnits::KB4;
    
        for (std::size_t i = 0; i < pagesNeeded; ++i) {
            // Get the physical page
            auto physAddr = g_pmmAllocator.request_pages(1);
            if (!physAddr) {
                // No memory. Panic
                Utils::Asm::KernelPanic();
                return;
            }
            
#ifdef __KERNEL_DEBUG__
            IO::kprintf("VMM: Mapping virtual 0x%lX to physical 0x%lX\r\n",
                startVirtAddr + (i * Paging::ByteUnits::KB4),
                reinterpret_cast<std::uintptr_t>(physAddr.data())
            );
#endif

            // Map it
            Paging::g_paging.map_page(
                reinterpret_cast<std::uintptr_t>(physAddr.data()), 
                startVirtAddr + (i * Paging::ByteUnits::KB4), 
                Paging::PageFlags::PAGE_PRESENT | Paging::PageFlags::PAGE_READ_WRITE
            );
        }

        baseAddr = reinterpret_cast<void*>(startVirtAddr);
        totalSize = initialSize;

        // Create the first, large, free block
        head = reinterpret_cast<NodeHeader*>(startVirtAddr);
        head->size = initialSize - sizeof(NodeHeader);
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
    return Memory::g_vmmAllocator.allocate(size);
}
void* operator new[](std::size_t size) noexcept {
    return Memory::g_vmmAllocator.allocate(size);
}
void operator delete(void* ptr) noexcept {
    Memory::g_vmmAllocator.free(ptr);
}
void operator delete(void* ptr, [[maybe_unused]] std::size_t size) noexcept {
    Memory::g_vmmAllocator.free(ptr);
}
void operator delete[](void* ptr) noexcept {
    Memory::g_vmmAllocator.free(ptr);
}
void operator delete[](void* ptr, [[maybe_unused]] std::size_t size) noexcept {
    Memory::g_vmmAllocator.free(ptr);
}