#include "core.h"
#include "actor_self.h"

extern s16 gSineTable[];

/* Eases `self`'s cached position (`self+0x1c`/`0x20`/`0x24`, the same
 * fields `InitActorPart` caches its `b`/`c`/`d` constructor arguments
 * into, per actor_part50.c) toward a caller-supplied target, with the
 * exact target/mode selected by `self+0x28` ("state"):
 *   - state 0: eases toward `posX`/`posY` offset by a per-frame-counter
 *     (`self+0x44`) lookup into `gSineTable` (two different
 *     index strides for the X/Y offsets, producing a scatter/orbit-style
 *     curve), and toward `posZ-0x200`.
 *   - state 1: snaps (no easing) directly to `posX` for the X axis, to
 *     `posY` plus a different table-driven offset for Y, and to
 *     `posZ+0x200` for Z.
 *   - any other state: eases toward `posX`/`posY` directly (no table
 *     offset), and toward `posZ+0x200`.
 * "Easing" is a round-toward-zero divide (by 16 for X/Y, by 4 for Z) of
 * the remaining delta, added back onto the cached position - the ROM's
 * own rsb/lsr/add/asr rounding idiom, same shape as `SetEntitySize` in
 * docs/matching.md.
 *
 * No register pins needed: the table offsets go through their own locals
 * (`ox`/`oy`) so they are summed before the position is added, as the
 * ROM does, and the explicit `goto ease_y` reproduces the ROM sharing
 * one copy of the Y/Z easing between state 0 and the default case. */
void MovePolarAkuAku(struct actor_self *self, s32 posX, s32 posY, s32 posZ)
{
    s32 tx, ty, tz;
    s32 cur, d;

    if (self->state == 0) {
        s32 ox = gSineTable[(self->stateTime * 4) & 0xff] * 24 - 0x1000;
        s32 oy;

        tx = posX + ox;
        oy = gSineTable[(self->stateTime * 2) & 0xff] * 10 - 0x1e00;
        ty = posY + oy;
        tz = posZ - 0x200;
        self->x += (tx - self->x) / 16;
        cur = self->y;
        d = ty - cur;
        goto ease_y;
    } else if (self->state == 1) {
        s32 oy;

        self->x = posX;
        oy = gSineTable[(self->stateTime * 9) & 0xff] * 4 - 0xa00;
        self->y = oy + posY;
        self->z = posZ + 0x200;
        return;
    }
    tz = posZ + 0x200;
    self->x += (posX - self->x) / 16;
    cur = self->y;
    d = posY - cur;
ease_y:
    self->y = cur + d / 16;
    self->z += (tz - self->z) / 4;
}

asm(".align 2, 0");
