#include "core.h"
#include "memory.h"
#include "level_state.h"
#include "actor_self.h"
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "level.h"
#include "globals.h"

/* Continues the `InitActorPart`/`gActorList`-rooted "self" object
 * family documented in actor.c/polar_player_actions.c: a "part table"
 * pointer at `self+0`, a table-index/"kind" field at `self+0xc`, an
 * anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 * accumulator at `self+8`, state at `self+0x28`, a frame counter at
 * `self+0x44`, and the movement-threshold-cached position triple at
 * `self+0x1c`/`self+0x20`/`self+0x24`. This file's functions are mostly
 * `InitActorPart`-calling constructor variants (each installing a
 * different `self+0x50` event/trampoline table before doing a small
 * amount of table-specific setup) plus a couple of small self-standing
 * helpers operating on the unrelated `gLevelState`-rooted "player"
 * object's `+0x78` counter field (an Aku-Aku-mask-style add/remove
 * pair, `RemovePolarAkuAkuMask`/`AddPolarAkuAkuMask`) and a `gPolarAkuAku`-rooted
 * sibling object (`ClearPolarAkuAkuMask`). See docs/matching/issue-54-actor-d3a8.md. */

extern struct level_state *gLevelState;
extern void *gActorList;
extern s32 _call_via_r2(void *arg0, s32 arg1, void *fn);

/* `actor_self` plus the one-shot byte flag UpdatePolarBoostPad/CreatePolarBoostPad use.
 *
 * The `*(u8 *)&self->...animDone = zero` stores below are deliberate: as
 * plain struct-member stores gcc rebuilds the byte zero in r0 instead of
 * storing the register it was pinned to (same trick as airship_fireball.c). */
struct actor_once {
    struct actor_self base;
    u8 once;            // 0x54
};

/* Passes its argument through to `SetMaskLevel(gLevelState, 0)`,
 * then `RefreshPolarAkuAku(self, 0)` - a trivial reset pair on a different,
 * `gPolarAkuAku`-rooted object family, unrelated to this file's
 * `self` (see `polar_player_actions.c`'s `CatchPolarPlayer`, which calls this with
 * `gPolarAkuAku`). */
void ClearPolarAkuAkuMask(void *self)
{
    SetMaskLevel(gLevelState, 0);
    RefreshPolarAkuAku(self, 0);
}

/* "Remove a mask": plays a sound, decrements `gLevelState`'s
 * `+0x78` counter (floored at 0), pushes the new count via
 * `SetMaskLevel`, and always calls `RefreshPolarAkuAku(self, 1)`. Returns the
 * (possibly unchanged) counter. */
s32 RemovePolarAkuAkuMask(void *self)
{
    s32 count;

    PlaySfx(gAudioContext, 0, 0x100);
    count = gLevelState->maskLevel;
    if (count != 0) {
        count -= 1;
        SetMaskLevel(gLevelState, count);
    }
    RefreshPolarAkuAku(self, 1);
    return count;
}

/* "Add a mask" - the increment counterpart to `RemovePolarAkuAkuMask` above,
 * capped at 3, `RefreshPolarAkuAku(self, 0)` instead of `1`. */
s32 AddPolarAkuAkuMask(void *self)
{
    s32 count;

    PlaySfx(gAudioContext, 1, 0x100);
    count = gLevelState->maskLevel;
    if (count != 3) {
        count += 1;
        SetMaskLevel(gLevelState, count);
    }
    RefreshPolarAkuAku(self, 0);
    return count;
}

/* Constructor variant: calls `InitActorPart` with `b`/`c`/`d` offset by
 * fixed deltas (`-0x1000`/`-0x1E00`/`-0x200`, the same constants
 * `MovePolarAkuAku` uses for its own state-0 scatter targets - plausibly a
 * "spawn at scatter offset" helper feeding that function), installs the
 * `gPolarAkuAkuVtable` event table, then pushes the caller's own
 * 6th argument through `SetMaskLevel` before resetting state via
 * `RefreshPolarAkuAku(self, 0)`. */
