#include "core.h"
#include "actor_self.h"
#include "level_state.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md.
 * This file covers the whole contiguous run of accessors/accumulator-
 * drivers/state-transition helpers for the singleton and its `self`
 * object between the parked `sub_802F338` and `sub_802F748`.
 *
 * The animation-reset blocks (`anim`/`zero1`/`zero2` register groups)
 * store through `*(T *)&self->field` casts: plain member stores let
 * gcc move the zero loads (docs/workflow.md step 7). */

/* `self`: the common actor prefix plus a meter that fills up to
 * `gUnknown_030014E4` (`sub_802F50C`) and reads back as a percentage
 * of 120 (`sub_802F47C`). */
struct meter_actor {
    struct actor_self base;
    s32 meter;                  // 0x54
};

extern s32 CollectWumpa(void *arg0);
extern void sub_802E484(s32 x, s32 y, s32 amount);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 __divsi3(s32 arg0, s32 arg1);
extern void sub_8029BAC(s32 arg0);
extern s32 sub_8029748(s32 arg0);
extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);
extern s32 sub_800132C(s32 a, s32 b, s32 c);
extern s32 sub_802A668(s32 arg0);
extern s32 sub_8029B2C(void);
extern void FreeVramTileBlock(void *arg0);
extern void mem_free(void *ptr);

extern struct level_state *gLevelState;
extern void *gAudioContext;
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
extern u8 gActorVtable[];

/* Accumulator-drain/reward-dispenser for the `gUnknown_030014FC`
 * accumulator `sub_802F540` fills: while the singleton flag
 * (`gUnknown_03001506`) is set, fully drains it via repeated
 * `CollectWumpa` calls; otherwise, once a `gUnknown_030014F8` cooldown
 * elapses, dispenses one of four tiers of reward (via `sub_802E484` at
 * `self`'s position) sized by the accumulator's own magnitude, and
 * plays a cue. */
