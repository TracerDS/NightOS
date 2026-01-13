#pragma once

#include <cstddef>
#include <cstdint>

namespace NOS::Memory {
    class VirtualMemoryAllocator {
    public:
        void init(std::uintptr_t startVirtAddr, std::size_t size) noexcept;

        void* allocate(std::size_t size) noexcept;
        void free(void* ptr) noexcept;
    };

    extern VirtualMemoryAllocator g_vmmAllocator;
}

void* operator new(std::size_t size) noexcept;
void* operator new[](std::size_t size) noexcept;

void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, std::size_t size) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, std::size_t size) noexcept;