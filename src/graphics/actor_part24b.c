#include "core.h"
#include "actor_self.h"

/* Same "boss-weapon self" object family as actor_part20.c (see that
 * file's header comment and docs/matching/issue-58-0x08030334-actor.md),
 * and the same 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` AABB-overlap
 * shape as `sub_802DD9C`/`sub_802D7B0` (actor_part75.c/actor_part74.c,
 * see docs/matching/issue-54-actor-d3a8.md) - only runs while the small
 * tracker object's state global (`gUnknown_03001538`) is 2 or 3. Box A:
 * `gStaticData_0817C3D8` (a fixed keyframe-table box) with the boss-
 * weapon's own screen-space accumulators (`gUnknown_03001540`/`0x1544`/
 * `0x1548`, all `>>8`) added into its `x`/`y`/`z`. Box B: `self+0x38`'s
 * own 12-byte vector, with `self`'s own `+0x1c`/`0x20`/`0x24` position
 * (all `>>8`) added into all three of `x`/`y`/`z`, then copied through
 * `sub_800014C`'s self-copy idiom before the 3-axis overlap test.
 * Returns 1 only when all three axes overlap.
 *
 * Same frame-struct shape as `sub_802DD9C`/`sub_802D7B0`: the three
 * boxes are members of one stack struct so their addresses are
 * rematerialized from sp, and only the copied box's address stays live
 * across the `sub_800014C` call. Built with old_agbcc
 * (docs/matching/issue-58-61-naked-retry.md). */
extern u8 gStaticData_0817C3D8[];
extern s32 gUnknown_03001538;
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001544;
extern s32 gUnknown_03001548;
extern void *sub_800014C(void *dest, void *src, s32 size);

struct box3 {
    s16 x, y, z;
    s16 w, h, d;
};

static inline void BoxOffset(struct box3 *b, s32 x, s32 y, s32 z)
{
    b->x += x;
    b->y += y;
    b->z += z;
}

u8 sub_8031378(void *selfArg)
{
    struct actor_self *self = selfArg;

    if ((u32)(gUnknown_03001538 - 2) <= 1) {
        struct {
            struct box3 a, c, t;
        } f;
        struct box3 *pa, *pc;

        f.a = *(struct box3 *)gStaticData_0817C3D8;
        BoxOffset(&f.a, gUnknown_03001540 >> 8, gUnknown_03001544 >> 8, gUnknown_03001548 >> 8);
        f.t = *(struct box3 *)self->unk_38;
        BoxOffset(&f.t, self->x >> 8, self->y >> 8, self->z >> 8);
        f.c = f.t;
        pc = &f.c;
        sub_800014C(pc, pc, sizeof(*pc));
        pa = &f.a;
        if (pa->z < pc->z + pc->d && pa->z + pa->d > pc->z
         && pa->y < pc->y + pc->h && pa->y + pa->h > pc->y
         && pa->x < pc->x + pc->w && pa->x + pa->w > pc->x)
            goto hit;
    }
    return 0;
hit:
    return 1;
}

asm(".align 2, 0");
