#include <gdt.h>

#ifdef __cplusplus
extern "C" {
#endif

struct
#ifndef __VSC_LINTER__
__attribute__((packed))
#endif
{
    GDT_Entry entries[5];
} GDT;

GDT_Entry GDT_CreateEntry(const uint8_t base, const uint16_t limit, const uint8_t access, const uint8_t flags) {
    GDT_Entry entry;
    entry.limit_lo = limit & 0xFFFF;
    entry.base_lo = base & 0xFFFFFF;
    entry.access = access;
    entry.limit_hi = (limit >> 16) & 0xF;
    entry.flags = flags;
    entry.base_hi = (base >> 24) & 0xFF;
    
    return entry;
}

GDT_Descriptor GDT_Init() {
    GDT_Descriptor gdtPtr;
    
    GDT.entries[0] = GDT_CreateEntry(0,0,0,0);
    GDT.entries[1] = GDT_CreateEntry(0, 0xFFFF,
        GDT_AT_PRESENT | GDT_AT_READWRITE | GDT_PT_KERNEL | GDT_DT_SYSTEM | GDT_AT_EXECUTABLE | GDT_DIR_UP,
        GDT_F_GRANULARITY_PAGE | GDT_F_SIZE_32 | GDT_F_LIMIT);
    GDT.entries[2] = GDT_CreateEntry(0, 0xFFFF,
        GDT_AT_PRESENT | GDT_AT_READWRITE | GDT_PT_KERNEL | GDT_DT_SYSTEM | GDT_DIR_UP,
        GDT_F_GRANULARITY_PAGE | GDT_F_SIZE_32 | GDT_F_LIMIT);
    GDT.entries[3] = GDT_CreateEntry(0, 0xFFFF,
        GDT_AT_PRESENT | GDT_AT_READWRITE | GDT_PT_KERNEL | GDT_DT_USER | GDT_AT_EXECUTABLE | GDT_DIR_UP,
        GDT_F_GRANULARITY_PAGE | GDT_F_SIZE_32 | GDT_F_LIMIT);
    GDT.entries[4] = GDT_CreateEntry(0, 0xFFFF,
        GDT_AT_PRESENT | GDT_AT_READWRITE | GDT_PT_KERNEL | GDT_DT_USER | GDT_DIR_UP,
        GDT_F_GRANULARITY_PAGE | GDT_F_SIZE_32 | GDT_F_LIMIT
    );
    
    gdtPtr.limit = sizeof(GDT) - 1;
    gdtPtr.entry = (uintptr_t) &GDT;
    return gdtPtr;
}

#ifdef __cplusplus
}
#endif