void *CreatePolarAkuAku(struct actor_self *self, void *part, s32 b, s32 c, s32 d, s32 sixth)
{
    InitActorPart(self, part, b - 0x1000, c - 0x1E00, d - 0x200);
    self->vtable = (struct actor_vtable *)gPolarAkuAkuVtable;
    SetMaskLevel(gLevelState, sixth);
    RefreshPolarAkuAku(self, 0);
    return self;
}

/* Trivial forwarder: `SetMaskLevel(gLevelState, arg1)`, where
 * `arg1` is this function's own second parameter, passed straight
 * through in `r1` (the same "ignore my own first argument, forward my
 * second" shape as `FinishPolarRun` in actor.c). */
void SetPolarMaskLevel(void *arg0, s32 arg1)
{
    SetMaskLevel(gLevelState, arg1);
}

/* Trivial getter: `gLevelState`'s `+0x78` counter (the same field
 * `RemovePolarAkuAkuMask`/`AddPolarAkuAkuMask` above adjust). */
s32 GetPolarMaskLevel(void)
{
    return gLevelState->maskLevel;
}

/* Once `self`'s frame counter (`+0x44`) exceeds 5, sets `visible`
 * (+0x2c), so the part appears after 5 frames. Then, on `IsTouchingPlayer`'s trampoline-fire edge, calls
 * `FinishPolarRun(gActorList)` (the player object), and always
 * advances via `UpdateActor`. */
void UpdatePolarGoal(void *selfArg)
{
    register struct actor_self *self asm("r4") = selfArg;

    if (self->stateTime > 5) {
        self->visible = 1;
    }

    if ((u8)IsTouchingPlayer(self)) {
        FinishPolarRun(gActorList);
    }

    UpdateActor(self);
}

/* Plain `InitActorPart` passthrough constructor (no offset applied)
 * installing the `gPolarGoalVtable` event table and clearing
 * `visible` (+0x2c) (rather than setting it, unlike
 * `InitActorPart`'s own default of `1`). */
void *CreatePolarGoal(struct actor_self *self, void *part, s32 b, s32 c, s32 d)
{
    InitActorPart(self, part, b, c, d);
    self->vtable = (struct actor_vtable *)gPolarGoalVtable;
    self->visible = 0;
    return self;
}

/* On `IsTouchingPlayer`'s trampoline-fire edge, forwards `self+0x1c` to
 * `BoostPolarPlayer(gActorList, ...)` (the player object) and, the
 * first time through (guarded by a one-shot byte flag at `self+0x54`),
 * plays a sound. Always advances via `UpdateActor`. */
void UpdatePolarBoostPad(void *selfArg)
{
    register struct actor_once *self asm("r4") = selfArg;

    if ((u8)IsTouchingPlayer(self)) {
        BoostPolarPlayer(gActorList, self->base.x);
        {
            u8 *flag = &self->once;

            if (*flag == 0) {
                PlaySfx(gAudioContext, 0x28, 0x100);
                *flag = 1;
            }
        }
    }

    UpdateActor(self);
}

/* Constructor: `InitActorPart` passthrough (`b`==own `posY` argument,
 * reused below), installs `gPolarBoostPadVtable`, then classifies
 * `posY>>8` into a 3-way "kind" (`self+0xc`: 0 if `< -0x14`, 1 if
 * `<= 0x14`, else 2) that selects which of the part table's three
 * 0xc-stride anim records seeds `self+0x10`/`0x12`, and resets the
 * accumulator (`+8`) and the `+0x54` one-shot flag both to 0. */
