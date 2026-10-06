#include "core.h"
#include "match.h"
#include "actor_self.h"
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"

/* Tail continuation of GitHub issue #50's chunk
 * (asm/code_3_2_20_8b7c_ac28.s, ROM 0x0802AC28-0x0802BED8): the giant
 * `CreateActor` kind-dispatch constructor and the run of "actor part
 * factory"/animation-table-state functions between it and here
 * (`CreatePolarCheckpointText`-`PolarPlayerStateCaught`) are still raw - this file only covers
 * the literal tail of that raw `.s` file, a self-contained run of
 * accumulator-drain/hazard-threshold helpers on the same `self` object
 * family documented in polar_player_actions.c/jetpack_player.c, operating on the
 * `gUnknown_0300148x`/`gUnknown_030014Ax` global cluster those files
 * already established (`gPolarQueuedWumpa`'s "reward" accumulator,
 * `gPolarPlayerInactive`-`030014A4`'s lock/hazard-latch quintet). See
 * docs/rom_map.md's "boss's BG2 spin/zoom effect..." section, which
 * already reads `DispensePolarWumpa` as one of a matched pair of accumulator-
 * drain/reward-dispenser functions (the other being `DispenseJetpackWumpa` in
 * jetpack_player.c) and `SpawnPolarCollectedWumpa` as a "spawn effect type N" family
 * member - both confirmed here by this function's own body.
 *
 * `self` is `struct actor_self`; the animation-reset blocks store
 * through `*(T *)&self->field` casts, as in polar_player_actions.c. */

/* Accumulator-drain/reward-dispenser for the `gPolarQueuedWumpa`
 * accumulator (filled by `QueuePolarWumpa`, still raw): while the "locked"
 * flag `gPolarPlayerInactive` is set, fully drains it via repeated
 * `CollectWumpa` calls without spawning anything; otherwise, once the
 * `gPolarWumpaDispenseTimer` cooldown elapses, dispenses one of four tiers of
 * reward (via `SpawnPolarCollectedWumpa` at `self`'s position) sized by the
 * accumulator's own magnitude, and plays a cue. Exact structural twin
 * of `DispenseJetpackWumpa` (jetpack_player.c) on a different accumulator/cooldown
 * pair - see docs/rom_map.md. */
void DispensePolarWumpa(void *selfArg)
{
    MATCH_HOLD_REG(struct actor_self *, self, r1) = selfArg;
    s32 acc = gPolarQueuedWumpa;

    if (acc == 0) {
        return;
    }

    if (gPolarPlayerInactive != 0) {
        do {
            CollectWumpa(gLevelState);
            gPolarQueuedWumpa--;
        } while (gPolarQueuedWumpa != 0);
        return;
    }

    if (gPolarWumpaDispenseTimer != 0) {
        gPolarWumpaDispenseTimer--;
        return;
    }

    gPolarWumpaDispenseTimer = 0xf;

    if (acc <= 9) {
        SpawnPolarCollectedWumpa(self->x, self->y, 1);
        gPolarQueuedWumpa -= 1;
    } else if (acc <= 0x13) {
        SpawnPolarCollectedWumpa(self->x, self->y, 2);
        gPolarQueuedWumpa -= 2;
    } else if (acc <= 0x27) {
        SpawnPolarCollectedWumpa(self->x, self->y, 4);
        gPolarQueuedWumpa -= 4;
    } else {
        SpawnPolarCollectedWumpa(self->x, self->y, 8);
        gPolarQueuedWumpa -= 8;
    }

    PlaySfx(gAudioContext, 8, 0x100);
}

/* Trivial byte getter. `player` is unused; PolarIsPauseLocked
 * (actor_spawn.c) passes gActorList. */
s32 IsPolarPauseLocked(void *player)
{
    return gPolarPauseLocked;
}

/* Frame-counter-threshold state-transition idiom: once `stateTime`
 * exceeds 0x13, latches `gPolarSteerEnabled`, clears the hazard lock
 * (`gPolarPlayerInactive`), and resets `self` to state 1/table-index 0 -
 * the same state/table-index/anim-frame reset idiom already documented
 * for the boss cluster's `DamageAirshipFireball`/`AirshipStateFall` and this family's
 * own `LaunchPolarPlayer` (polar_player_actions.c) - then fires `SetCellAnimSpeed(0x24)`.
 * gPolarPlayerStateFuncs[13]: undoes what HurtPolarPlayer sets (steering
 * off, `gPolarPlayerInactive`) and goes back to PolarPlayerStateRun
 * after 20 frames. No code found that enters state 13. */
