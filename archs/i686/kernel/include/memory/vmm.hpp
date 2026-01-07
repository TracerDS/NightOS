#pragma once

#include <cstddef>
#include <cstdint>

namespace Memory::VMM {
    void Init(std::uintptr_t startAddr, std::size_t size) noexcept;
}

void* operator new(size_t size) noexcept;
void* operator new[](std::size_t size) noexcept;

void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, std::size_t size) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, std::size_t size) noexcept;