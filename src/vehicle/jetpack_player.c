#include "core.h"
#include "actor_self.h"
#include "level_state.h"
#include <libgcc.h>
#include "system.h"
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"

/* Same "spawn/pre-attack" singleton family as wumpa.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md.
 * This file covers the whole contiguous run of accessors/accumulator-
 * drivers/state-transition helpers for the singleton and its `self`
 * object between the parked `AllocJetpackPlayerTiles` and `RunJetpackPlayerState`.
 *
 * The animation-reset blocks (`anim`/`zero1`/`zero2` register groups)
 * store through `*(T *)&self->field` casts: plain member stores let
 * gcc move the zero loads (docs/workflow.md step 7). */

/* `self`: the common actor prefix plus a meter that fills up to
 * `gJetpackPlayerMaxHp` (`HealJetpackPlayer`) and reads back as a percentage
 * of 120 (`GetJetpackPlayerHpPercent`). */
struct meter_actor {
    struct actor_self base;
    s32 meter;                  // 0x54
};

extern struct level_state *gLevelState;
extern u8 gActorVtable[];

/* Accumulator-drain/reward-dispenser for the `gJetpackQueuedWumpa`
 * accumulator `QueueJetpackWumpa` fills: while the singleton flag
 * (`gJetpackPlayerInactive`) is set, fully drains it via repeated
 * `CollectWumpa` calls; otherwise, once a `gJetpackWumpaDispenseTimer` cooldown
 * elapses, dispenses one of four tiers of reward (via `SpawnJetpackCollectedWumpa` at
 * `self`'s position) sized by the accumulator's own magnitude, and
 * plays a cue. */
void DispenseJetpackWumpa(void *selfArg)
{
    register struct meter_actor *self asm("r1") = selfArg;
    s32 acc = gJetpackQueuedWumpa;

    if (acc == 0) {
        return;
    }

    if (gJetpackPlayerInactive != 0) {
        do {
            CollectWumpa(gLevelState);
            gJetpackQueuedWumpa--;
        } while (gJetpackQueuedWumpa != 0);
        return;
    }

    if (gJetpackWumpaDispenseTimer != 0) {
        gJetpackWumpaDispenseTimer--;
        return;
    }

    gJetpackWumpaDispenseTimer = 0xf;

    if (acc <= 9) {
        SpawnJetpackCollectedWumpa(self->base.x, self->base.y, 1);
        gJetpackQueuedWumpa -= 1;
    } else if (acc <= 0x13) {
        SpawnJetpackCollectedWumpa(self->base.x, self->base.y, 2);
        gJetpackQueuedWumpa -= 2;
    } else if (acc <= 0x27) {
        SpawnJetpackCollectedWumpa(self->base.x, self->base.y, 4);
        gJetpackQueuedWumpa -= 4;
    } else {
        SpawnJetpackCollectedWumpa(self->base.x, self->base.y, 8);
        gJetpackQueuedWumpa -= 8;
    }

    PlaySfx(gAudioContext, 8, 0x100);
}

/* Trivial pre-increment counter accessor. `player` is unused; the caller
 * passes gActorList. */
s32 CountJetpackBomber(void *player)
{
    return ++gJetpackBomberCount;
}

/* Threshold check on the `meter` accumulator against
 * `gJetpackPlayerMaxHp`'s cap, used as a gate elsewhere in this cluster. */
s32 GetJetpackPlayerHpPercent(void *selfArg)
{
    struct meter_actor *self = selfArg;
    s32 v;
    s32 r;

    if (gJetpackPlayerMaxHp == 0x64) {
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
 * `SetActorCheckpoint`, discarding the result. */
void SetJetpackCheckpoint(void *selfArg)
{
    struct meter_actor *self = selfArg;

    SetActorCheckpoint(self->base.z + 0x7800);
}

/* Trivial byte getter for `gJetpackPauseLocked`. `player` is unused;
 * JetpackIsPauseLocked passes gActorList. */
s32 IsJetpackPauseLocked(void *player)
{
    return gJetpackPauseLocked;
}

/* Countdown timer (`gJetpackFlashTimer`) driving a palette-strip
 * animation refresh, ping-ponging the frame index via `__divsi3`
 * the same way `AnimateAirshipPalette` (airship_graphics.c) does for its own strip.
 * `self` is unused; UpdateJetpackPlayer passes the player. */
void AnimateJetpackPlayerPalette(void *self)
{
    if (gJetpackFlashTimer != 0) {
        s32 frame;

        gJetpackFlashTimer--;
        frame = __divsi3(gJetpackFlashTimer, 3);
        if (frame > 2) {
            frame = 5 - frame;
        }
        QueueVramDmaTransfer((void *)gJetpackFlashPalettes[frame], (void *)OBJ_PLTT, 0x20, 0x10);
    }
}

/* Advances the `meter` accumulator by a scaled `delta`, clamped to
 * `gJetpackPlayerMaxHp`'s cap, while the singleton flag is clear. */
void HealJetpackPlayer(void *selfArg, s32 delta)
{
    struct meter_actor *self = selfArg;

    if (gJetpackPlayerInactive == 0) {
        s32 max = gJetpackPlayerMaxHp;
        s32 add = __divsi3(delta * max, 0x64);
        s32 v = self->meter + add;

        self->meter = v;
        if (v > max) {
            self->meter = max;
        }
    }
}

/* Feeds `delta` into the `gJetpackQueuedWumpa` reward accumulator (the
 * one `DispenseJetpackWumpa` drains), arming its `gJetpackWumpaDispenseTimer` cooldown
 * the first time it goes from zero - gated on the level state's
 * `timeTrial` flag. Its own first parameter (`self`) is unused. */
void QueueJetpackWumpa(void *selfArg, s32 delta)
{
    if (gLevelState->timeTrial == 0) {
        if (gJetpackQueuedWumpa == 0) {
            gJetpackWumpaDispenseTimer = 0xf;
        }
        gJetpackQueuedWumpa += delta;
    }
}

/* If `animDone` is set, resets `self` to state 1/table-index 0
 * (an idle transition) and arms the singleton's `gJetpackInputEnabled`/
 * clears `gJetpackPlayerInactive` flags, playing a cue. */
void JetpackPlayerStateResume(void *selfArg)
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
            SetCellAnimSpeed(0x28);
            gJetpackInputEnabled = state;
            gJetpackPlayerInactive = zero2;
        }
    }
}

