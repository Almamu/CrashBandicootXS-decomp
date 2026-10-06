#include "core.h"
#include "match.h"
#include "memory.h"
#include "actor_self.h"
#include <libgcc.h>
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"

/* Continues the same player/action-object action-table family already
 * documented in ctrl.c/action_ctrl_states.c/action_ctrl_land.c - `self`
 * is the same large per-instance object those files use (state at
 * `+0x28`, a table-index field at `+0xc`, an anim-frame halfword/byte
 * pair at `+0x10`/`+0x12`, a counter at `+0x44`, an accumulator at `+8`
 * that doubles as `struct anim_part_instance.field_08` for
 * `GetAnimFrameBaseOffset`, and a "part table" pointer at `+0`, the
 * same convention action_ctrl_states.c documents at `animTimer` for its own
 * object), plus a `+0x50`-rooted `{s16 offset; void *fn}` trampoline
 * record fed through `_call_via_r2`/`_call_via_r3` (the same convention
 * already named in part_list_cull.c/part_list.c for a sibling "part"
 * object, just at a different fixed offset here) and a `+0x48`/`+0x4c`
 * circular doubly-linked-list pair (confirmed by `DestroyPolarPlayer`'s own
 * unlink sequence below) rooted at the player-pointer global
 * `gActorList`. `self` is `struct actor_self` (actor_self.h);
 * the functions not yet converted still use raw offsets into it - see
 * docs/rom_map.md's "gUnknown_030014xx tier-threshold actor
 * family" and "type-byte event dispatch" sections for the semantics
 * behind the individual functions below. */


extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *addr, void *arg1, void *tableEntry, void *fn);

/* Accumulates `gPolarPlayerVelY` into `y`, then drains
 * `gPolarPlayerVelY` toward a fixed ceiling (`0x780`) - the same
 * "lazy-singleton accumulator" shape documented in docs/rom_map.md for
 * `PolarPlayerStateMount`. Once `y` crosses a threshold (`0x2800`),
 * clamps it and fires the state-1/table-index-4 transition (anim frame
 * taken from `self`'s own part-table pointer at `+0x30`). */
void PolarPlayerStateLaunched(void *selfArg)
{
    struct actor_self *self = selfArg;
    s32 total = self->y + gPolarPlayerVelY;

    self->y = total;
    gPolarPlayerVelY += 0x60;
    if (gPolarPlayerVelY > 0x780) {
        gPolarPlayerVelY = 0x780;
    }

    if (total > 0x2800) {
        self->y = 0x2800;
        {
            MATCH_HOLD_REG(u8 *, addr, r1) = &gPolarSteerEnabled;
            MATCH_HOLD_REG(u8, val, r0) = 1;
            *addr = val;
        }
        SetCellAnimSpeed(0x24);
        {
            MATCH_HOLD_REG(s32, stateVal, r0) = 1;
            MATCH_HOLD_REG(s32, idxVal, r1) = 4;
            self->state = stateVal;
            {
                MATCH_HOLD_REG(s32, zero, r2) = 0;
                self->stateTime = zero;
                self->animIndex = idxVal;
                {
                    /* Stored through `*(T *)&field` casts: plain member
                     * stores let gcc move the zero load (docs/workflow.md
                     * step 7). */
                    MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->anims[4].duration;
                    MATCH_HOLD_REG(u8, zero2, r1) = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero2;
                }
                self->animTime = zero;
            }
        }
    }
}

/* On the "confirm" input edge (`animDone` set), plays a sound, resets
 * `gPolarPlayerVelY` to a large negative "cooldown" value, and fires
 * the state-11/table-index-7 transition (anim frame from `self`'s
 * part-table pointer at `+0x54`). While `y` (the accumulator
 * `PolarPlayerStateLaunched` above drives) exceeds a threshold, additionally spawns
 * an effect object via `CreateActor` and stashes it into
 * `gRiderlessPolar`. */
void PolarPlayerStateFinish(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animDone != 0) {
        PlaySfx(gAudioContext, 0x3c, 0x100);
        gPolarPlayerVelY = 0xFFFFF980;
        {
            MATCH_HOLD_REG(s32, stateVal, r0) = 0xb;
            MATCH_HOLD_REG(s32, idxVal, r1) = 7;

            self->state = stateVal;
            {
                MATCH_HOLD_REG(s32, zero, r5) = 0;

                self->stateTime = zero;
                self->animIndex = idxVal;
                {
                    /* Casts as in PolarPlayerStateLaunched. */
                    MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->anims[7].duration;
                    MATCH_HOLD_REG(u8, zero2, r1) = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero2;
                }
                self->animTime = zero;

                if (self->y > 0x2000) {
                    gRiderlessPolar = CreateActor(2, self->x, 0x2800,
                                                     self->z, (void *)zero);
                }
            }
        }
    }
}

