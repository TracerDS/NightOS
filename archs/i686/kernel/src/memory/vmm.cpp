#include <memory/vmm.hpp>
#include <memory/pmm.hpp>
#include <memory/paging.hpp>
#include <logger.hpp>
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
        void Initialize() noexcept;
        void* Allocate(std::size_t size) noexcept;
        void Free(void* ptr) noexcept;
    };
    LinkedListAllocator g_linkedListAllocator{};

    void LinkedListAllocator::Initialize() noexcept {
        auto startVirtAddr = VirtualMemoryAllocator::HEAP_VIRTUAL_START;
        auto size = VirtualMemoryAllocator::HEAP_VIRTUAL_END - startVirtAddr;
    
#ifdef __KERNEL_DEBUG__
        Logger::Log(
            "[VMM] Initializing VMM heap at 0x%08lX, size %lu bytes\r\n",
            startVirtAddr,
            size
        );
#endif

        auto physAddr = g_pmmAllocator.request_pages(size / ByteUnits::KB4);
        if (!physAddr) {
            // No memory. Panic
            IO::kprintf_color(
                "[VMM] Out of memory during initialization!\r\n",
                Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
                Terminal::VGAColor::VGA_COLOR_BLACK
            );
            Utils::Asm::KernelPanic();
            return;
        }

        for (std::size_t i = 0; i < physAddr.size(); ++i) {
            // Map it
            g_paging.map_page(
                physAddr.ToAddress() + (i * ByteUnits::KB4),
                startVirtAddr + (i * ByteUnits::KB4), 
                PageFlags::PAGE_PRESENT | PageFlags::PAGE_READ_WRITE
            );
        }

        baseAddr = reinterpret_cast<void*>(startVirtAddr);
        totalSize = size;

        // Create the first, large, free block
        head = reinterpret_cast<NodeHeader*>(startVirtAddr);
        head->size = size - sizeof(NodeHeader);
        head->isFree = true;
        head->next = nullptr;
    }
    
    void* LinkedListAllocator::Allocate(std::size_t size) noexcept {
        std::size_t alignedSize = Utils::align_up(size, alignof(std::max_align_t));

#ifdef __KERNEL_DEBUG__
        Logger::Log(
            "[VMM] Allocating %lu bytes (aligned to %lu bytes)\r\n",
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

#ifdef __KERNEL_DEBUG__
        Logger::Log(
            "[VMM] Freeing memory at 0x%08lX\r\n",
            reinterpret_cast<std::uintptr_t>(ptr)
        );
#endif

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

    void VirtualMemoryAllocator::init() noexcept {
        g_linkedListAllocator.Initialize();
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