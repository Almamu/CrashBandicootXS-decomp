#include "core.h"
#include "actor_self.h"

/* Continuation of actor_part19.c's player/action-object family, right
 * after the still-raw `DetonateNearbyPolarNitros` (see docs/matching.md) - same
 * `self` object and conventions documented there. */

extern void *gAudioContext;
extern void *gLevelState;
extern void *gActorList;

extern u8 IsTouchingPlayer(void *self);
extern u8 IsTouchingYeti(void *self);
extern void AddBrokenCrate(void *self);
extern void GivePolarPlayerMask(void *arg0);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void UpdatePolarCrate(void *selfArg);

/* On proximity (`IsTouchingPlayer`), ties the lap counter and the lock-timer
 * setter `GivePolarPlayerMask`, then transitions to the shared "used"
 * animation sequence 0x12. Whether or
 * not that fired, on `IsTouchingYeti`'s overlap test transitions a second
 * time with its own sound cue - both share the same state-0x12
 * transition block (plus `self->palette = 1`) before tail-calling the
 * shared cleanup `UpdatePolarCrate`.
 *
 * The `*(T *)&self->...` stores are deliberate: through a pointer they aren't
 * marked as struct-member accesses, which keeps gcc's scheduler from
 * moving the `anims[0x12]` load below the zero constants (the ROM loads
 * it first); plain member stores reorder it. */
void UpdatePolarAkuAkuCrate(struct actor_self *self)
{
    if (self->animIndex != 0x12 && IsTouchingPlayer(self)) {
        AddBrokenCrate(gLevelState);
        GivePolarPlayerMask(gActorList);
        self->animIndex = 0x12;
        {
            register u16 anim asm("r0") = self->anims[0x12].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
            self->animTime = zero2;
        }
        self->palette = 1;
    }

    if (self->animIndex != 0x12 && IsTouchingYeti(self)) {
        PlaySfx(gAudioContext, 3, 0x100);
        AddBrokenCrate(gLevelState);
        self->animIndex = 0x12;
        {
            register u16 anim asm("r0") = self->anims[0x12].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
            self->animTime = zero2;
        }
        self->palette = 1;
    }

    UpdatePolarCrate(self);
}

asm(".align 2, 0");

/* Sits right after actor_part19d.c's `UpdatePolarAkuAkuCrate` and before the
 * still-raw remainder of `asm/code_3_2_20_28568_c99c.s` (starting at
 * `UpdatePolarElectricFence`) - directly adjacent to `actor_part19d.c` now, closing
 * part of GitHub issue #53's `0x0802C99C`-`0x0802D3A8` chunk. Same
 * `self` object (`struct actor_self`) and conventions documented in `actor_part19g.c` (the
 * `animIndex` state field, the `+0x10`/`+0x12`/`+8` anim-reset idiom,
 * the `+0x30` type-byte indirection, and the `InitActorPart`-based
 * constructor family already matched throughout this ROM region). */

extern void FreezeLevelClock(void *arg0, s32 arg1);
extern void QueuePolarWumpa(void *arg0, s32 delta);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern s32 __divsi3(s32 arg0, s32 arg1);

extern u8 gPolarCrateVtable[];
extern u8 gPolarTimeCrateVtable[];
extern u8 gPolarQuestionCrateVtable[];
extern u8 gPolarAkuAkuCrateVtable[];
extern u8 gPolarNitroCrateVtable[];
extern u8 gPolarLifeCrateVtable[];
extern u8 gStaticData_087E4F54[];
extern u8 gPolarBasicCrateVtable[];

/* Extends the type-byte event dispatch family (`UpdateJetpackTimeCrate`/etc, per
 * docs/rom_map.md; the `UpdatePolarQuestionCrate` shape in actor_part19g.c) with
 * values `5`-`7`. On proximity (`IsTouchingPlayer`), plays a sound, ties the
 * lap counter, then dispatches `FreezeLevelClock` with a tier argument keyed
 * off `self+0x30`'s type byte (`5`->1, `6`->2, `7`->anything else
 * dispatches nothing) before the shared "used" state transition;
 * tail-calls `UpdatePolarCrate`. */
void UpdatePolarTimeCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12 && IsTouchingPlayer(self)) {
        s32 typeByte;

        PlaySfx(gAudioContext, 3, 0x100);
        AddBrokenCrate(gLevelState);

        typeByte = **(u8 **)((u8 *)self + 0x30);

        switch (typeByte) {
        case 5:
            FreezeLevelClock(gLevelState, 1);
            break;
        case 6:
            FreezeLevelClock(gLevelState, 2);
            break;
        case 7:
            FreezeLevelClock(gLevelState, 3);
            break;
        }

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

/* Unconditional (no `IsTouchingPlayer` proximity guard) "used"-state
 * transition: plays a sound, ties the lap counter, clears `stateTime`,
 * then the usual state-0x12/anim-reset block. No tail call - the
 * caller drives whatever comes after directly. */
void sub_802CA28(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12) {
        PlaySfx(gAudioContext, 4, 0x100);
        AddBrokenCrate(gLevelState);
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
    }
}

