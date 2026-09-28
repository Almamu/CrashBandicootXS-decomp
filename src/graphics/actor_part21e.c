#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part21c.c/actor_part21d.c - see actor_part20.c's header
 * comment and docs/matching/issue-58-0x08030334-actor.md.
 *
 * `sub_80306AC`/`sub_8030734`'s third sibling: advances
 * `gUnknown_03001548` by its per-frame delta and ramps
 * `gUnknown_03001560` toward `0xb2` the same +-1/frame way. While the
 * phase counter (`gUnknown_03001570`) is armed (0), computes the
 * player's (`gUnknown_03000884`) distance from a target point
 * (`+0x24` axis, offset `+0xa` minus the accumulated position) via
 * `sub_803ADB4`, and - only once that "speed" term is positive -
 * computes a signed Manhattan-style distance in X/Y (`+0x1c`/`+0x20`
 * against `gUnknown_03001540`/`gUnknown_03001544`, scaled by the speed
 * term, `abs`-combined) and, while under a `0x7FF` threshold, spawns an
 * effect via `sub_802E674` (the 5-argument, velocity-carrying sibling of `sub_8030734`'s
 * `sub_802E62C`) and advances the same `gUnknown_03001574` counter
 * through the weapon table's next threshold slot (`+0x14`/`+0x18`/
 * `+0x10`). Otherwise the phase just decrements. Always re-runs
 * `sub_8030E08` and, past a higher position ceiling (`0x4300`),
 * re-arms the phase from the weapon table (`+4`) and fires the
 * state-2/table-index-0 transition on the tracker object, then always
 * finishes with `sub_803171C`.
 *
 * The distances are written `a - (b - K)`: gcc's `fold` reassociates
 * that into `(a + K) - b`, which is exactly what keeps the ROM from
 * CSE-ing it with the spawn call's own `b - K` arguments (written
 * `(a + K) - b` directly, the compiler shares `b - K` instead). The
 * `/` goes through the ROM's own `__divsi3` (`sub_803ADB4`) and the
 * absolute values are the branchless `asrs`/`eors`/`subs` form. */
extern s32 gUnknown_03001548;
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001570;
extern struct actor_self *gUnknown_03000884;
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001544;
extern s32 sub_802E674(s32 x, s32 y, s32 z, s32 dx, s32 dy);
extern s32 gUnknown_03001574;
extern s32 *gUnknown_03001568;
extern s32 gUnknown_03001554;
extern struct actor_self *gUnknown_03001534;
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern void sub_8030E08(void);
extern void sub_803171C(void);

asm(".set __divsi3, sub_803ADB4");

static inline void BossSetState(s32 st, s32 idx)
{
    struct actor_self *self;
    gUnknown_03001538 = st;
    gUnknown_0300153C = 0;
    self = gUnknown_03001534;
    self->animIndex = idx;
    self->animTimer = self->anims[idx].duration;
    self->animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold)
        self->animTime = 0;
}

static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

void sub_8030834(void)
{
    s32 v;
    s32 phase;

    gUnknown_03001548 += gUnknown_03001560;
    v = gUnknown_03001560;
    if (v <= 0xb2)
        gUnknown_03001560 = v + 1;

    phase = gUnknown_03001570;
    if (phase == 0) {
        struct actor_self *pl = gUnknown_03000884;
        s32 speed = (pl->z - (gUnknown_03001548 - 10)) / -0x1AA;
        if (speed > 0) {
            s32 dx, dy;
            speed = 0x1000 / speed;
            dx = ((pl->x - (gUnknown_03001540 - 0xCDB)) * speed) >> 12;
            dy = ((pl->y - (gUnknown_03001544 + 0x516D)) * speed) >> 12;
            if (Abs(dx) + Abs(dy) <= 0x7FF) {
                sub_802E674(gUnknown_03001540 - 0xCDB, gUnknown_03001544 + 0x516D, gUnknown_03001548 - 10, dx, dy);
                if (++gUnknown_03001574 == gUnknown_03001568[5]) {
                    gUnknown_03001574 = phase;
                    gUnknown_03001570 = gUnknown_03001568[6];
                } else {
                    gUnknown_03001570 = gUnknown_03001568[4];
                }
            }
        }
    } else {
        gUnknown_03001570 = phase - 1;
    }
    sub_8030E08();
    if (gUnknown_03001554 > 0x4300) {
        gUnknown_03001570 = gUnknown_03001568[1];
        gUnknown_03001574 = 0;
        BossSetState(2, 0);
    }
    sub_803171C();
}
