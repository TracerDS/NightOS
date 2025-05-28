#pragma once

#ifdef __MULTIBOOT2__
#   include <grub/multiboot2.h>
#elif defined(__MULTIBOOT__)
#   include <grub/multiboot.h>
#else
#   error "No multiboot header defined. Please define either __MULTIBOOT2__ or __MULTIBOOT__."
#endif

#include <cstdint>

extern std::uint8_t __multiboot_start__[];
extern std::uint8_t __multiboot_end__[];