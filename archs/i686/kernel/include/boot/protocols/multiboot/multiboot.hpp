#pragma once

#ifdef __MULTIBOOT2__
#   include <boot/protocols/multiboot/multiboot2.h>
#elif defined(__MULTIBOOT__)
#   include <boot/protocols/multiboot/multiboot.h>
#else
#   error "No multiboot header defined. Please define either __MULTIBOOT2__ or __MULTIBOOT__."
#endif

#include <cstdint>

extern std::uint8_t __multiboot_start__[];
extern std::uint8_t __multiboot_end__[];