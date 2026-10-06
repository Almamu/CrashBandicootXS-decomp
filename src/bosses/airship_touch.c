#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "system.h"
#include "bosses.h"

/* Same "boss-weapon self" object family as airship_fireball.c (see that
 * file's header comment and docs/matching/archive/issue-58-0x08030334-actor.md),
 * and the same 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` AABB-overlap
 * shape as `IsTouchingYeti`/`UpdateYeti` (yeti_graphics.c/yeti_update.c,
 * see docs/matching/archive/issue-54-actor-d3a8.md) - only runs while the small
 * tracker object's state global (`gAirshipState`) is 2 or 3. Box A:
 * `gAirshipBox` (a fixed keyframe-table box) with the boss-
 * weapon's own screen-space accumulators (`gAirshipX`/`0x1544`/
 * `0x1548`, all `>>8`) added into its `x`/`y`/`z`. Box B: `self+0x38`'s
 * own 12-byte vector, with `self`'s own `+0x1c`/`0x20`/`0x24` position
 * (all `>>8`) added into all three of `x`/`y`/`z`, then copied through
 * `MemCopy32`'s self-copy idiom before the 3-axis overlap test.
 * Returns 1 only when all three axes overlap.
 *
 * Same frame-struct shape as `IsTouchingYeti`/`UpdateYeti`: the three
 * boxes are members of one stack struct so their addresses are
 * rematerialized from sp, and only the copied box's address stays live
 * across the `MemCopy32` call. Built with old_agbcc
 * (docs/matching/archive/issue-58-61-naked-retry.md). */

static inline void BoxOffset(struct anim_box *b, s32 x, s32 y, s32 z)
{
    b->x += x;
    b->y += y;
    b->z += z;
}

u8 IsTouchingAirship(void *selfArg)
{
    struct actor_self *self = selfArg;

    if ((u32)(gAirshipState - 2) <= 1) {
        struct {
            struct anim_box a, c, t;
        } f;
        struct anim_box *pa, *pc;

        f.a = gAirshipBox;
        BoxOffset(&f.a, gAirshipX >> 8, gAirshipY >> 8, gAirshipZ >> 8);
        f.t = *(struct anim_box *)self->box;
        BoxOffset(&f.t, self->x >> 8, self->y >> 8, self->z >> 8);
        f.c = f.t;
        pc = &f.c;
        MemCopy32(pc, pc, sizeof(*pc));
        pa = &f.a;
        if (pa->z < pc->z + pc->d && pa->z + pa->d > pc->z && pa->y < pc->y + pc->h &&
            pa->y + pa->h > pc->y && pa->x < pc->x + pc->w && pa->x + pa->w > pc->x)
            goto hit;
    }
    return 0;
hit:
    return 1;
}

asm(".align 2, 0");
