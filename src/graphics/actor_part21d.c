#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part21c.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * `sub_80306AC`'s (actor_part21c.c) companion: advances
 * `gUnknown_03001548` by its per-frame delta the same way, but also
 * ramps `gUnknown_03001560` itself toward a fixed target (`0x98`,
 * +-1/frame). Drives a small phase counter (`gUnknown_03001570`) that,
 * on its "armed" phase (0), spawns an effect via `sub_802E62C` centered
 * on a fixed camera offset and advances a per-effect counter
 * (`gUnknown_03001574`) through a small weapon-kind table
 * (`gUnknown_03001568`)'s thresholds, otherwise just decrements the
 * phase. Always re-runs the position-easing helper `sub_8030E08`, and -
 * while `gUnknown_03001554` hasn't crossed its (lower) ceiling
 * `0x31FF` - re-arms the phase from the weapon table and fires the
 * state-3/table-index-0 transition on the tracker object
 * (`gUnknown_03001534`), same shape as `sub_80306AC`. Always finishes
 * with `sub_803171C` (the palette bank-1 flash-color select).
 *
 * The tracker transition is a `static inline` helper (state/table-index
 * as parameters): that is what makes the compiler materialize the
 * state constant right before its own store instead of hoisting its
 * address load ahead of it (the gap that kept this NAKED before). */
extern s32 GetAnimFrameBaseOffset(void *self);
extern void sub_803171C(void);
extern void sub_8030E08(void);
extern s32 sub_802E62C(s32 x, s32 y, s32 z);

extern s32 gUnknown_03001548;
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001554;
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern struct actor_self *gUnknown_03001534;
extern s32 gUnknown_03001570;
extern s32 gUnknown_03001574;
extern s32 *gUnknown_03001568;
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001544;

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

void sub_8030734(void)
{
    s32 v;
    gUnknown_03001548 += gUnknown_03001560;
    v = gUnknown_03001560;
    if (v <= 0x98)
        gUnknown_03001560 = v + 1;
    else
        gUnknown_03001560 = v - 1;

    if (gUnknown_03001570 == 0) {
        sub_802E62C(gUnknown_03001540 - 0xCDB, gUnknown_03001544 + 0x516D, gUnknown_03001548 - 10);
        if (++gUnknown_03001574 == gUnknown_03001568[2]) {
            gUnknown_03001574 = 0;
            gUnknown_03001570 = gUnknown_03001568[3];
        } else {
            gUnknown_03001570 = gUnknown_03001568[1];
        }
    } else {
        gUnknown_03001570--;
    }
    sub_8030E08();
    if (gUnknown_03001554 <= 0x31FF) {
        gUnknown_03001570 = gUnknown_03001568[4];
        gUnknown_03001574 = 0;
        BossSetState(3, 0);
    }
    sub_803171C();
}