void *CreatePolarBoostPad(struct actor_once *self, void *part, s32 posY, s32 c, s32 d)
{
    s32 classify = posY;
    s32 idx;

    InitActorPart(self, part, posY, c, d);
    self->base.vtable = (struct actor_vtable *)gPolarBoostPadVtable;

    classify >>= 8;

    {
        s32 low = -0x14;

        idx = 0;
        if (classify >= low) {
            idx = 1;
            if (classify > 0x14) {
                idx = 2;
            }
        }
    }

    self->base.animIndex = idx;
    {
        struct anim_frame_record *table = self->base.anims;
        u16 anim = table[idx].duration;
        register u8 zeroShared asm("r2") = 0;
        register s32 zeroAccum asm("r1") = 0;

        self->base.animTimer = anim;
        *(u8 *)&self->base.animDone = zeroShared;
        self->base.animTime = zeroAccum;
        self->once = zeroShared;
    }

    return self;
}

/* State machine: while `self+0xc` ("kind") is still 0, first checks
 * `IsTouchingPlayer`'s trampoline-fire edge (transitions to kind 1, seeds
 * anim from the part table's `+0xc` record, plays a sound, refreshes
 * the player via `AddBrokenCrate`, and fires `SetActorCheckpoint`/`CreatePolarCheckpointText`
 * position-tied calls), then - only if still kind 0 - checks
 * `IsTouchingYeti`'s AABB-overlap test (transitions to kind 3, seeds anim
 * from the `+0x24` record, arms `self+0x18`, plays a different sound,
 * and refreshes the player again). Finally, once kind==3 and the
 * anim-done flag (`+0x12`) is set, fires the `+0x50` table's slot-3
 * trampoline (guarded by a redundant `self != NULL` check matching the
 * ROM); otherwise advances via `UpdateActor`. */
void UpdatePolarCheckpointCrate(void *selfArg)
{
    register struct actor_self *self asm("r4") = selfArg;
    s32 kind = self->animIndex;

    if (kind == 0) {
        if ((u8)IsTouchingPlayer(self)) {
            self->animIndex = 1;
            {
                struct anim_frame_record *table = self->anims;
                u16 anim = table[1].duration;
                register u8 zero asm("r1") = 0;

                self->animTimer = anim;
                *(u8 *)&self->animDone = zero;
            }
            self->animTime = kind;

            PlaySfx(gAudioContext, 0x17, 0x100);
            AddBrokenCrate(gLevelState);
            SetActorCheckpoint(self->z);
            CreatePolarCheckpointText(self->x, self->y - 0xF00, self->z);
        }

        kind = self->animIndex;
        if (kind == 0 && IsTouchingYeti(self)) {
            self->animIndex = 3;
            {
                struct anim_frame_record *table = self->anims;
                u16 anim = table[3].duration;
                register u8 zero asm("r1") = 0;

                self->animTimer = anim;
                *(u8 *)&self->animDone = zero;
            }
            self->animTime = kind;
            self->palette = 1;

            PlaySfx(gAudioContext, 3, 0x100);
            AddBrokenCrate(gLevelState);
        }
    }

    if (self->animIndex == 3 && self->animDone != 0) {
        if (self != NULL) {
            struct actor_vtable *table = self->vtable;
            s32 offset = table->destroy.thisOffset;
            u8 *addr = (u8 *)self + offset;
            void *fn = table->destroy.fn;

            _call_via_r2(addr, 3, fn);
        }
    } else {
        UpdateActor(self);
    }
}

/* Constructor: `InitActorPart` passthrough installing
 * `gPolarCheckpointCrateVtable`, then - only if `GetActorCheckpoint()` equals the
 * caller's own `d` argument - transitions to kind 2 (anim from the part
 * table's `+0x18` record, accumulator/flag/counter all reset) and plays
 * a sound. */
void *CreatePolarCheckpointCrate(struct actor_self *self, void *part, s32 b, s32 c, s32 d)
{
    InitActorPart(self, part, b, c, d);
    self->vtable = (struct actor_vtable *)gPolarCheckpointCrateVtable;

    if (GetActorCheckpoint() == d) {
        self->animIndex = 2;
        {
            struct anim_frame_record *table = self->anims;
            u16 anim = table[2].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
            self->animTime = zero2;
        }

        PlaySfx(gAudioContext, 0x17, 0x100);
    }

    return self;
}

asm(".align 2, 0");
