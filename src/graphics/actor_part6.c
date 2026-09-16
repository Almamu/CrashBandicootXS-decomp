#include "core.h"

extern void *gUnknown_03001308;

/* If `gUnknown_03001308+0x2b` is nonzero, returns
 * `(gUnknown_03001308's sub-object)+0x34`'s low 2 bits minus 1;
 * otherwise returns those same low 2 bits unmodified. Same
 * `gUnknown_03001308` sub-object convention used throughout this ROM
 * region (see `sub_8007F78`/`sub_8006FE4`). */
s32 sub_8008408(void)
{
    if (*((u8 *)gUnknown_03001308 + 0x2b) == 0) {
        void *subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
        u8 byte2 = *((u8 *)subObj + 0x34);
        u32 result = ((u32)byte2 << 30) >> 30;
        return result;
    } else {
        void *subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
        u8 byte2 = *((u8 *)subObj + 0x34);
        u32 result = ((u32)byte2 << 30) >> 30;
        return result - 1;
    }
}
asm(".align 2, 0");
