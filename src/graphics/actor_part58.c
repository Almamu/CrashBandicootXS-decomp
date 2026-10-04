#include "core.h"
#include "memory.h"
#include "level_state.h"
#include "actor_self.h"

/* Continues the `InitActorPart`/`gActorList`-rooted "self" object
 * family documented in actor_part50.c/actor_part19.c: a "part table"
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
 * sibling object (`sub_802D490`). See docs/matching/issue-54-actor-d3a8.md. */

extern struct level_state *gLevelState;
extern void *gAudioContext;
extern s32 SetMaskLevel(struct level_state *arg0, s32 arg1);
extern void sub_802D204(void *self, s32 arg1);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void *InitActorPart(void *self, void *part, s32 b, s32 c, s32 d);
extern u8 sub_802A6EC(void *self);
extern void UpdateActor(void *self);
extern void *gActorList;
extern void sub_802BFD4(void *arg0);
extern void sub_802C0BC(void *selfArg, s32 arg1);
extern u8 sub_802DD9C(void *self);
extern void AddBrokenCrate(struct level_state *self);
extern s32 sub_8029748(s32 arg0);
extern void CreatePolarCheckpointText(s32 arg0, s32 arg1, s32 arg2);
extern s32 _call_via_r2(void *arg0, s32 arg1, void *fn);
extern s32 sub_802973C(void);

extern u8 gPolarAkuAkuVtable[];
extern u8 gStaticData_087E5074[];
extern u8 gStaticData_087E5094[];
extern u8 gPolarCheckpointCrateVtable[];

/* `actor_self` plus the one-shot byte flag sub_802D600/sub_802D648 use.
 *
 * The `*(u8 *)&self->...animDone = zero` stores below are deliberate: as
 * plain struct-member stores gcc rebuilds the byte zero in r0 instead of
 * storing the register it was pinned to (same trick as actor_part20.c). */
struct actor_once {
    struct actor_self base;
    u8 once;            // 0x54
};

/* Passes its argument through to `SetMaskLevel(gLevelState, 0)`,
 * then `sub_802D204(self, 0)` - a trivial reset pair on a different,
 * `gPolarAkuAku`-rooted object family, unrelated to this file's
 * `self` (see `actor_part19.c`'s `sub_802C018`, which calls this with
 * `gPolarAkuAku`). */
void sub_802D490(void *self)
{
    SetMaskLevel(gLevelState, 0);
    sub_802D204(self, 0);
}

/* "Remove a mask": plays a sound, decrements `gLevelState`'s
 * `+0x78` counter (floored at 0), pushes the new count via
 * `SetMaskLevel`, and always calls `sub_802D204(self, 1)`. Returns the
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
    sub_802D204(self, 1);
    return count;
}

/* "Add a mask" - the increment counterpart to `RemovePolarAkuAkuMask` above,
 * capped at 3, `sub_802D204(self, 0)` instead of `1`. */
s32 AddPolarAkuAkuMask(void *self)
{
    s32 count;

    PlaySfx(gAudioContext, 1, 0x100);
    count = gLevelState->maskLevel;
    if (count != 3) {
        count += 1;
        SetMaskLevel(gLevelState, count);
    }
    sub_802D204(self, 0);
    return count;
}

/* Constructor variant: calls `InitActorPart` with `b`/`c`/`d` offset by
 * fixed deltas (`-0x1000`/`-0x1E00`/`-0x200`, the same constants
 * `MovePolarAkuAku` uses for its own state-0 scatter targets - plausibly a
 * "spawn at scatter offset" helper feeding that function), installs the
 * `gPolarAkuAkuVtable` event table, then pushes the caller's own
 * 6th argument through `SetMaskLevel` before resetting state via
 * `sub_802D204(self, 0)`. */
void *CreatePolarAkuAku(struct actor_self *self, void *part, s32 b, s32 c, s32 d, s32 sixth)
{
    InitActorPart(self, part, b - 0x1000, c - 0x1E00, d - 0x200);
    self->vtable = (struct actor_vtable *)gPolarAkuAkuVtable;
    SetMaskLevel(gLevelState, sixth);
    sub_802D204(self, 0);
    return self;
}

