#pragma once

#include <init.hpp>
#include <cstdint>

namespace Memory {    
    namespace Allocators::LinkedList {
        void Init();
        void* kmalloc(std::size_t size) noexcept;
        void kfree(void* ptr) noexcept;
    }
}