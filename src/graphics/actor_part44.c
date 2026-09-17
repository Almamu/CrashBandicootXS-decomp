#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md.
 * This file covers the whole contiguous run of accessors/accumulator-
 * drivers/state-transition helpers for the singleton and its `self`
 * object between the parked `sub_802F338` and `sub_802F748`. */

extern s32 sub_8023430(void *arg0);
extern void sub_802E484(s32 x, s32 y, s32 amount);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern void sub_8029BAC(s32 arg0);
extern s32 sub_8029748(s32 arg0);
extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);
extern s32 sub_800132C(s32 a, s32 b, s32 c);
extern s32 sub_802A668(s32 arg0);
extern s32 sub_8029B2C(void);
extern void sub_8028C48(void *arg0);
extern void mem_free(void *ptr);

extern void *gUnknown_030012C0;
extern void *gUnknown_030012BC;
extern u8 gUnknown_03001505;
extern u8 gUnknown_03001506;
extern u8 gUnknown_03001507;
extern s32 gUnknown_030014E0;
extern s32 gUnknown_030014E4;
extern u8 gUnknown_030014E8;
extern s32 gUnknown_030014F4;
extern s32 gUnknown_030014F8;
extern s32 gUnknown_030014FC;
extern s32 gUnknown_03001508;
extern void *gUnknown_03001518[2];
extern u8 gStaticData_0817C200[];
extern u8 gStaticData_087E5144[];
extern u8 gStaticData_087E4DF4[];

/* Accumulator-drain/reward-dispenser for the `gUnknown_030014FC`
 * accumulator `sub_802F540` fills: while the singleton flag
 * (`gUnknown_03001506`) is set, fully drains it via repeated
 * `sub_8023430` calls; otherwise, once a `gUnknown_030014F8` cooldown
 * elapses, dispenses one of four tiers of reward (via `sub_802E484` at
 * `self`'s position) sized by the accumulator's own magnitude, and
 * plays a cue. */
void sub_802F3BC(void *selfArg)
{
    register u8 *self asm("r1") = selfArg;
    s32 acc = gUnknown_030014FC;

    if (acc == 0) {
        return;
    }

    if (gUnknown_03001506 != 0) {
        do {
            sub_8023430(gUnknown_030012C0);
            gUnknown_030014FC--;
        } while (gUnknown_030014FC != 0);
        return;
    }

    if (gUnknown_030014F8 != 0) {
        gUnknown_030014F8--;
        return;
    }

    gUnknown_030014F8 = 0xf;

    if (acc <= 9) {
        sub_802E484(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), 1);
        gUnknown_030014FC -= 1;
    } else if (acc <= 0x13) {
        sub_802E484(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), 2);
        gUnknown_030014FC -= 2;
    } else if (acc <= 0x27) {
        sub_802E484(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), 4);
        gUnknown_030014FC -= 4;
    } else {
        sub_802E484(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), 8);
        gUnknown_030014FC -= 8;
    }

    PlaySfx(gUnknown_030012BC, 8, 0x100);
}

/* Trivial pre-increment counter accessor. */
s32 sub_802F46C(void)
{
    return ++gUnknown_030014E0;
}

/* Threshold check on `self+0x54`'s accumulator against
 * `gUnknown_030014E4`'s cap, used as a gate elsewhere in this cluster. */
s32 sub_802F47C(void *selfArg)
{
    u8 *self = selfArg;
    s32 v;
    s32 r;

    if (gUnknown_030014E4 == 0x64) {
        return *(s32 *)(self + 0x54);
    }

    v = *(s32 *)(self + 0x54);
    r = sub_803ADB4(v * 0x64, 0x78);
    if (r == 0 && v > 0) {
        r = 1;
    }
    return r;
}

/* Forwards `self+0x24` (z position) plus a fixed offset to
 * `sub_8029748`, discarding the result. */
