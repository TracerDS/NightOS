#pragma once

#include <cstddef>
#include <cstdint>
#include <concepts>

#include <init.hpp>

struct multiboot_info;

namespace Memory {
    void Init(struct multiboot_info* mb_info) noexcept;

    void mark_page(std::uintptr_t addr, bool used) noexcept;
    void* request_pages(std::size_t pages) noexcept;
}