/* On the "confirm" input edge, sets `gPolarSteerEnabled`/state-1/
 * table-index-0 (anim frame from `self`'s own part-table pointer at
 * `+0`) and fires `SetCellAnimSpeed(0x24)` - the state-transition counterpart
 * to `PolarPlayerStateLaunched`, entered directly rather than through the
 * accumulator threshold. */
void PolarPlayerStateLand(void *selfArg)
{
    MATCH_HOLD_REG(struct actor_self *, self, r3) = selfArg;

    if (self->animDone != 0) {
        {
            MATCH_HOLD_REG(u8 *, addr, r1) = &gPolarSteerEnabled;
            MATCH_HOLD_REG(u8, val, r0) = 1;
            *addr = val;
        }
        {
            MATCH_HOLD_REG(s32, stateVal, r0) = 1;
            MATCH_HOLD_REG(s32, zero, r2) = 0;

            self->state = stateVal;
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

/* Once-only latch (`gPolarPlayerInactive`): arms a countdown
 * (`gPolarFinishTimer = 0x16`), runs `SetCellAnimSpeed(0x24)`, clamps
 * `gPolarPlayerVelY` to non-negative, then calls `StopYeti` and
 * marks both `gPolarPlayerInactive` and `gPolarSteerEnabled`. `player`
 * is unused; both callers pass gActorList. */
void FinishPolarRun(void *player)
{
    if (gPolarPlayerInactive == 0) {
        gPolarFinishTimer = 0x16;
        SetCellAnimSpeed(0x24);
        if (gPolarPlayerVelY < 0) {
            gPolarPlayerVelY = 0;
        }
        StopYeti();
        gPolarPlayerInactive = 1;
        gPolarSteerEnabled = 0;
    }
}

/* Resets the `gPolarSteerEnabled`/`030014A1`/`030014A0` latch trio, runs
 * `ClearPolarAkuAkuMask` on `gPolarAkuAku`, and fires the state-7/table-
 * index-6 transition (anim frame from `self`'s part-table pointer at
 * `+0x48`) plus a sound cue. */
void CatchPolarPlayer(void *selfArg)
{
    struct actor_self *self = selfArg;

    gPolarSteerEnabled = 0;
    gPolarPlayerHalted = 1;
    ClearPolarAkuAkuMask(gPolarAkuAku);
    gPolarPlayerInactive = 1;

    {
        MATCH_HOLD_REG(s32, stateVal, r0) = 7;
        MATCH_HOLD_REG(s32, idxVal, r1) = 6;

        self->state = stateVal;
        {
            MATCH_HOLD_REG(s32, zero, r2) = 0;

            self->stateTime = zero;
            self->animIndex = idxVal;
            {
                MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->anims[6].duration;
                MATCH_HOLD_REG(u8, zero2, r1) = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero2;
            }
            self->animTime = zero;
        }
    }

    PlaySfx(gAudioContext, 0x41, 0x100);
}

/* Accumulator: while `gLevelState+0x8c` is clear, arms
 * `gPolarWumpaDispenseTimer` (once, on the first accumulation) and adds
 * `delta` into `gPolarQueuedWumpa`. Ignores its own first (player-
 * pointer) argument entirely - see docs/rom_map.md's correction on
 * this function. */
void QueuePolarWumpa(void *arg0, s32 delta)
{
    if (gLevelState->timeTrial == 0) {
        if (gPolarQueuedWumpa == 0) {
            gPolarWumpaDispenseTimer = 0xf;
        }
        gPolarQueuedWumpa += delta;
    }
}

/* Trivial forwarder - ignores its own argument and calls
 * `AddLife(gLevelState)`, per docs/rom_map.md's correction
 * (the ROM's own tail-call epilogue clobbers r0/the call's result, so
 * this is void, not passed through as a return value). */
void GivePolarPlayerLife(void *arg0)
{
    AddLife(gLevelState);
}

/* Only runs while `state` is 1-3: sets table-index 2, anim
 * frame from `self`'s part-table pointer at `+0x18`, and - once the
 * frame counter reaches the entry's threshold (the same `+4`-halfword-
 * of-a-0xc-stride-table shape as `UpdatePolarCollectedWumpa` below) - resets the
 * `+8` accumulator. Stashes `arg1` into `x`, plays a
 * state-keyed sound cue (0x5a for state 2, 0x55 for state 1), and
 * transitions to state 3. */
void BoostPolarPlayer(void *selfArg, s32 arg1param)
{
    MATCH_HOLD_REG(struct actor_self *, self, r4) = selfArg;
    MATCH_HOLD_REG(s32, arg1, r5) = arg1param;

    if ((u32)(self->state - 1) <= 2) {
        s32 frame;

        self->animIndex = 2;
        {
            /* Casts as in PolarPlayerStateLaunched. */
            MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->anims[2].duration;
            MATCH_HOLD_REG(u8, zero1, r1) = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
        }

        frame = GetAnimFrameBaseOffset(self);
        {
            MATCH_HOLD_REG(s32, idx, r2) = self->animIndex;
            MATCH_HOLD_REG(u8 *, table, r3) = (u8 *)self->anims;
            MATCH_HOLD_REG(u8 *, entryPtr, r1) = (u8 *)(idx * 0xc);
            MATCH_HOLD_REG(s32, four, r2);
            MATCH_HOLD_REG(s32, val, r1);

            asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
            four = 4;
            val = *(s16 *)(entryPtr + four);

            if (frame >= val) {
                self->animTime = 0;
            }
        }

        self->x = arg1;

        {
            s32 switchState = self->state;

            if (switchState == 2) {
                SetCellAnimSpeed(0x5a);
            } else if (switchState == 1) {
                SetCellAnimSpeed(0x55);
            }
        }

        self->state = 3;
        self->stateTime = 0;
        gPolarSteerEnabled = 0;
    }
}

/* Lock-timer setter: while `AddPolarAkuAkuMask(gPolarAkuAku)` returns 3,
 * arms `gPolarInvulnTimer = 500` - the same lock/active flag
 * `ShockPolarPlayer` gates on, per docs/rom_map.md. */
void GivePolarPlayerMask(void *arg0)
{
    if (AddPolarAkuAkuMask(gPolarAkuAku) == 3) {
        gPolarInvulnTimer = 500;
    }
}

/* While `state` is 1-3 and `gPolarInvulnTimer` (the same
 * lock/active flag `ShockPolarPlayer` gates on, per docs/rom_map.md) is
 * clear: transitions to state 5/table-index 3 (anim frame from
 * `self`'s part-table pointer at `+0x24`), resets
 * `gPolarPlayerVelY` to `-0x780`, and fires `SetCellAnimSpeed(0x1c)`. */
void LaunchPolarPlayer(void *selfArg)
{
    MATCH_HOLD_REG(struct actor_self *, self, r2) = selfArg;

    if ((u32)(self->state - 1) <= 2) {
        MATCH_HOLD_REG(s32, flag, r3) = gPolarInvulnTimer;

        if (flag == 0) {
            gPolarSteerEnabled = flag;
            {
                MATCH_HOLD_REG(s32, stateVal, r0) = 5;
                MATCH_HOLD_REG(s32, idxVal, r1) = 3;

                self->state = stateVal;
                self->stateTime = flag;
                self->animIndex = idxVal;
                {
                    /* Casts as in PolarPlayerStateLaunched. */
                    MATCH_HOLD_REG(u16, anim, r0) = *(u16 *)&self->anims[3].duration;
                    MATCH_HOLD_REG(u8, zero2, r1) = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero2;
                }
                self->animTime = flag;
            }
            gPolarPlayerVelY = 0xFFFFF880;
            SetCellAnimSpeed(0x1c);
        }
    }
}

/* Teardown, gated by `arg1` bit 0: temporarily swaps `vtable`'s
 * vtable to `gPolarPlayerVtable` to run `gPolarQueuedWumpa` drain
 * calls into `CollectWumpa(gLevelState)`, runs two
 * `FreeVramTileBlock` cleanup calls on `gPolarPlayerTiles[0]`/`[1]`, sets
 * `vtable` to the "dead" vtable `gActorVtable`, unlinks
 * `self` from the circular `+0x48`(prev)/`+0x4c`(next) list, and frees
 * `self` when `arg1 & 1`. */
void DestroyPolarPlayer(void *selfArg, u32 arg1param)
{
    MATCH_HOLD_REG(u8 *, self, r5) = selfArg;
    u32 arg1 = arg1param;

    *(u8 **)(self + 0x50) = (u8 *)gPolarPlayerVtable;

    if (gPolarQueuedWumpa != 0) {
        do {
            CollectWumpa(gLevelState);
            gPolarQueuedWumpa -= 1;
        } while (gPolarQueuedWumpa != 0);
    }

    FreeVramTileBlock(gPolarPlayerTiles[0]);
    FreeVramTileBlock(gPolarPlayerTiles[1]);

    *(u8 **)(self + 0x50) = (u8 *)gActorVtable;

    {
        u8 *next = *(u8 **)(self + 0x4c);
        u8 *prev = *(u8 **)(self + 0x48);
        *(u8 **)(next + 0x48) = prev;
    }
    {
        u8 *prev = *(u8 **)(self + 0x48);
        u8 *next = *(u8 **)(self + 0x4c);
        *(u8 **)(prev + 0x4c) = next;
    }

    if (arg1 & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");