void sub_802F4AC(void *selfArg)
{
    u8 *self = selfArg;

    sub_8029748(*(s32 *)(self + 0x24) + 0x7800);
}

/* Trivial byte getter for `gUnknown_030014E8`. */
u8 sub_802F4C0(void)
{
    return gUnknown_030014E8;
}

/* Countdown timer (`gUnknown_030014F4`) driving a palette-strip
 * animation refresh, ping-ponging the frame index via `sub_803ADB4`
 * the same way `sub_8031744` (actor_part26.c) does for its own strip. */
void sub_802F4CC(void)
{
    if (gUnknown_030014F4 != 0) {
        s32 frame;

        gUnknown_030014F4--;
        frame = sub_803ADB4(gUnknown_030014F4, 3);
        if (frame > 2) {
            frame = 5 - frame;
        }
        QueueVramDmaTransfer(gStaticData_0817C200 + (frame << 5), (void *)0x05000200, 0x20, 0x10);
    }
}

/* Advances `self+0x54`'s accumulator by a scaled `delta`, clamped to
 * `gUnknown_030014E4`'s cap, while the singleton flag is clear. */
void sub_802F50C(void *selfArg, s32 delta)
{
    u8 *self = selfArg;

    if (gUnknown_03001506 == 0) {
        s32 max = gUnknown_030014E4;
        s32 add = sub_803ADB4(delta * max, 0x64);
        s32 v = *(s32 *)(self + 0x54) + add;

        *(s32 *)(self + 0x54) = v;
        if (v > max) {
            *(s32 *)(self + 0x54) = max;
        }
    }
}

/* Feeds `delta` into the `gUnknown_030014FC` reward accumulator (the
 * one `sub_802F3BC` drains), arming its `gUnknown_030014F8` cooldown
 * the first time it goes from zero - gated on the current game-mode
 * flag at `gUnknown_030012C0+0x8c`. Its own first parameter (`self`)
 * is unused. */
void sub_802F540(void *selfArg, s32 delta)
{
    if (*((u8 *)gUnknown_030012C0 + 0x8c) == 0) {
        if (gUnknown_030014FC == 0) {
            gUnknown_030014F8 = 0xf;
        }
        gUnknown_030014FC += delta;
    }
}

/* If `self+0x12`'s flag is set, resets `self` to state 1/table-index 0
 * (an idle transition) and arms the singleton's `gUnknown_03001507`/
 * clears `gUnknown_03001506` flags, playing a cue. */
void sub_802F570(void *selfArg)
{
    register u8 *self asm("r2") = selfArg;

    if (self[0x12] != 0) {
        register s32 state asm("r5") = 1;
        register s32 zero asm("r1") = 0;

        *(s32 *)(self + 0x28) = state;
        *(s32 *)(self + 0x44) = zero;
        *(s32 *)(self + 0xc) = zero;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
            register u8 zero2 asm("r4") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero2;
            *(s32 *)(self + 8) = zero;
            sub_8029BAC(0x28);
            gUnknown_03001507 = state;
            gUnknown_03001506 = zero2;
        }
    }
}

/* Two independent one-shot transitions on `self`: if it's mid-table-
 * index-5 with the `self+0x12` flag set, resets its table index/anim
 * state; separately, once `self+0x44`'s counter hits `0x32`, marks
 * `self+0x28` state 1 and plays a cue. */
void sub_802F5AC(void *selfArg)
{
    register u8 *self asm("r3") = selfArg;

    if (*(s32 *)(self + 0xc) == 5 && self[0x12] != 0) {
        register s32 zero asm("r2") = 0;

        *(s32 *)(self + 0xc) = zero;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        *(s32 *)(self + 8) = zero;
    }

    if (*(s32 *)(self + 0x44) == 0x32) {
        *(s32 *)(self + 0x28) = 1;
        *(s32 *)(self + 0x44) = 0;
        sub_8029BAC(0x28);
    }
}