/* Same proximity-gated "used" transition shape as `UpdatePolarQuestionCrate`/
 * `UpdatePolarLifeCrate` (actor_part19g.c), forwarding a fixed accumulator
 * delta of `4` to `QueuePolarWumpa(gActorList, ...)`; tail-calls
 * `UpdatePolarCrate`. */
void sub_802CA6C(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12 && IsTouchingPlayer(self)) {
        PlaySfx(gAudioContext, 3, 0x100);
        AddBrokenCrate(gLevelState);
        QueuePolarWumpa(gActorList, 4);
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

/* Same shape as `sub_802CA6C` above, accumulator delta `1` instead of
 * `4`. */
void UpdatePolarBasicCrate(void *selfArg)
{
    struct actor_self *self = selfArg;

    if (self->animIndex != 0x12 && IsTouchingPlayer(self)) {
        PlaySfx(gAudioContext, 3, 0x100);
        AddBrokenCrate(gLevelState);
        QueuePolarWumpa(gActorList, 1);
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

/* `InitActorPart`-based constructor: forwards `a`/`b`/`c`/`lastArg`
 * straight through, installs `self+0x50 = gPolarCrateVtable`, then
 * classifies a "kind" (`animIndex`) from a `__divsi3`-scaled
 * function of `b` (clamped to `[0, 5]`) plus up to two `+6` bumps keyed
 * off `c`'s own range - selecting one of up to 18 per-kind anim
 * records from the part table (`self[0]`, stride `0xc`) to seed
 * `animTimer`/`animDone`/`self+8`, the same idiom as `CreatePolarBoostPad`
 * (`src/graphics/actor_part58.c`). */
void *InitPolarCrate(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;
    s32 idx;

    InitActorPart(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gPolarCrateVtable;

    idx = __divsi3((b >> 8) + 0x3c, 0x14);

    if (idx < 0) {
        idx = 0;
    }
    if (idx > 5) {
        idx = 5;
    }

    {
        s32 cShifted = c >> 8;

        if (cShifted <= 0x2b) {
            idx += 6;
        }
        if (cShifted <= 6) {
            idx += 6;
        }
    }

    self->animIndex = idx;
    {
        register u8 *table asm("r1") = (u8 *)self->anims;
        u16 anim = *(u16 *)(idx * 0xc + table);
        register u8 zero1 asm("r1") = 0;
        register s32 zero2 asm("r2") = 0;

        *(u16 *)&self->animTimer = anim;
        *(u8 *)&self->animDone = zero1;
        *(s32 *)&self->animTime = zero2;
    }

    return self;
}

/* Thin `InitPolarCrate`-forwarding constructor, `vtable` overridden to
 * `gPolarTimeCrateVtable`. */
void *CreatePolarTimeCrate(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;

    InitPolarCrate(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gPolarTimeCrateVtable;
    return self;
}

/* Same shape as `CreatePolarTimeCrate`, `self+0x50 = gPolarQuestionCrateVtable`. */
void *CreatePolarQuestionCrate(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;

    InitPolarCrate(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gPolarQuestionCrateVtable;
    return self;
}

/* Same shape as `CreatePolarTimeCrate`, `self+0x50 = gPolarAkuAkuCrateVtable`. */
void *CreatePolarAkuAkuCrate(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;

    InitPolarCrate(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gPolarAkuAkuCrateVtable;
    return self;
}

/* Same shape as `CreatePolarTimeCrate`, `self+0x50 = gPolarNitroCrateVtable`. */
void *CreatePolarNitroCrate(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;

    InitPolarCrate(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gPolarNitroCrateVtable;
    return self;
}

/* Same shape as `CreatePolarTimeCrate`, `self+0x50 = gPolarLifeCrateVtable`, plus
 * a 6th argument stashed straight into `self+0x54`. */
void *CreatePolarLifeCrate(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg, s32 arg6)
{
    struct actor_self *self = selfArg;

    InitPolarCrate(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gPolarLifeCrateVtable;
    *(s32 *)((u8 *)self + 0x54) = arg6;
    return self;
}

/* Same shape as `CreatePolarTimeCrate`, `self+0x50 = gStaticData_087E4F54`. */
void *sub_802CC54(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;

    InitPolarCrate(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gStaticData_087E4F54;
    return self;
}

/* Same shape as `CreatePolarTimeCrate`, `self+0x50 = gPolarBasicCrateVtable`. */
void *CreatePolarBasicCrate(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    struct actor_self *self = selfArg;

    InitPolarCrate(self, a, b, c, lastArg);
    self->vtable = (struct actor_vtable *)gPolarBasicCrateVtable;
    return self;
}

asm(".align 2, 0");
