#include "core.h"

/* Same boss-weapon "self" object family as actor_part20.c - see that
 * file's header comment and docs/matching/issue-58-0x08030334-actor.md.
 * `gUnknown_03001534` here is a *separate*, smaller (0x1c-byte) tracker
 * object of the same shape, allocated by `sub_8030F88` (left raw in
 * this chunk). */

extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001558;
extern s32 gUnknown_03001544;
extern s32 gUnknown_0300155C;
extern s32 gUnknown_03001548;
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern void *gUnknown_03001534;

/* Advances the boss-weapon's screen-space accumulators
 * (`gUnknown_03001540`/`gUnknown_03001544`/`gUnknown_03001548`) by
 * their per-frame deltas, clamps the `gUnknown_0300155C` ramp to
 * `0x140`, and - once `gUnknown_03001544` exceeds a threshold - resets
 * the small tracker object at `gUnknown_03001534` to state 0 (table-
 * index 0) and clears `DISPCNT`'s bit 10 (window/mosaic-family bit not
 * otherwise named yet). */
void sub_8030C98(void)
{
    s32 total;
    s32 delta;

    gUnknown_03001540 += gUnknown_03001558;

    total = gUnknown_03001544 + gUnknown_0300155C;
    gUnknown_03001544 = total;

    gUnknown_03001548 += gUnknown_03001560;

    delta = gUnknown_0300155C + 7;
    gUnknown_0300155C = delta;
    if (delta > 0x140) {
        gUnknown_0300155C = 0x140;
    }

    if (total > 0xbb80) {
        u8 *self;
        register s32 zero asm("r5") = 0;

        gUnknown_03001538 = zero;
        gUnknown_0300153C = zero;

        self = gUnknown_03001534;
        *(s32 *)(self + 0xc) = zero;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }

        {
            s32 frame = GetAnimFrameBaseOffset(self);
            register s32 idx asm("r2") = *(s32 *)(self + 0xc);
            register u8 *table asm("r3") = *(u8 **)self;
            register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
            register s32 four asm("r2");
            register s32 val asm("r1");

            asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
            four = 4;
            val = *(s16 *)(entryPtr + four);

            if (frame >= val) {
                *(s32 *)(self + 8) = zero;
            }
        }

        REG_DISPCNT &= 0xfbff;
    }
}