/* Trivial forwarder: `SetMaskLevel(gLevelState, arg1)`, where
 * `arg1` is this function's own second parameter, passed straight
 * through in `r1` (the same "ignore my own first argument, forward my
 * second" shape as `sub_802BFD4` in actor_part50.c). */
void sub_802D57C(void *arg0, s32 arg1)
{
    SetMaskLevel(gLevelState, arg1);
}

/* Trivial getter: `gLevelState`'s `+0x78` counter (the same field
 * `RemovePolarAkuAkuMask`/`AddPolarAkuAkuMask` above adjust). */
s32 sub_802D590(void)
{
    return gLevelState->maskLevel;
}

/* Once `self`'s frame counter (`+0x44`) exceeds 5, latches the one-shot
 * flag at `+0x2c`. Then, on `sub_802A6EC`'s trampoline-fire edge, calls
 * `sub_802BFD4(gActorList)` (the player object), and always
 * advances via `UpdateActor`. */
void sub_802D59C(void *selfArg)
{
    register struct actor_self *self asm("r4") = selfArg;

    if (self->stateTime > 5) {
        self->unk_2C[0] = 1;
    }

    if (sub_802A6EC(self)) {
        sub_802BFD4(gActorList);
    }

    UpdateActor(self);
}

/* Plain `InitActorPart` passthrough constructor (no offset applied)
 * installing the `gStaticData_087E5074` event table and clearing the
 * one-shot flag at `+0x2c` (rather than setting it, unlike
 * `InitActorPart`'s own default of `1`). */
void *sub_802D5D4(struct actor_self *self, void *part, s32 b, s32 c, s32 d)
{
    InitActorPart(self, part, b, c, d);
    self->vtable = (struct actor_vtable *)gStaticData_087E5074;
    self->unk_2C[0] = 0;
    return self;
}

/* On `sub_802A6EC`'s trampoline-fire edge, forwards `self+0x1c` to
 * `sub_802C0BC(gActorList, ...)` (the player object) and, the
 * first time through (guarded by a one-shot byte flag at `self+0x54`),
 * plays a sound. Always advances via `UpdateActor`. */
void sub_802D600(void *selfArg)
{
    register struct actor_once *self asm("r4") = selfArg;

    if (sub_802A6EC(self)) {
        sub_802C0BC(gActorList, self->base.x);
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
 * reused below), installs `gStaticData_087E5094`, then classifies
 * `posY>>8` into a 3-way "kind" (`self+0xc`: 0 if `< -0x14`, 1 if
 * `<= 0x14`, else 2) that selects which of the part table's three
 * 0xc-stride anim records seeds `self+0x10`/`0x12`, and resets the
 * accumulator (`+8`) and the `+0x54` one-shot flag both to 0. */
void *sub_802D648(struct actor_once *self, void *part, s32 posY, s32 c, s32 d)
{
    s32 classify = posY;
    s32 idx;

    InitActorPart(self, part, posY, c, d);
    self->base.vtable = (struct actor_vtable *)gStaticData_087E5094;

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
 * `sub_802A6EC`'s trampoline-fire edge (transitions to kind 1, seeds
 * anim from the part table's `+0xc` record, plays a sound, refreshes
 * the player via `AddBrokenCrate`, and fires `sub_8029748`/`CreatePolarCheckpointText`
 * position-tied calls), then - only if still kind 0 - checks
 * `sub_802DD9C`'s AABB-overlap test (transitions to kind 3, seeds anim
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
        if (sub_802A6EC(self)) {
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
            sub_8029748(self->z);
            CreatePolarCheckpointText(self->x, self->y - 0xF00, self->z);
        }

        kind = self->animIndex;
        if (kind == 0 && sub_802DD9C(self)) {
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
 * `gPolarCheckpointCrateVtable`, then - only if `sub_802973C()` equals the
 * caller's own `d` argument - transitions to kind 2 (anim from the part
 * table's `+0x18` record, accumulator/flag/counter all reset) and plays
 * a sound. */
void *CreatePolarCheckpointCrate(struct actor_self *self, void *part, s32 b, s32 c, s32 d)
{
    InitActorPart(self, part, b, c, d);
    self->vtable = (struct actor_vtable *)gPolarCheckpointCrateVtable;

    if (sub_802973C() == d) {
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
