#include "core.h"

/* Tail continuation of GitHub issue #50's chunk
 * (asm/code_3_2_20_8b7c_ac28.s, ROM 0x0802AC28-0x0802BED8): the giant
 * `sub_802AC28` kind-dispatch constructor and the run of "actor part
 * factory"/animation-table-state functions between it and here
 * (`sub_802B12C`-`sub_802BBE4`) are still raw - this file only covers
 * the literal tail of that raw `.s` file, a self-contained run of
 * accumulator-drain/hazard-threshold helpers on the same `self` object
 * family documented in actor_part19.c/actor_part44.c, operating on the
 * `gUnknown_0300148x`/`gUnknown_030014Ax` global cluster those files
 * already established (`gUnknown_03001488`'s "reward" accumulator,
 * `gUnknown_030014A0`-`030014A4`'s lock/hazard-latch quintet). See
 * docs/rom_map.md's "boss's BG2 spin/zoom effect..." section, which
 * already reads `sub_802BC68` as one of a matched pair of accumulator-
 * drain/reward-dispenser functions (the other being `sub_802F3BC` in
 * actor_part44.c) and `sub_802B174` as a "spawn effect type N" family
 * member - both confirmed here by this function's own body. */

extern s32 gUnknown_03001488;
extern u8 gUnknown_030014A0;
extern s32 gUnknown_03001484;
extern void *gUnknown_030012C0;
extern void *gUnknown_030012BC;

extern s32 sub_8023430(void *self);
extern void sub_802B174(s32 a, s32 b, s32 c);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_8029BAC(s32 arg0);

/* Accumulator-drain/reward-dispenser for the `gUnknown_03001488`
 * accumulator (filled by `sub_802C078`, still raw): while the "locked"
 * flag `gUnknown_030014A0` is set, fully drains it via repeated
 * `sub_8023430` calls without spawning anything; otherwise, once the
 * `gUnknown_03001484` cooldown elapses, dispenses one of four tiers of
 * reward (via `sub_802B174` at `self`'s position) sized by the
 * accumulator's own magnitude, and plays a cue. Exact structural twin
 * of `sub_802F3BC` (actor_part44.c) on a different accumulator/cooldown
 * pair - see docs/rom_map.md. */
void sub_802BC68(void *selfArg)
{
    register u8 *self asm("r1") = selfArg;
    s32 acc = gUnknown_03001488;

    if (acc == 0) {
        return;
    }

    if (gUnknown_030014A0 != 0) {
        do {
            sub_8023430(gUnknown_030012C0);
            gUnknown_03001488--;
        } while (gUnknown_03001488 != 0);
        return;
    }

    if (gUnknown_03001484 != 0) {
        gUnknown_03001484--;
        return;
    }

    gUnknown_03001484 = 0xf;

    if (acc <= 9) {
        sub_802B174(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), 1);
        gUnknown_03001488 -= 1;
    } else if (acc <= 0x13) {
        sub_802B174(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), 2);
        gUnknown_03001488 -= 2;
    } else if (acc <= 0x27) {
        sub_802B174(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), 4);
        gUnknown_03001488 -= 4;
    } else {
        sub_802B174(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), 8);
        gUnknown_03001488 -= 8;
    }

    PlaySfx(gUnknown_030012BC, 8, 0x100);
}

extern u8 gUnknown_03001480;

/* Trivial byte getter - `sub_802A688` (actor_part94.c) is a NAKED
 * trampoline that calls this through the player pointer. */
u8 sub_802BD18(void)
{
    return gUnknown_03001480;
}

extern u8 gUnknown_030014A3;

/* Frame-counter-threshold state-transition idiom: once `self+0x44`
 * exceeds 0x13, latches `gUnknown_030014A3`, clears the hazard lock
 * (`gUnknown_030014A0`), and resets `self` to state 1/table-index 0 -
 * the same state/table-index/anim-frame reset idiom already documented
 * for the boss cluster's `sub_8030530`/`sub_8030C98` and this family's
 * own `sub_802C14C` (actor_part19.c) - then fires `sub_8029BAC(0x24)`. */
void sub_802BD24(void *selfArg)
{
    register u8 *self asm("r3") = selfArg;

    if (*(s32 *)(self + 0x44) > 0x13) {
        gUnknown_030014A3 = 1;
        gUnknown_030014A0 = 0;
        {
            register s32 state asm("r0") = 1;
            register s32 zero asm("r2") = 0;

            *(s32 *)(self + 0x28) = state;
            *(s32 *)(self + 0x44) = zero;
            *(s32 *)(self + 0xc) = zero;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
                register u8 zero2 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero2;
            }
            *(s32 *)(self + 8) = zero;
        }
        sub_8029BAC(0x24);
    }
}

