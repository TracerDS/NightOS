#include <gdt.h>

#ifdef __cplusplus
extern "C" {
#endif

const GDT_Entry GDT_NULL = GDT_CreateEntry(0,0,0,0);
const GDT_Entry GDT_CodeKernel32 = GDT_CreateEntry(0, 0xFFFF,
    GDT_AT_PRESENT | GDT_AT_READWRITE | GDT_PT_KERNEL | GDT_DT_SYSTEM | GDT_AT_EXECUTABLE | GDT_DIR_UP,
    GDT_F_GRANULARITY_PAGE | GDT_F_SIZE_32 | GDT_F_LIMIT);
const GDT_Entry GDT_DataKernel32 = GDT_CreateEntry(0, 0xFFFF,
    GDT_AT_PRESENT | GDT_AT_READWRITE | GDT_PT_KERNEL | GDT_DT_SYSTEM | GDT_DIR_UP,
    GDT_F_GRANULARITY_PAGE | GDT_F_SIZE_32 | GDT_F_LIMIT);
const GDT_Entry GDT_CodeUser32 = GDT_CreateEntry(0, 0xFFFF,
    GDT_AT_PRESENT | GDT_AT_READWRITE | GDT_PT_KERNEL | GDT_DT_USER | GDT_AT_EXECUTABLE | GDT_DIR_UP,
    GDT_F_GRANULARITY_PAGE | GDT_F_SIZE_32 | GDT_F_LIMIT);
const GDT_Entry GDT_DataUser32 = GDT_CreateEntry(0, 0xFFFF,
    GDT_AT_PRESENT | GDT_AT_READWRITE | GDT_PT_KERNEL | GDT_DT_USER | GDT_DIR_UP,
    GDT_F_GRANULARITY_PAGE | GDT_F_SIZE_32 | GDT_F_LIMIT);

GDT_Entry g_GDT[5] = {
    GDT_NULL,
    GDT_CodeKernel32,
    GDT_DataKernel32,
    GDT_CodeUser32,
    GDT_DataUser32,
}

#ifdef __cplusplus
}
#endif