void PolarPlayerStateRecover(void *selfArg)
{
    MATCH_HOLD_REG(struct actor_self *, self, r3) = selfArg;

    if (self->stateTime > 0x13) {
        gPolarSteerEnabled = 1;
        gPolarPlayerInactive = 0;
        {
            MATCH_HOLD_REG(s32, state, r0) = 1;
            MATCH_HOLD_REG(s32, zero, r2) = 0;

            self->state = state;
            self->stateTime = zero;
            self->animIndex = zero;
            {
                MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->anims[0].duration;
                MATCH_HOLD_REG(u8, zero2, r1) = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero2;
            }
            self->animTime = zero;
        }
        SetCellAnimSpeed(0x24);
    }
}

/* Per-axis hazard-threshold driver: drains a shared "camera catch-up"
 * budget (`gPolarPlayerVelY`) into `y`, advances `z`
 * by a fixed step, and derives a camera-relative depth
 * (`depth`, via `GetCellAnimDistance`) - the same shape as `JetpackPlayerStateFall`/
 * `JetpackPlayerStateFinish` (jetpack_player.c). Once that depth drops to/below the
 * far threshold, triggers a screen-flash (`FadeBrightness`) once (latched
 * via `gPolarFadeStarted`) and also latches `gPolarPauseLocked` (this
 * axis's own one-shot flag, see `IsPolarPauseLocked`); once it drops to/below
 * the near threshold, arms hazard direction 1 via `SetActorCategoryExitStatus`. */
void PolarPlayerStateFinishLeap(void *selfArg)
{
    struct actor_self *self = selfArg;

    self->y += gPolarPlayerVelY;
    gPolarPlayerVelY += 0x2d;
    self->z += 0x3c;
    self->depth = (GetCellAnimDistance() << 8) - self->z;

    if (gPolarFadeStarted == 0 && self->depth <= 0x16FF) {
        FadeBrightness(0, 2, 1);
        gPolarPauseLocked = 1;
        gPolarFadeStarted = 1;
    }

    if (self->depth <= 0x3FF) {
        SetActorCategoryExitStatus(1);
    }
}

/* Same shape as `PolarPlayerStateFinishLeap` above (same axis budget/threshold pair),
 * but doesn't touch `gPolarPauseLocked` and arms hazard direction 2
 * instead of 1. */
void PolarPlayerStateCarriedOff(void *selfArg)
{
    struct actor_self *self = selfArg;

    self->y += gPolarPlayerVelY;
    gPolarPlayerVelY += 0x2d;
    self->z += 0x3c;
    self->depth = (GetCellAnimDistance() << 8) - self->z;

    if (gPolarFadeStarted == 0 && self->depth <= 0x16FF) {
        FadeBrightness(0, 2, 1);
        gPolarFadeStarted = 1;
    }

    if (self->depth <= 0x3FF) {
        SetActorCategoryExitStatus(2);
    }
}

/* Third axis of the same hazard-threshold family as `PolarPlayerStateFinishLeap`/
 * `PolarPlayerStateCarriedOff`, but driven directly off `y` (no shared
 * accumulator/no `z`/`depth` derivation) and arming hazard
 * direction 3. */
void PolarPlayerStateKnockedOff(void *selfArg)
{
    struct actor_self *self = selfArg;

    self->y += -0x100;

    if (gPolarFadeStarted == 0 && self->y < (s32)0xFFFFC000) {
        FadeBrightness(0, 2, 1);
        gPolarFadeStarted = 1;
    }

    if (self->y < (s32)0xFFFF8E00) {
        SetActorCategoryExitStatus(3);
    }
}

/* Frame-counter-threshold state-transition idiom, structural twin of
 * `PolarPlayerStateRecover` above: once `stateTime` reaches 0x1e, latches
 * `gPolarSteerEnabled`, then either (if input bit 1 of
 * `gKeys` is clear) resets `self` to state 1/table-index 0
 * via the same reset idiom and fires `SetCellAnimSpeed(0x24)`, or (bit set)
 * transitions to state 2 and fires `SetCellAnimSpeed(0x38)` instead. */
void PolarPlayerStateBoost(void *selfArg)
{
    MATCH_HOLD_REG(struct actor_self *, self, r2) = selfArg;

    if (self->stateTime == 0x1e) {
        gPolarSteerEnabled = 1;

        {
            u16 bit = gKeys.all & 2;

            if (bit == 0) {
                MATCH_HOLD_REG(s32, state, r0) = 1;

                self->state = state;
                self->stateTime = bit;
                self->animIndex = bit;
                {
                    MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->anims[0].duration;
                    MATCH_HOLD_REG(u8, zero2, r1) = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero2;
                }
                self->animTime = bit;
                SetCellAnimSpeed(0x24);
            } else {
                MATCH_HOLD_REG(s32, state, r0) = 2;

                self->state = state;
                self->stateTime = 0;
                SetCellAnimSpeed(0x38);
            }
        }
    }
}

asm(".align 2, 0");
