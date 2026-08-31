#pragma once

#include <core/init.hpp>

namespace NOS::Interrupts::HPET {
    struct PACKED address_structure {
        std::uint8_t address_space_id;    // 0 - system memory, 1 - system I/O
        std::uint8_t register_bit_width;
        std::uint8_t register_bit_offset;
        std::uint8_t reserved;
        std::uint64_t address;
    };

    struct PACKED description_table_header {
        char signature[4];    // 'HPET' in case of HPET table
        std::uint32_t length;
        std::uint8_t revision;
        std::uint8_t checksum;
        char oemid[6];
        std::uint64_t oem_tableid;
        std::uint32_t oem_revision;
        std::uint32_t creator_id;
        std::uint32_t creator_revision;
    };

    struct PACKED hpet : public description_table_header {
        std::uint8_t hardware_rev_id;
        std::uint8_t comparator_count   : 5;
        std::uint8_t counter_size       : 1;
        std::uint8_t reserved           : 1;
        std::uint8_t legacy_replacement : 1;
        std::uint16_t pci_vendor_id;
        address_structure address;
        std::uint8_t hpet_number;
        std::uint16_t minimum_tick;
        std::uint8_t page_protection;
    };
}