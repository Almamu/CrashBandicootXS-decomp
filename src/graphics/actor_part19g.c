#include "core.h"
#include "actor_self.h"

/* Continuation of actor_part19.c's player/action-object family, right
 * after the parked `CreatePolarCollectedWumpa` (see actor_part19c2.c) - same `self`
 * object (`struct actor_self`) and conventions documented there. The
 * animation-reset blocks (the `anim`/`zero1`/`zero2` register trios)
 * store through `*(T *)&self->field` casts: plain member stores let
 * gcc move the zero loads (docs/workflow.md step 7). */

/* Anonymous 12-byte (3-word) copy unit - see actor_part19c.c's own
 * copy of this comment for why this shape (rather than three separate
 * `s32` field copies) is needed to reproduce the ROM's `ldm`/`stm`
 * lowering for `self+0x38`'s refresh from `gPolarNitroCrateBox`. */
struct vec3_words {
    s32 a, b, c;
};

/* `UpdatePolarLifeCrate`'s class adds one field after the common prefix: an
 * object it hands to `MarkSpawnCollected`'s 15-entry list. */
struct listed_actor {
    struct actor_self base;
    void *unk_54;               // 0x54
};

extern void *gAudioContext;
extern void *gLevelState;
extern void *gActorList;

extern u8 gPolarWumpaVtable[];
extern u8 gPolarNitroCrateBox[];

extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern u8 IsTouchingPlayer(void *self);
extern void UpdateActor(void *self);
extern u8 IsTouchingYeti(void *self);
extern void AddBrokenCrate(void *self);
extern void MarkSpawnCollected(void *arg0);
extern void HurtPolarPlayer(void *arg0);
extern void AddActorMissedNitro(void);
extern void DetonateNearbyPolarNitros(void *self);
extern void QueuePolarWumpa(void *arg0, s32 delta);
extern void GivePolarPlayerMask(void *arg0);
extern void GivePolarPlayerLife(void *arg0);
extern void UpdatePolarCrate(void *selfArg);

/* On proximity (`IsTouchingPlayer`), accumulates `1` into the shared
 * `gActorList`-targeted accumulator via `QueuePolarWumpa` then fires
 * the `vtable` trampoline (behind this family's `if (self)` guard);
 * otherwise tail-calls `UpdateActor(self)`. */
void UpdatePolarWumpa(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (IsTouchingPlayer(self)) {
        QueuePolarWumpa(gActorList, 1);
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->destroy.thisOffset, (void *)3, table->destroy.fn);
        }
    } else {
        UpdateActor(self);
    }
}

/* Thin `InitActorPart`-based constructor: forwards its own `a`/`b`/`c`
 * parameters straight through (untouched, same registers) plus the
 * caller's last stack argument, then sets `vtable` to
 * `gPolarWumpaVtable` - one of the "spawn effect type N" family
 * documented in docs/rom_map.md. */
void *CreatePolarWumpa(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;

    InitActorPart(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gPolarWumpaVtable;
    return self;
}

/* Shared cleanup/tail step for this actor family (per docs/rom_map.md):
 * once (state != 0x12 and `IsTouchingYeti`'s overlap test passes),
 * transitions to the shared "used" state 0x12 (anim frame from
 * `self`'s part-table pointer at `+0xd8`) with a sound cue and the
 * lap-counter tie `AddBrokenCrate`. Either way, fires the `vtable`
 * trampoline once state is (already, or now) 0x12 and `animDone` is
 * set; otherwise tail-calls `UpdateActor`. */
void UpdatePolarCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12) {
        if (IsTouchingYeti(self)) {
            PlaySfx(gAudioContext, 3, 0x100);
            AddBrokenCrate(gLevelState);
            self->animIndex = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                register u8 zero1 asm("r1") = 0;
                register s32 zero2 asm("r2") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
                *(s32 *)&self->animTime = zero2;
            }
        }
    }

    if (self->animIndex == 0x12 && self->animDone != 0) {
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->destroy.thisOffset, (void *)3, table->destroy.fn);
        }
        return;
    }

    UpdateActor(self);
}

/* Extends the shared "type-byte event dispatch" family
 * (`UpdateJetpackTimeCrate`/etc., per docs/rom_map.md) to value range `0x1c`-
 * `0x1f`, reading the type byte through one extra pointer indirection
 * (`self+0x30`). Ties into the wraparound-lap-counter system via
 * `AddBrokenCrate` and dispatches accumulator/lock-timer calls
 * (`QueuePolarWumpa`/`GivePolarPlayerMask`) before tail-calling the shared cleanup
 * `UpdatePolarCrate`. */
void UpdatePolarQuestionCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12 && IsTouchingPlayer(self)) {
        s32 typeByte;

        AddBrokenCrate(gLevelState);
        typeByte = **(u8 **)((u8 *)self + 0x30);

        if (typeByte == 0x1d) {
            goto case_1d;
        }
        if (typeByte > 0x1d) {
            goto gt_1d_dispatch;
        }
        if (typeByte == 0x1c) {
            goto case_1c;
        }
        goto state_block;

    gt_1d_dispatch:
        if (typeByte == 0x1e) {
            goto case_1e;
        }
        if (typeByte == 0x1f) {
            goto case_1f;
        }
        goto state_block;

    case_1c:
        PlaySfx(gAudioContext, 3, 0x100);
        QueuePolarWumpa(gActorList, 1);
        goto state_block;

    case_1d:
        PlaySfx(gAudioContext, 3, 0x100);
        QueuePolarWumpa(gActorList, 3);
        goto state_block;

    case_1e:
        PlaySfx(gAudioContext, 3, 0x100);
        QueuePolarWumpa(gActorList, 5);
        goto state_block;

    case_1f:
        GivePolarPlayerMask(gActorList);

    state_block:
        self->animIndex = 0x12;
        {
            register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
            *(s32 *)&self->animTime = zero2;
        }
    }

    UpdatePolarCrate(self);
}

/* Extends the lap-counter/proximity-dispatch family: on proximity
 * (`IsTouchingPlayer`), plays a sound, ties the lap counter, forwards the
 * global player pointer to `GivePolarPlayerLife` and `unk_54` to
 * `MarkSpawnCollected`, then (whether or not that first branch fired) on
 * `IsTouchingYeti`'s overlap test transitions to the shared "used" state
 * a second time with its own sound cue - both branches finish with the
 * same state-0x12 transition block before tail-calling `UpdatePolarCrate`. */
void UpdatePolarLifeCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12) {
        if (IsTouchingPlayer(self)) {
            PlaySfx(gAudioContext, 7, 0x100);
            AddBrokenCrate(gLevelState);
            GivePolarPlayerLife(gActorList);
            MarkSpawnCollected(((struct listed_actor *)self)->unk_54);
            self->animIndex = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                register u8 zero1 asm("r1") = 0;
                register s32 zero2 asm("r2") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
                *(s32 *)&self->animTime = zero2;
            }
            self->palette = 1;
        }

        if (self->animIndex != 0x12 && IsTouchingYeti(self)) {
            PlaySfx(gAudioContext, 3, 0x100);
            AddBrokenCrate(gLevelState);
            self->animIndex = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                register u8 zero1 asm("r1") = 0;
                register s32 zero2 asm("r2") = 0;

                *(u16 *)&self->animTimer = anim;
                *(u8 *)&self->animDone = zero1;
                *(s32 *)&self->animTime = zero2;
            }
            self->palette = 1;
        }
    }

    UpdatePolarCrate(self);
}

/* Once already in the "used" state (0x12): refreshes `self+0x38`
 * (a 12-byte AABB, from `gPolarNitroCrateBox`) and, once
 * `stateTime` reaches `0x14`, calls `DetonateNearbyPolarNitros` (still raw - see
 * docs/matching.md). Otherwise, while `depth` (a lap/lifetime
 * counter) exceeds `0xa000`, calls `AddActorMissedNitro` and fires the
 * `vtable` trampoline; else on proximity or overlap, plays a sound,
 * ties the lap counter and the homing-chase helper `HurtPolarPlayer`,
 * and transitions to the "used" state. Tail-calls `UpdatePolarCrate`. */
void UpdatePolarNitroCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex == 0x12) {
        goto usedState;
    }

    if (self->depth > 0xa000) {
        AddActorMissedNitro();
        if (self != 0) {
            struct actor_vtable *table = self->vtable;
            _call_via_r2((u8 *)self + table->destroy.thisOffset, (void *)3, table->destroy.fn);
        }
        return;
    } else {
        register u32 raw asm("r0") = IsTouchingPlayer(self);
        register u32 found asm("r5");

        raw = raw << 24;
        found = raw >> 24;

        if (found) {
            PlaySfx(gAudioContext, 4, 0x100);
            AddBrokenCrate(gLevelState);
            HurtPolarPlayer(gActorList);
            {
                register s32 zero2 asm("r2") = 0;

                self->stateTime = zero2;
                self->animIndex = 0x12;
                {
                    register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero1;
                }
                *(s32 *)&self->animTime = zero2;
            }
        } else if (IsTouchingYeti(self)) {
            PlaySfx(gAudioContext, 4, 0x100);
            AddBrokenCrate(gLevelState);
            {
                register s32 zero2 asm("r5") = found;

                self->stateTime = zero2;
                self->animIndex = 0x12;
                {
                    register u16 anim asm("r0") = *(u16 *)&self->anims[18].duration;
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)&self->animTimer = anim;
                    *(u8 *)&self->animDone = zero1;
                }
                *(s32 *)&self->animTime = zero2;
            }
        }
    }
    goto tail;

usedState:
    *(struct vec3_words *)((u8 *)self + 0x38) = *(struct vec3_words *)gPolarNitroCrateBox;

    if (self->stateTime == 0x14) {
        DetonateNearbyPolarNitros(self);
    }

tail:
    UpdatePolarCrate(self);
}

asm(".align 2, 0");
