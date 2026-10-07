#include "core.h"
#include "match.h"
#include "crate.h"
#include "crates.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see crate_reset.c's header comment and
 * docs/matching/archive/issue-13-graphics-fc70.md). `GetTopCrate`/
 * `GetBottomCrate`/`CollideCrateWithPlayer` right before this function are left
 * untouched raw; `DecrementSlotCrateStage` right after it is outside this issue's
 * range and also stays raw. */

/* Extracts `u48` (+0x48) bits 6-7 (a 2-bit sub-state field packed
 * alongside the low bits other functions in this subsystem test via
 * `& 7`). */
u32 GetSlotCrateStage(struct crate *self)
{
    return ((u32)self->u48.slotState & CRATE_SLOT_STAGE_MASK) >> CRATE_SLOT_STAGE_SHIFT;
}

/* GitHub issue #14: 0x08010A0C-0x08010D54, continuing the physics/
 * collision subsystem (`crate_reset.c`-`slot_crate.c`, see
 * docs/matching/archive/issue-13-graphics-fc70.md). `GetSlotCrateStage` right before
 * this function is already matched in slot_crate.c; everything here
 * operates on the crate, as `GetSlotCrateStage`/`ResetCrate` do
 * (`struct crate`, include/crate.h). */

/* Decrements `self+0x48`'s bits 6-7 sub-state by one, if it isn't
 * already zero. */
void DecrementSlotCrateStage(struct crate *self)
{
    u8 state = (u8)GetSlotCrateStage(self);

    if (state != 0) {
        u8 newState = (u8)(state - 1);
        self->u48.slotState =
            (self->u48.slotState & CRATE_SLOT_CLEAR_STAGE) | (newState << CRATE_SLOT_STAGE_SHIFT);
    }
}

/* Setter for the slot crate's stage (`u48.slotState` bits 6-7). */
void SetSlotCrateStage(struct crate *self, u32 state)
{
    u8 s = (u8)state;
    self->u48.slotState =
        (self->u48.slotState & CRATE_SLOT_CLEAR_STAGE) | (s << CRATE_SLOT_STAGE_SHIFT);
}

/* Clears the slot crate's stage (`u48.slotState` bits 6-7). */
void ClearSlotCrateStage(struct crate *self)
{
    u32 v = self->u48.slotState;
    v &= CRATE_SLOT_CLEAR_STAGE;
    self->u48.slotState = v;
}

/* Getter for the slot crate's spins left (`u48.slotState` bits 3-5). */
u32 GetSlotCrateSpins(struct crate *self)
{
    return ((u32)self->u48.slotState & CRATE_SLOT_SPINS_MASK) >> CRATE_SLOT_SPINS_SHIFT;
}

/* Decrements `self+0x48`'s bits 3-5 sub-state by one, if it isn't
 * already zero - same shape as `DecrementSlotCrateStage` for the neighboring
 * bit-field. */
void DecrementSlotCrateSpins(struct crate *self)
{
    u8 state = (u8)GetSlotCrateSpins(self);

    if (state != 0) {
        u8 newState = (u8)(state - 1);
        self->u48.slotState =
            (self->u48.slotState & CRATE_SLOT_CLEAR_SPINS) | (newState << CRATE_SLOT_SPINS_SHIFT);
    }
}

/* Setter for the slot crate's spins left (`u48.slotState` bits 3-5). */
void SetSlotCrateSpins(struct crate *self, u32 state)
{
    u8 s = (u8)state;
    self->u48.slotState =
        (self->u48.slotState & CRATE_SLOT_CLEAR_SPINS) | (s << CRATE_SLOT_SPINS_SHIFT);
}

/* Setter for the slot crate's phase (`u48.slotState` bits 0-2). */
void SetSlotCratePhase(struct crate *self, u32 state)
{
    u8 s = (u8)state;
    self->u48.slotState = (self->u48.slotState & CRATE_SLOT_CLEAR_PHASE) | s;
}

