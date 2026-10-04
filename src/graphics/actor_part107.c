#include "core.h"
#include "actor_self.h"

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
 * member - both confirmed here by this function's own body.
 *
 * `self` is `struct actor_self`; the animation-reset blocks store
 * through `*(T *)&self->field` casts, as in actor_part19.c. */

extern s32 gUnknown_03001488;
extern u8 gUnknown_030014A0;
extern s32 gUnknown_03001484;
extern void *gLevelState;
extern void *gUnknown_030012BC;

extern s32 CollectWumpa(void *self);
extern void sub_802B174(s32 a, s32 b, s32 c);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_8029BAC(s32 arg0);

/* Accumulator-drain/reward-dispenser for the `gUnknown_03001488`
 * accumulator (filled by `sub_802C078`, still raw): while the "locked"
 * flag `gUnknown_030014A0` is set, fully drains it via repeated
 * `CollectWumpa` calls without spawning anything; otherwise, once the
 * `gUnknown_03001484` cooldown elapses, dispenses one of four tiers of
 * reward (via `sub_802B174` at `self`'s position) sized by the
 * accumulator's own magnitude, and plays a cue. Exact structural twin
 * of `sub_802F3BC` (actor_part44.c) on a different accumulator/cooldown
 * pair - see docs/rom_map.md. */
void sub_802BC68(void *selfArg)
{
    register struct actor_self *self asm("r1") = selfArg;
    s32 acc = gUnknown_03001488;

    if (acc == 0) {
        return;
    }

    if (gUnknown_030014A0 != 0) {
        do {
            CollectWumpa(gLevelState);
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
        sub_802B174(self->x, self->y, 1);
        gUnknown_03001488 -= 1;
    } else if (acc <= 0x13) {
        sub_802B174(self->x, self->y, 2);
        gUnknown_03001488 -= 2;
    } else if (acc <= 0x27) {
        sub_802B174(self->x, self->y, 4);
        gUnknown_03001488 -= 4;
    } else {
        sub_802B174(self->x, self->y, 8);
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

/* Frame-counter-threshold state-transition idiom: once `stateTime`
 * exceeds 0x13, latches `gUnknown_030014A3`, clears the hazard lock
 * (`gUnknown_030014A0`), and resets `self` to state 1/table-index 0 -
 * the same state/table-index/anim-frame reset idiom already documented
 * for the boss cluster's `sub_8030530`/`sub_8030C98` and this family's
 * own `sub_802C14C` (actor_part19.c) - then fires `sub_8029BAC(0x24)`. */
void sub_802BD24(void *selfArg)
{
    register struct actor_self *self asm("r3") = selfArg;

    if (self->stateTime > 0x13) {
        gUnknown_030014A3 = 1;
        gUnknown_030014A0 = 0;
        {
            register s32 state asm("r0") = 1;
            register s32 zero asm("r2") = 0;

            self->state = state;
            self->stateTime = zero;
            self->animIndex = zero;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[0].duration;
                register u8 zero2 asm("r1") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero2;
            }
            self->animTime = zero;
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
 * budget (`gUnknown_030014A4`) into `y`, advances `z`
 * by a fixed step, and derives a camera-relative depth
 * (`depth`, via `sub_8029B2C`) - the same shape as `sub_802F5E4`/
 * `sub_802F640` (actor_part44.c). Once that depth drops to/below the
 * far threshold, triggers a screen-flash (`sub_800132C`) once (latched
 * via `gUnknown_030014A2`) and also latches `gUnknown_03001480` (this
 * axis's own one-shot flag, see `sub_802BD18`); once it drops to/below
 * the near threshold, arms hazard direction 1 via `sub_802A668`. */
void sub_802BD64(void *selfArg)
{
    struct actor_self *self = selfArg;

    self->y += gUnknown_030014A4;
    gUnknown_030014A4 += 0x2d;
    self->z += 0x3c;
    self->depth = (sub_8029B2C() << 8) - self->z;

    if (gUnknown_030014A2 == 0 && self->depth <= 0x16FF) {
        sub_800132C(0, 2, 1);
        gUnknown_03001480 = 1;
        gUnknown_030014A2 = 1;
    }

    if (self->depth <= 0x3FF) {
        sub_802A668(1);
    }
}

/* Same shape as `sub_802BD64` above (same axis budget/threshold pair),
 * but doesn't touch `gUnknown_03001480` and arms hazard direction 2
 * instead of 1. */
void sub_802BDD0(void *selfArg)
{
    struct actor_self *self = selfArg;

    self->y += gUnknown_030014A4;
    gUnknown_030014A4 += 0x2d;
    self->z += 0x3c;
    self->depth = (sub_8029B2C() << 8) - self->z;

    if (gUnknown_030014A2 == 0 && self->depth <= 0x16FF) {
        sub_800132C(0, 2, 1);
        gUnknown_030014A2 = 1;
    }

    if (self->depth <= 0x3FF) {
        sub_802A668(2);
    }
}

/* Third axis of the same hazard-threshold family as `sub_802BD64`/
 * `sub_802BDD0`, but driven directly off `y` (no shared
 * accumulator/no `z`/`depth` derivation) and arming hazard
 * direction 3. */
void sub_802BE34(void *selfArg)
{
    struct actor_self *self = selfArg;

    self->y += -0x100;

    if (gUnknown_030014A2 == 0 && self->y < (s32)0xFFFFC000) {
        sub_800132C(0, 2, 1);
        gUnknown_030014A2 = 1;
    }

    if (self->y < (s32)0xFFFF8E00) {
        sub_802A668(3);
    }
}

extern u32 gKeys;

/* Frame-counter-threshold state-transition idiom, structural twin of
 * `sub_802BD24` above: once `stateTime` reaches 0x1e, latches
 * `gUnknown_030014A3`, then either (if input bit 1 of
 * `gKeys` is clear) resets `self` to state 1/table-index 0
 * via the same reset idiom and fires `sub_8029BAC(0x24)`, or (bit set)
 * transitions to state 2 and fires `sub_8029BAC(0x38)` instead. */
void sub_802BE80(void *selfArg)
{
    register struct actor_self *self asm("r2") = selfArg;

    if (self->stateTime == 0x1e) {
        gUnknown_030014A3 = 1;

        {
            u16 bit = gKeys & 2;

            if (bit == 0) {
                register s32 state asm("r0") = 1;

                self->state = state;
                self->stateTime = bit;
                self->animIndex = bit;
                {
                    register u16 anim asm("r0") = *(u16 *)&self->anims[0].duration;
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero2;
                }
                self->animTime = bit;
                sub_8029BAC(0x24);
            } else {
                register s32 state asm("r0") = 2;

                self->state = state;
                self->stateTime = 0;
                sub_8029BAC(0x38);
            }
        }
    }
}

asm(".align 2, 0");