/* Advances `gUnknown_03001508`'s bounded oscillator by 9 (clamped to
 * +0x140 by absolute value), then fires two one-shot threshold
 * effects on `self+0x20` (screamed sfx cue + a `sub_802A668` hazard
 * call). */
void sub_802F5E4(void *selfArg)
{
    u8 *self = selfArg;
    s32 v = gUnknown_03001508 + 9;
    s32 sign;

    gUnknown_03001508 = v;
    sign = v >> 31;
    v ^= sign;
    v -= sign;
    if (v > 0x140) {
        gUnknown_03001508 = 0x140;
    }

    if (gUnknown_03001505 == 0 && *(s32 *)(self + 0x20) > 0x7080) {
        sub_800132C(0, 2, 1);
        gUnknown_03001505 = 1;
    }

    if (*(s32 *)(self + 0x20) > 0xE100) {
        sub_802A668(3);
    }
}

/* Advances `self+0x24` (z position) by a fixed step, derives
 * `self+0x34` (a camera-relative depth) via `sub_8029B2C`, and fires
 * the same one-shot threshold pair as `sub_802F5E4` off that derived
 * value instead, additionally latching `gUnknown_030014E8`. */
void sub_802F640(void *selfArg)
{
    u8 *self = selfArg;
    s32 v;

    *(s32 *)(self + 0x24) += 0x200;

    v = *(s32 *)(self + 0x24) - (sub_8029B2C() << 8);
    *(s32 *)(self + 0x34) = v;

    if (gUnknown_03001505 == 0 && v > 0x8200) {
        sub_800132C(0, 2, 1);
        gUnknown_03001505 = 1;
        gUnknown_030014E8 = 1;
    }

    if (*(s32 *)(self + 0x34) > 0xA000) {
        sub_802A668(1);
    }
}

/* `self+0x20`-threshold-gated twin of `sub_802F570`/`sub_802F69C`'s own
 * idle-reset idiom. */
void sub_802F69C(void *selfArg)
{
    register u8 *self asm("r2") = selfArg;

    if (*(s32 *)(self + 0x20) > 0x1E00) {
        register s32 state asm("r5") = 1;
        register s32 zero asm("r1") = 0;

        *(s32 *)(self + 0x28) = state;
        *(s32 *)(self + 0x44) = zero;
        *(s32 *)(self + 0xc) = zero;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
            register u8 zero2 asm("r4") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero2;
            *(s32 *)(self + 8) = zero;
            sub_8029BAC(0x28);
            gUnknown_03001507 = state;
            gUnknown_03001506 = zero2;
        }
    }
}

/* Teardown/destructor: marks `self` "dying" (`gStaticData_087E5144`
 * table), fully drains the `gUnknown_030014FC` reward accumulator,
 * frees the two keyframe-size tile allocations `sub_802F338` made
 * (`gUnknown_03001518`), marks `self` fully "dead"
 * (`gStaticData_087E4DF4`), unlinks it from its doubly-linked list,
 * and optionally frees it. */
void sub_802F6DC(void *selfArg, s32 flags)
{
    u8 *self = selfArg;
    s32 flagsReg = flags;

    *(void **)(self + 0x50) = gStaticData_087E5144;

    if (gUnknown_030014FC != 0) {
        do {
            sub_8023430(gUnknown_030012C0);
            gUnknown_030014FC--;
        } while (gUnknown_030014FC != 0);
    }

    sub_8028C48(gUnknown_03001518[0]);
    sub_8028C48(gUnknown_03001518[1]);

    *(void **)(self + 0x50) = gStaticData_087E4DF4;

    *(u8 **)(*(u8 **)(self + 0x4c) + 0x48) = *(u8 **)(self + 0x48);
    *(u8 **)(*(u8 **)(self + 0x48) + 0x4c) = *(u8 **)(self + 0x4c);

    if (flagsReg & 1) {
        mem_free(self);
    }
}