/* Getter for the slot crate's phase (`u48.slotState` bits 0-2). */
u32 GetSlotCratePhase(struct crate *self)
{
    return self->u48.slotState & CRATE_SLOT_PHASE_MASK;
}

void SetCrateKind(struct crate *self, u8 val)
{
    self->kind = val;
}

u8 GetCrateKind(struct crate *self)
{
    return self->kind;
}

void SetCrateFallDistance(struct crate *self, s32 val)
{
    self->fallDistance = val;
}

s32 GetCrateFallDistance(struct crate *self)
{
    return self->fallDistance;
}

/* Overwrites `self+0x4d`'s low 7 bits with `val`, preserving bit 7. */
void SetCrateState(struct crate *self, u32 val)
{
    u8 v = (u8)val;
    u8 *p = &self->state;
    u32 mask = CRATE_STATE_BUSY;
    *p = v | (mask & *p);
}

u32 GetCrateState(struct crate *self)
{
    /* Register-pinned: the ROM copies `self` into r1 before advancing
     * it, keeping r0 free for the mask constant - a plain `self[0x4d] &
     * 0x7f` lets this compiler reuse r0 as the address register
     * instead, dropping the ROM's own `adds r1, r0, #0` copy. */
    MATCH_HOLD_REG(u8 *, p, r1) = &self->state;
    MATCH_HOLD_REG(u32, mask, r0) = CRATE_STATE_MASK;
    MATCH_HOLD_REG(u8, v, r1) = *p;
    return mask & v;
}

void SetCrateFallSpeed(struct crate *self, u8 val)
{
    self->fallSpeed = val;
}

/* Sign-extending byte getter. */
s32 GetCrateFallSpeed(struct crate *self)
{
    return self->fallSpeed;
}

/* The original disassembly never gave this one its own label/symbol -
 * it sits directly after `GetCrateFallSpeed`'s padding, at the address the
 * `bx lr`/alignment arithmetic works out to. Boolean getter for
 * `self+0x4d` bit 7. */
u32 IsCrateBusy(struct crate *self)
{
    MATCH_HOLD_REG(u8 *, p, r0) = &self->state;
    MATCH_HOLD_REG(u32, mask, r1) = CRATE_STATE_BUSY;
    MATCH_HOLD_REG(u8, v, r0) = *p;
    mask &= v;
    if (mask != 0) {
        return 1;
    }
    return 0;
}

/* Sets `self+0x4d` bit 7 and the global "hit" latch
 * `gPlayer->busy`. */
void SetCrateBusy(struct crate *self)
{
    u8 *p = &self->state;
    u32 mask = CRATE_STATE_BUSY;
    u8 v = mask | *p;
    struct player *g;
    u32 one;
    *p = v;
    g = gPlayer;
    one = 1;
    g->busy = one;
}

/* Clears `self+0x4d` bit 7 and the global "hit" latch
 * `gPlayer->busy`. */
void ClearCrateBusy(struct crate *self)
{
    u8 *p = &self->state;
    u32 mask = CRATE_STATE_MASK;
    u8 v = mask & *p;
    u32 zero = 0;
    *p = v;
    gPlayer->busy = zero;
}

void SetCrateTouched(struct crate *self, u8 val)
{
    self->touched = val;
}

u8 GetCrateParamB(struct crate *self)
{
    return self->paramB;
}

u8 GetCrateParamA(struct crate *self)
{
    return self->paramA;
}

/* Overwrites the whole `self+0x48` field with a zero-extended byte
 * (unlike `SetSlotCratePhase`, which masks - this one clobbers all bits). */
void SetCrateSolidKind(struct crate *self, u32 val)
{
    self->u48.solidKind = (u8)val;
}

void SetCrateTrialKind(struct crate *self, s32 val)
{
    self->trialKind = val;
}

s32 GetCrateTrialKind(struct crate *self)
{
    return self->trialKind;
}
