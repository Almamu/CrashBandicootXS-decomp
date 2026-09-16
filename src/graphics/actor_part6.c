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

/* Always-true stub. */
s32 sub_8008480(void)
{
    return 1;
}

extern void sub_8026ED0(void *arg0);
extern u8 gStaticData_087E3BEC[];

/* Same `gStaticData_087E3BEC`/conditional-`sub_8026ED0` shape as
 * `sub_80073BC` (already matched in `graphics.c`). */
void sub_8008484(struct actor *self, u32 arg1)
{
    self->table = gStaticData_087E3BEC;
    if (arg1 & 1) {
        sub_8026ED0(self);
    }
}

/* Same `sub_800725C`/table-swap/`sub_8007AB4` shape as `sub_8008434`
 * above, but re-initializes an existing `self` instead of allocating
 * a new one. */
struct actor *sub_80084A4(struct actor *self)
{
    sub_800725C(self);
    self->table = gStaticData_087E3C44;
    sub_8007AB4(self);
    return self;
}

extern void *sub_80083B8(void *part);
extern u8 gStaticData_0816B300[];

/* Looks up `part`'s keyframe record via `sub_80083B8` (already parked
 * as `NON_MATCHING` in `actor_part5.c`), then picks a pointer off it
 * per the record's `+4` byte's upper nibble: 0 -> `info+0x24`, 6 ->
 * `info+0x14`, anything else (1-5, or above 6) -> the fixed fallback
 * table `gStaticData_0816B300`. Needed the case labels scattered
 * out of numeric order (rather than grouped into the obvious
 * contiguous "0 / 1-5 / 6" ranges) to get gcc to emit a real jump
 * table instead of a compare chain - this compiler only builds a
 * jump table when the case-to-block mapping can't be expressed as a
 * few simple range checks, so a source-level shape that *looks*
 * needlessly scattered is what is needed to match the ROM's own
 * jump table here. */
void *sub_80084C4(void *part)
{
    void *info = sub_80083B8(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (u8 *)info + 0x24;
        break;
    case 3:
    case 4:
        result = gStaticData_0816B300;
        break;
    case 1:
    case 2:
        result = gStaticData_0816B300;
        break;
    case 5:
        result = gStaticData_0816B300;
        break;
    case 6:
        result = (u8 *)info + 0x14;
        break;
    default:
        result = gStaticData_0816B300;
        break;
    }
    return result;
}

extern u8 gStaticData_0816B2F8[];

/* Same `sub_80083B8`-derived-record-nibble-switch shape as
 * `sub_80084C4` above, with a different result mapping: 0 and 4
 * select `info+0x1c`, anything else falls back to
 * `gStaticData_0816B2F8`. Unlike `sub_80084C4`, no case-scattering
 * trick was needed here - 0 and 4 are already non-adjacent, which is
 * enough on its own to make gcc emit a jump table instead of a
 * compare chain. */
void *sub_8008518(void *part)
{
    void *info = sub_80083B8(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (u8 *)info + 0x1c;
        break;
    case 1:
    case 2:
    case 3:
        result = gStaticData_0816B2F8;
        break;
    case 4:
        result = (u8 *)info + 0x1c;
        break;
    case 5:
    case 6:
        result = gStaticData_0816B2F8;
        break;
    default:
        result = gStaticData_0816B2F8;
        break;
    }
    return result;
}
asm(".align 2, 0");
