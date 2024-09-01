#pragma once


enum GDT_PrivilegeType {
    GDT_PT_KERNEL      = 0b00000000, // ring 0
    GDT_PT_DRIVER      = 0b00100000, // ring 1
    GDT_PT_DRIVER_SAFE = 0b01000000, // ring 2
    GDT_PT_USER        = 0b01100000  // ring 3
};

enum GDT_DescriptorType {
    GDT_DT_SYSTEM = 0b00010000,
    GDT_DT_DATA   = 0b00000000
};

enum GDT_DirectionType {
    GDT_DIR_UP   = 0b00000000,
    GDT_DIR_DOWN = 0b00000100
};

enum GDT_SecurityRing {
    GDT_SR_SAME  = 0b00000000,
    GDT_SR_LOWER = 0b00000100
};

enum GDT_AccessType {
    GDT_AT_PRESENT          = 0b10000000,
    
    // Privilege levels
    
    // Descriptor types
    
    GDT_AT_EXECUTABLE     = 0b00001000,
    GDT_AT_NOT_EXECUTABLE = 0b00000000,

    // Security flags

    GDT_AT_READABLE       = 0b00000010,
    GDT_AT_NOT_READABLE   = 0b00000000,
    
    GDT_AT_WRITEABLE      = 0b00000010,
    GDT_AT_NOT_WRITEABLE  = 0b00000000,

    GDT_AT_ACCESSED       = 0b00000001,
    GDT_AT_NOT_ACCESSED   = 0b00000001,
};

enum GDT_Flags {
    GDT_F_GRANULARITY_BYTE = 0b0000,
    GDT_F_GRANULARITY_PAGE = 0b1000,

    GDT_F_SIZE_16BIT       = 0b0000,
    GDT_F_SIZE_32BIT       = 0b0100,
    
    GDT_F_64BIT_CODE       = 0b0010,
};

struct GDT_Entry {
    uint16_t limit_lo;
    uint32_t base_lo:24;
    uint8_t access;
    uint8_t limit_hi:4;
    uint8_t flags:4;
    uint8_t base_hi;
} __attribute__((packed));

struct GDT_Descriptor
{
    uint16_t limit;   // sizeof(gdt) - 1
    GDT_Entry* entry; // address of GDT
} __attribute__((packed));

GDT_Entry GDT_CreateEntry(const uint8_t base, const uint32_t limit, const uint8_t access, const uint8_t flags) noexcept {
    GDT_Entry entry;
    entry.limit_lo = limit & 0xFFFF;
    entry.limit_hi = ((limit >> 16) & 0xF) | (flags & 0xF0);

    entry.base_lo = base & 0xFFFF;
    entry.base_hi = (base >> 24) & 0xFF;

    entry.access = access;
    entry.flags = flags & 0xF;
    
    return entry;
}

bool GDT_Load(const GDT_Entry* gdt);