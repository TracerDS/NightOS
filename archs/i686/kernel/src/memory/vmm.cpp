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

        std::uintptr_t heapStart;
        std::uintptr_t heapEnd;
        NodeHeader* head;

        bool ExpandHeap(std::size_t size) noexcept;
    public:
        void Initialize() noexcept;
        void* Allocate(std::size_t size) noexcept;
        void Free(void* ptr) noexcept;
    };
    LinkedListAllocator g_linkedListAllocator{};

    void LinkedListAllocator::Initialize() noexcept {
        heapStart = VirtualMemoryAllocator::HEAP_VIRTUAL_START;
        heapEnd = heapStart;

        std::size_t initialSize = 4 * ByteUnits::KB4;
    
        if (!ExpandHeap(initialSize)) {
            Logger::LogError(
                "[VMM] Out of memory during heap initialization!\r\n",
                Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
                Terminal::VGAColor::VGA_COLOR_BLACK
            );
            Utils::Asm::KernelPanic();
            return;
        }
    }

    bool LinkedListAllocator::ExpandHeap(std::size_t size) noexcept {
        std::size_t pagesNeeded = Utils::align_up(size, ByteUnits::KB4) / ByteUnits::KB4;

        if ((heapEnd + pagesNeeded * ByteUnits::KB4) > VirtualMemoryAllocator::HEAP_VIRTUAL_END) {
            // Out of heap limit
            Logger::LogError(
                "[VMM] Heap virtual address space exhausted!\r\n"
            );
            return false;
        }

        auto physMem = g_pmmAllocator.request_pages(pagesNeeded);
        if (!physMem) {
            Logger::LogError(
                "[VMM] Out of memory during expansion!\r\n"
            );
            return false;
        }

        for (std::size_t i = 0; i < physMem.size(); ++i) {
            g_paging.map_page(
                physMem.ToAddress() + (i * ByteUnits::KB4),
                heapEnd + (i * ByteUnits::KB4),
                PageFlags::PAGE_PRESENT | PageFlags::PAGE_WRITE_BACK
            );
        }
        NodeHeader* newNode = reinterpret_cast<NodeHeader*>(heapEnd);
        newNode->size = pagesNeeded * ByteUnits::KB4 - sizeof(NodeHeader);
        newNode->isFree = true;
        newNode->next = nullptr;

        if (!head) {
            head = newNode;
        } else {
            NodeHeader* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }

        heapEnd += pagesNeeded * ByteUnits::KB4;
#ifdef __NOS_KERNEL_DEBUG__
        Logger::Log(
            "[VMM] Expanded heap by %lu bytes, new end: 0x%08lX\r\n",
            pagesNeeded * ByteUnits::KB4,
            heapEnd
        );
#endif
        return true;
    }
    
    void* LinkedListAllocator::Allocate(std::size_t size) noexcept {
        std::size_t alignedSize = Utils::align_up(size, alignof(std::max_align_t));

#ifdef __NOS_KERNEL_DEBUG__
        Logger::Log(
            "[VMM] Allocating %lu bytes (aligned to %lu bytes)\r\n",
            size,
            alignedSize
        );
#endif

        while (true) {
            NodeHeader* current = head;

            while (current) {
                if (current->isFree && current->size >= alignedSize) {
                    // Size + header + minimal split size;
                    // arbitrary number - in this case its 16 bytes
                    if (current->size >= alignedSize + sizeof(NodeHeader) + 16) {
                        // Split block
                        NodeHeader* newNode = reinterpret_cast<NodeHeader*>(
                            reinterpret_cast<std::uintptr_t>(current) +
                            sizeof(NodeHeader) + alignedSize
                        );
                        newNode->size = current->size - alignedSize - sizeof(NodeHeader);
                        newNode->isFree = true;
                        newNode->next = current->next;

                        current->size = alignedSize;
                        current->next = newNode;
                    }
                    current->isFree = false;
                    return reinterpret_cast<void*>(
                        reinterpret_cast<std::uintptr_t>(current) + sizeof(NodeHeader)
                    );
                }
                current = current->next;
            }

            // No free block found, expand heap
            std::size_t bytesNeeded = alignedSize + sizeof(NodeHeader);

            // Minimum expansion size
            if (bytesNeeded < ByteUnits::KB4)
                bytesNeeded = ByteUnits::KB4;

            if (!ExpandHeap(bytesNeeded)) {
                // Out of memory
                return nullptr;
            }
        }

        return nullptr;
    }
    
    void LinkedListAllocator::Free(void* ptr) noexcept {
        if (!ptr)
            return;

#ifdef __NOS_KERNEL_DEBUG__
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