/* Two independent one-shot transitions on `self`: if it's mid-table-
 * index-5 with the `animDone` flag set, resets its table index/anim
 * state; separately, once `stateTime` hits `0x32`, sets `state` to 1
 * and plays a cue. */
void JetpackPlayerStateBoost(void *selfArg)
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
        SetCellAnimSpeed(0x28);
    }
}

/* Advances `gJetpackPlayerVelY`'s bounded oscillator by 9 (clamped to
 * +0x140 by absolute value), then fires two one-shot threshold
 * effects on `y` (screamed sfx cue + a `SetActorCategoryExitStatus` hazard
 * call). */
void JetpackPlayerStateFall(void *selfArg)
{
    struct meter_actor *self = selfArg;
    s32 v = gJetpackPlayerVelY + 9;
    s32 sign;

    gJetpackPlayerVelY = v;
    sign = v >> 31;
    v ^= sign;
    v -= sign;
    if (v > 0x140) {
        gJetpackPlayerVelY = 0x140;
    }

    if (gJetpackFadeStarted == 0 && self->base.y > 0x7080) {
        FadeBrightness(0, 2, 1);
        gJetpackFadeStarted = 1;
    }

    if (self->base.y > 0xE100) {
        SetActorCategoryExitStatus(3);
    }
}

/* Advances `z` by a fixed step, derives `depth` (a camera-relative
 * depth) via `GetCellAnimDistance`, and fires
 * the same one-shot threshold pair as `JetpackPlayerStateFall` off that derived
 * value instead, additionally latching `gJetpackPauseLocked`. */
void JetpackPlayerStateFinish(void *selfArg)
{
    struct meter_actor *self = selfArg;
    s32 v;

    self->base.z += 0x200;

    v = self->base.z - (GetCellAnimDistance() << 8);
    self->base.depth = v;

    if (gJetpackFadeStarted == 0 && v > 0x8200) {
        FadeBrightness(0, 2, 1);
        gJetpackFadeStarted = 1;
        gJetpackPauseLocked = 1;
    }

    if (self->base.depth > 0xA000) {
        SetActorCategoryExitStatus(1);
    }
}

/* `y`-threshold-gated twin of `JetpackPlayerStateResume`/`JetpackPlayerStateEnter`'s own
 * idle-reset idiom. */
void JetpackPlayerStateEnter(void *selfArg)
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
            SetCellAnimSpeed(0x28);
            gJetpackInputEnabled = state;
            gJetpackPlayerInactive = zero2;
        }
    }
}

/* Teardown/destructor: marks `self` "dying" (`gJetpackPlayerVtable`
 * table), fully drains the `gJetpackQueuedWumpa` reward accumulator,
 * frees the two keyframe-size tile allocations `AllocJetpackPlayerTiles` made
 * (`gJetpackPlayerTiles`), marks `self` fully "dead"
 * (`gActorVtable`), unlinks it from its doubly-linked list,
 * and optionally frees it. */
void DestroyJetpackPlayer(void *selfArg, s32 flags)
{
    u8 *self = selfArg;
    s32 flagsReg = flags;

    *(void **)(self + 0x50) = (void *)gJetpackPlayerVtable;

    if (gJetpackQueuedWumpa != 0) {
        do {
            CollectWumpa(gLevelState);
            gJetpackQueuedWumpa--;
        } while (gJetpackQueuedWumpa != 0);
    }

    FreeVramTileBlock(gJetpackPlayerTiles[0]);
    FreeVramTileBlock(gJetpackPlayerTiles[1]);

    *(void **)(self + 0x50) = gActorVtable;

    *(u8 **)(*(u8 **)(self + 0x4c) + 0x48) = *(u8 **)(self + 0x48);
    *(u8 **)(*(u8 **)(self + 0x48) + 0x4c) = *(u8 **)(self + 0x4c);

    if (flagsReg & 1) {
        mem_free(self);
    }
}

/* Same "spawn/pre-attack" singleton family as wumpa.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

/* Per-state member-pointer dispatch, `(this->*gJetpackPlayerStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`). */
void RunJetpackPlayerState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gJetpackPlayerStateFuncs);
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");

/* Same "spawn/pre-attack" singleton family as wumpa.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

/* Trivial byte getter for the singleton's own flag, `JetpackPlayerStateResume`/
 * `JetpackPlayerStateEnter`'s read counterpart. */
u8 IsJetpackPlayerInactive(void)
{
    return gJetpackPlayerInactive;
}

asm(".align 2, 0");
