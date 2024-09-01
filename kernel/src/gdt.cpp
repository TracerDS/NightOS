#include <gdt.hpp>
#include <stdint.h>

extern bool _loadGDT(const GDT_Entry* gdt);

bool GDT_Load(const GDT_Entry* gdt) {
    return _loadGDT(gdt);
}


GDT_Entry g_GDT[] = {
    // null descriptor
    GDT_CreateEntry(0,0,0,0),

    // Kernel 32bit code segment
    GDT_CreateEntry (
        0,
        0xFFFFF,
        GDT_AT_PRESENT | GDT_PT_KERNEL | GDT_DT_DATA | GDT_AT_READABLE,
        GDT_F_SIZE_32BIT | GDT_F_GRANULARITY_PAGE
    ),

    // Kernel 32-bit data segment
    GDT_CreateEntry(
        0,
        0xFFFFF,
        GDT_AT_PRESENT | GDT_PT_KERNEL | GDT_DT_DATA | GDT_AT_WRITEABLE,
        GDT_F_SIZE_32BIT | GDT_F_GRANULARITY_PAGE
    ),
};

GDT_Descriptor g_GDTDescriptor = { sizeof(g_GDT) - 1, g_GDT };