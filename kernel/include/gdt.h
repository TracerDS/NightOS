#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GDT_AT_PRESENT = 0b10000000,
    GDT_AT_EXECUTABLE = 0b00001000,
    GDT_AT_READABLE = 0b00000010,
    GDT_AT_WRITEABLE = 0b00000010,
    GDT_AT_READWRITE = 0b00000010,
    GDT_AT_ACCESSED = 0b00000001
} GDT_AccessType;

typedef enum {
    GDT_PT_KERNEL =      0b00000000,
    GDT_PT_DRIVER =      0b00100000,
    GDT_PT_DRIVER_SAFE = 0b01000000,
    GDT_PT_USER =        0b01100000
} GDT_PrivilegeType;

typedef enum {
    GDT_DT_SYSTEM = 0b00010000,
    GDT_DT_USER = 0b00000000
} GDT_DescriptorType;

typedef enum {
    GDT_DIR_UP = 0b00000000,
    GDT_DIR_DOWN = 0b00000100
} GDT_DirectionType;

typedef enum {
    GDT_SR_SAME = 0b00000000,
    GDT_SR_LOWER = 0b00000100
} GDT_SecurityRing;

typedef enum {
    GDT_F_GRANULARITY_BYTE = 0b00000000,
    GDT_F_GRANULARITY_PAGE = 0b10000000,
    GDT_F_SIZE_16          = 0b00000000,
    GDT_F_SIZE_32          = 0b01000000,
    GDT_F_LONG_64          = 0b00100000,
    GDT_F_LONG_DATA        = 0b00000000,
    GDT_F_LIMIT            = 0b00001111,
} GDT_Flags;

#ifdef __VSC_LINTER__
struct GDT_Entry
#else
typedef struct
#endif
{
    uint16_t limit_lo;
    uint32_t base_lo:24;
    uint8_t access;
    uint8_t limit_hi:4;
    uint8_t flags:4;
    uint8_t base_hi;
}
#ifndef __VSC_LINTER__
__attribute__((packed)) GDT_Entry
#endif
;

#ifdef __VSC_LINTER__
struct TSS_Entry
#else
typedef struct
#endif
{
    GDT_Entry entry;
    uint32_t base;
    uint32_t reserved;
}
#ifndef __VSC_LINTER__
__attribute__((packed)) TSS_Entry
#endif
;

#ifdef __VSC_LINTER__
struct GDT_Descriptor
#else
typedef struct
#endif
{
    uint16_t limit;
    uintptr_t entry;
}
#ifndef __VSC_LINTER__
__attribute__((packed)) GDT_Descriptor
#endif
;

GDT_Descriptor GDT_Init();
GDT_Entry GDT_CreateEntry(const uint8_t base, const uint16_t limit, const uint8_t access, const uint8_t flags = 0);

#ifdef __cplusplus
}
#endif