extern s32 gUnknown_030014A4;
extern u8 gUnknown_030014A2;
extern s32 sub_8029B2C(void);
extern void sub_800132C(u8 flags, s32 frameDelay, u8 sync);
extern void sub_802A668(s32 arg0);

/* Per-axis hazard-threshold driver: drains a shared "camera catch-up"
 * budget (`gUnknown_030014A4`) into `self+0x20`, advances `self+0x24`
 * by a fixed step, and derives a camera-relative depth
 * (`self+0x34`, via `sub_8029B2C`) - the same shape as `sub_802F5E4`/
 * `sub_802F640` (actor_part44.c). Once that depth drops to/below the
 * far threshold, triggers a screen-flash (`sub_800132C`) once (latched
 * via `gUnknown_030014A2`) and also latches `gUnknown_03001480` (this
 * axis's own one-shot flag, see `sub_802BD18`); once it drops to/below
 * the near threshold, arms hazard direction 1 via `sub_802A668`. */
void sub_802BD64(void *selfArg)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x20) += gUnknown_030014A4;
    gUnknown_030014A4 += 0x2d;
    *(s32 *)(self + 0x24) += 0x3c;
    *(s32 *)(self + 0x34) = (sub_8029B2C() << 8) - *(s32 *)(self + 0x24);

    if (gUnknown_030014A2 == 0 && *(s32 *)(self + 0x34) <= 0x16FF) {
        sub_800132C(0, 2, 1);
        gUnknown_03001480 = 1;
        gUnknown_030014A2 = 1;
    }

    if (*(s32 *)(self + 0x34) <= 0x3FF) {
        sub_802A668(1);
    }
}

/* Same shape as `sub_802BD64` above (same axis budget/threshold pair),
 * but doesn't touch `gUnknown_03001480` and arms hazard direction 2
 * instead of 1. */
void sub_802BDD0(void *selfArg)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x20) += gUnknown_030014A4;
    gUnknown_030014A4 += 0x2d;
    *(s32 *)(self + 0x24) += 0x3c;
    *(s32 *)(self + 0x34) = (sub_8029B2C() << 8) - *(s32 *)(self + 0x24);

    if (gUnknown_030014A2 == 0 && *(s32 *)(self + 0x34) <= 0x16FF) {
        sub_800132C(0, 2, 1);
        gUnknown_030014A2 = 1;
    }

    if (*(s32 *)(self + 0x34) <= 0x3FF) {
        sub_802A668(2);
    }
}

/* Third axis of the same hazard-threshold family as `sub_802BD64`/
 * `sub_802BDD0`, but driven directly off `self+0x20` (no shared
 * accumulator/no `self+0x24`/`self+0x34` derivation) and arming hazard
 * direction 3. */
void sub_802BE34(void *selfArg)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x20) += -0x100;

    if (gUnknown_030014A2 == 0 && *(s32 *)(self + 0x20) < (s32)0xFFFFC000) {
        sub_800132C(0, 2, 1);
        gUnknown_030014A2 = 1;
    }

    if (*(s32 *)(self + 0x20) < (s32)0xFFFF8E00) {
        sub_802A668(3);
    }
}

extern u32 gUnknown_030007E0;

/* Frame-counter-threshold state-transition idiom, structural twin of
 * `sub_802BD24` above: once `self+0x44` reaches 0x1e, latches
 * `gUnknown_030014A3`, then either (if input bit 1 of
 * `gUnknown_030007E0` is clear) resets `self` to state 1/table-index 0
 * via the same reset idiom and fires `sub_8029BAC(0x24)`, or (bit set)
 * transitions to state 2 and fires `sub_8029BAC(0x38)` instead. */
void sub_802BE80(void *selfArg)
{
    register u8 *self asm("r2") = selfArg;

    if (*(s32 *)(self + 0x44) == 0x1e) {
        gUnknown_030014A3 = 1;

        {
            u16 bit = gUnknown_030007E0 & 2;

            if (bit == 0) {
                register s32 state asm("r0") = 1;

                *(s32 *)(self + 0x28) = state;
                *(s32 *)(self + 0x44) = bit;
                *(s32 *)(self + 0xc) = bit;
                {
                    register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero2;
                }
                *(s32 *)(self + 8) = bit;
                sub_8029BAC(0x24);
            } else {
                register s32 state asm("r0") = 2;

                *(s32 *)(self + 0x28) = state;
                *(s32 *)(self + 0x44) = 0;
                sub_8029BAC(0x38);
            }
        }
    }
}

asm(".align 2, 0");
