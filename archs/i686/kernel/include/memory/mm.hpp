#pragma once

#include <cstddef>
#include <cstdint>
#include <concepts>

#include <init.hpp>

namespace Memory {


    void* request_pages(std::size_t pages) noexcept;
}