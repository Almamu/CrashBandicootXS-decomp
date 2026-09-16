#include "core.h"
#include "actor.h"

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

extern void *sub_8026EDC(s32 size);
extern struct actor *sub_800725C(struct actor *self);
extern void sub_8007AB4(void *arg0);
extern u8 gStaticData_087E3C44[];

/* Allocates a new `struct actor`-shaped object (`sub_8026EDC`),
 * initializes it via `sub_800725C` (already matched in graphics.c -
 * wires up `gStaticData_087E3BEC` and clears flags), then overwrites
 * its table with `gStaticData_087E3C44` instead and clears its
 * part-object fields via `sub_8007AB4` (already matched in
 * actor_part.c). `arg0` becomes `field_08`, `arg1`/`arg2` become the
 * Q8 `x`/`y` position. */
struct actor *sub_8008434(u16 arg0, u16 arg1, u16 arg2)
{
    struct actor *part = sub_8026EDC(0x40);

    sub_800725C(part);
    part->table = gStaticData_087E3C44;
    sub_8007AB4(part);
    part->field_08 = arg0;
    part->x = (s32)arg1 << 8;
    part->y = (s32)arg2 << 8;
    return part;
}
asm(".align 2, 0");
