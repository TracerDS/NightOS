#pragma once

#include <cstddef>
#include <cstdint>

namespace NOS::Memory {
    class VirtualMemoryAllocator {
    public:
        static constexpr std::uintptr_t HEAP_VIRTUAL_START = 0xD0000000;
        static constexpr std::uintptr_t HEAP_VIRTUAL_END   = 0xE0000000;
    public:
        void init() noexcept;

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