void sub_802F3BC(void *selfArg)
{
    register struct meter_actor *self asm("r1") = selfArg;
    s32 acc = gUnknown_030014FC;

    if (acc == 0) {
        return;
    }

    if (gUnknown_03001506 != 0) {
        do {
            CollectWumpa(gLevelState);
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
        sub_802E484(self->base.x, self->base.y, 1);
        gUnknown_030014FC -= 1;
    } else if (acc <= 0x13) {
        sub_802E484(self->base.x, self->base.y, 2);
        gUnknown_030014FC -= 2;
    } else if (acc <= 0x27) {
        sub_802E484(self->base.x, self->base.y, 4);
        gUnknown_030014FC -= 4;
    } else {
        sub_802E484(self->base.x, self->base.y, 8);
        gUnknown_030014FC -= 8;
    }

    PlaySfx(gAudioContext, 8, 0x100);
}

/* Trivial pre-increment counter accessor. */
s32 sub_802F46C(void)
{
    return ++gUnknown_030014E0;
}

/* Threshold check on the `meter` accumulator against
 * `gUnknown_030014E4`'s cap, used as a gate elsewhere in this cluster. */
s32 sub_802F47C(void *selfArg)
{
    struct meter_actor *self = selfArg;
    s32 v;
    s32 r;

    if (gUnknown_030014E4 == 0x64) {
        return self->meter;
    }

    v = self->meter;
    r = __divsi3(v * 0x64, 0x78);
    if (r == 0 && v > 0) {
        r = 1;
    }
    return r;
}

/* Forwards `z` plus a fixed offset to
 * `sub_8029748`, discarding the result. */
void sub_802F4AC(void *selfArg)
{
    struct meter_actor *self = selfArg;

    sub_8029748(self->base.z + 0x7800);
}

/* Trivial byte getter for `gUnknown_030014E8`. */
u8 sub_802F4C0(void)
{
    return gUnknown_030014E8;
}

/* Countdown timer (`gUnknown_030014F4`) driving a palette-strip
 * animation refresh, ping-ponging the frame index via `__divsi3`
 * the same way `sub_8031744` (actor_part26.c) does for its own strip. */
void sub_802F4CC(void)
{
    if (gUnknown_030014F4 != 0) {
        s32 frame;

        gUnknown_030014F4--;
        frame = __divsi3(gUnknown_030014F4, 3);
        if (frame > 2) {
            frame = 5 - frame;
        }
        QueueVramDmaTransfer(gStaticData_0817C200 + (frame << 5), (void *)OBJ_PLTT, 0x20, 0x10);
    }
}

/* Advances the `meter` accumulator by a scaled `delta`, clamped to
 * `gUnknown_030014E4`'s cap, while the singleton flag is clear. */
void sub_802F50C(void *selfArg, s32 delta)
{
    struct meter_actor *self = selfArg;

    if (gUnknown_03001506 == 0) {
        s32 max = gUnknown_030014E4;
        s32 add = __divsi3(delta * max, 0x64);
        s32 v = self->meter + add;

        self->meter = v;
        if (v > max) {
            self->meter = max;
        }
    }
}

/* Feeds `delta` into the `gUnknown_030014FC` reward accumulator (the
 * one `sub_802F3BC` drains), arming its `gUnknown_030014F8` cooldown
 * the first time it goes from zero - gated on the level state's
 * `timeTrial` flag. Its own first parameter (`self`) is unused. */
void sub_802F540(void *selfArg, s32 delta)
{
    if (gLevelState->timeTrial == 0) {
        if (gUnknown_030014FC == 0) {
            gUnknown_030014F8 = 0xf;
        }
        gUnknown_030014FC += delta;
    }
}

/* If `animDone` is set, resets `self` to state 1/table-index 0
 * (an idle transition) and arms the singleton's `gUnknown_03001507`/
 * clears `gUnknown_03001506` flags, playing a cue. */
void sub_802F570(void *selfArg)
{
    register struct meter_actor *self asm("r2") = selfArg;

    if (self->base.animDone != 0) {
        register s32 state asm("r5") = 1;
        register s32 zero asm("r1") = 0;

        self->base.state = state;
        self->base.stateTime = zero;
        self->base.animIndex = zero;
        {
            register u16 anim asm("r0") = *(u16 *)&self->base.anims[0].duration;
            register u8 zero2 asm("r4") = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero2;
            self->base.animTime = zero;
            sub_8029BAC(0x28);
            gUnknown_03001507 = state;
            gUnknown_03001506 = zero2;
        }
    }
}

/* Two independent one-shot transitions on `self`: if it's mid-table-
 * index-5 with the `animDone` flag set, resets its table index/anim
 * state; separately, once `stateTime` hits `0x32`, sets `state` to 1
 * and plays a cue. */
void sub_802F5AC(void *selfArg)
{
    register struct meter_actor *self asm("r3") = selfArg;

    if (self->base.animIndex == 5 && self->base.animDone != 0) {
        register s32 zero asm("r2") = 0;

        self->base.animIndex = zero;
        {
            register u16 anim asm("r0") = *(u16 *)&self->base.anims[0].duration;
            register u8 zero1 asm("r1") = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero1;
        }
        self->base.animTime = zero;
    }

    if (self->base.stateTime == 0x32) {
        self->base.state = 1;
        self->base.stateTime = 0;
        sub_8029BAC(0x28);
    }
}

/* Advances `gUnknown_03001508`'s bounded oscillator by 9 (clamped to
 * +0x140 by absolute value), then fires two one-shot threshold
 * effects on `y` (screamed sfx cue + a `sub_802A668` hazard
 * call). */
void sub_802F5E4(void *selfArg)
{
    struct meter_actor *self = selfArg;
    s32 v = gUnknown_03001508 + 9;
    s32 sign;

    gUnknown_03001508 = v;
    sign = v >> 31;
    v ^= sign;
    v -= sign;
    if (v > 0x140) {
        gUnknown_03001508 = 0x140;
    }

    if (gUnknown_03001505 == 0 && self->base.y > 0x7080) {
        sub_800132C(0, 2, 1);
        gUnknown_03001505 = 1;
    }

    if (self->base.y > 0xE100) {
        sub_802A668(3);
    }
}

/* Advances `z` by a fixed step, derives `depth` (a camera-relative
 * depth) via `sub_8029B2C`, and fires
 * the same one-shot threshold pair as `sub_802F5E4` off that derived
 * value instead, additionally latching `gUnknown_030014E8`. */
void sub_802F640(void *selfArg)
{
    struct meter_actor *self = selfArg;
    s32 v;

    self->base.z += 0x200;

    v = self->base.z - (sub_8029B2C() << 8);
    self->base.depth = v;

    if (gUnknown_03001505 == 0 && v > 0x8200) {
        sub_800132C(0, 2, 1);
        gUnknown_03001505 = 1;
        gUnknown_030014E8 = 1;
    }

    if (self->base.depth > 0xA000) {
        sub_802A668(1);
    }
}

/* `y`-threshold-gated twin of `sub_802F570`/`sub_802F69C`'s own
 * idle-reset idiom. */
void sub_802F69C(void *selfArg)
{
    register struct meter_actor *self asm("r2") = selfArg;

    if (self->base.y > 0x1E00) {
        register s32 state asm("r5") = 1;
        register s32 zero asm("r1") = 0;

        self->base.state = state;
        self->base.stateTime = zero;
        self->base.animIndex = zero;
        {
            register u16 anim asm("r0") = *(u16 *)&self->base.anims[0].duration;
            register u8 zero2 asm("r4") = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero2;
            self->base.animTime = zero;
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
 * (`gActorVtable`), unlinks it from its doubly-linked list,
 * and optionally frees it. */
void sub_802F6DC(void *selfArg, s32 flags)
{
    u8 *self = selfArg;
    s32 flagsReg = flags;

    *(void **)(self + 0x50) = gStaticData_087E5144;

    if (gUnknown_030014FC != 0) {
        do {
            CollectWumpa(gLevelState);
            gUnknown_030014FC--;
        } while (gUnknown_030014FC != 0);
    }

    FreeVramTileBlock(gUnknown_03001518[0]);
    FreeVramTileBlock(gUnknown_03001518[1]);

    *(void **)(self + 0x50) = gActorVtable;

    *(u8 **)(*(u8 **)(self + 0x4c) + 0x48) = *(u8 **)(self + 0x48);
    *(u8 **)(*(u8 **)(self + 0x48) + 0x4c) = *(u8 **)(self + 0x4c);

    if (flagsReg & 1) {
        mem_free(self);
    }
}
