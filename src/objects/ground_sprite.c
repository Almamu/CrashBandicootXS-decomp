#include "core.h"
#include "math_util.h"
#include "match.h"
#include "actor.h"
#include "gfx_part.h"
#include "gobj_1a794.h"
#include "objects.h"
#include "memory.h"

/* Void tail-call wrapper around the already-matched `DrawSpriteObj`. */
void DrawGroundSprite(void *arg0)
{
    DrawSpriteObj(arg0);
}

/* Constant-6 stub. */
s32 GetGroundSpriteClassId(void)
{
    return 6;
}

/* Same shape as `CreateMovingSprite`/etc.: allocates a bigger (0x80-byte)
 * part-object, re-initializes it via `InitMovingSprite`, overwrites its
 * table with `gGroundSpriteVtable`, clears it via `ResetGroundSprite`
 * below, then sets `field_08` and the Q8 `x`/`y` position from the
 * three `u16` arguments. */
void *CreateGroundSprite(u16 arg0, u16 arg1, u16 arg2, u16 unused)
{
    struct actor *part = OperatorNew(0x80);

    InitMovingSprite(part);
    part->table = (void *)gGroundSpriteVtable;
    ResetGroundSprite(part);
    part->id = arg0;
    part->x = INT_TO_Q8((s32)arg1);
    part->y = INT_TO_Q8((s32)arg2);
    return part;
}

/* Overwrites `self->table` with `gGroundSpriteVtable`, then tail-
 * calls `DestroyMovingSprite` - which unconditionally overwrites `table`
 * again with `gMovingSpriteVtable` and fires its own trampoline, so
 * this function's own table write only matters transiently (read by
 * nothing before `DestroyMovingSprite` clobbers it). `unusedArg` is passed
 * straight through to `DestroyMovingSprite`'s own second parameter without
 * this function ever touching it itself - the ROM leaves it in
 * whatever register its own caller happened to leave it in. */
void DestroyGroundSprite(struct actor *self, u32 unusedArg)
{
    self->table = (void *)gGroundSpriteVtable;
    DestroyMovingSprite(self, unusedArg);
}

/* Part-object field clearer/initializer, the `gGroundSpriteVtable`-
 * table sibling of `ResetMovingSprite`'s own `gMovingSpriteVtable`-table
 * clearer: sets `flags` bits 6/7, zeroes the speeds and speed ramps
 * `ApplySpriteVelocity` consumes (`speedX`/`speedY`, `rampX`/`rampY`) plus
 * `dir`/`mover`/`type`/`lastHitbox`, sets `hitAxes` to 8, and (unlike
 * `ResetMovingSprite`) sets `flags2` bit 0 instead of clearing bit 3. */
void ResetGroundSprite(void *selfArg)
{
    struct gobj *self = selfArg;

    {
        MATCH_HOLD_REG(s32, mask1, r0) = 0x80;
        MATCH_HOLD_REG(s32, curFlags, r1) = self->flags;
        MATCH_HOLD_REG(s32, combined, r0);

        combined = mask1 | curFlags;
        {
            MATCH_HOLD_REG(s32, mask2, r1) = 0x40;
            MATCH_HOLD_REG(s32, result, r0);

            result = combined | mask2;
            self->flags = result;
        }
    }
    {
        MATCH_HOLD_REG(s32, zero, r0) = 0;

        self->speedX = zero;
        self->speedY = zero;
        self->rampX.start = zero;
        self->rampX.step = zero;
        self->rampX.target = zero;
        self->rampY.start = zero;
        self->rampY.step = zero;
        self->rampY.target = zero;
        {
            MATCH_HOLD_REG(u8 *, addr68, r2) = &self->hitAxes;
            MATCH_HOLD_REG(s32, eight, r1) = 8;

            *addr68 = eight;
        }
        {
            MATCH_HOLD_REG(u8 *, addr24, r1) = &self->dir;

            *addr24 = zero;
        }
        self->mover = (struct mover *)zero;
        self->type = zero;
        self->lastHitbox = (void *)zero;
    }
    {
        MATCH_HOLD_REG(s32, mask, r0) = 1;
        MATCH_HOLD_REG(s32, byte, r1) = self->flags2;
        MATCH_HOLD_REG(s32, result, r0);

        result = mask | byte;
        self->flags2 = result;
    }
}

/* Same `InitMovingSprite`/table-swap/clearer shape as `CreateGroundSprite` above,
 * but re-initializes an existing `self` instead of allocating a new
 * one - the same relationship `InitMovingSprite` itself has to
 * `CreateGroundSprite`. */
struct actor *InitGroundSprite(struct actor *self)
{
    InitMovingSprite(self);
    self->table = (void *)gGroundSpriteVtable;
    ResetGroundSprite(self);
    return self;
}

/* `flags2` bit 1 get/set/clear accessors. */
u8 IsGroundSpriteGrounded(void *selfArg)
{
    struct gobj *self = selfArg;
    return (self->flags2 >> 1) & 1;
}

void ClearGroundSpriteGrounded(void *selfArg)
{
    struct gobj *self = selfArg;
    MATCH_HOLD_REG(s32, mask, r1) = -3;
    MATCH_HOLD_REG(s32, byte, r2) = self->flags2;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask & byte;
    self->flags2 = result;
}

void SetGroundSpriteGrounded(void *selfArg)
{
    struct gobj *self = selfArg;
    MATCH_HOLD_REG(s32, mask, r1) = 2;
    MATCH_HOLD_REG(s32, byte, r2) = self->flags2;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask | byte;
    self->flags2 = result;
}

/* `flags2` bit 0 get/set/clear accessors. */
u8 IsGroundSpriteFloorProbeEnabled(void *selfArg)
{
    MATCH_HOLD_REG(struct gobj *, self, r1);
    MATCH_HOLD_REG(s32, mask, r0) = 1;
    MATCH_HOLD_REG(s32, byte, r1);
    MATCH_HOLD_REG(s32, result, r0);

    self = selfArg;
    byte = self->flags2;
    result = mask & byte;
    return result;
}

void DisableGroundSpriteFloorProbe(void *selfArg)
{
    struct gobj *self = selfArg;
    MATCH_HOLD_REG(s32, mask, r1) = -2;
    MATCH_HOLD_REG(s32, byte, r2) = self->flags2;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask & byte;
    self->flags2 = result;
}

void EnableGroundSpriteFloorProbe(void *selfArg)
{
    struct gobj *self = selfArg;
    MATCH_HOLD_REG(s32, mask, r1) = 1;
    MATCH_HOLD_REG(s32, byte, r2) = self->flags2;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask | byte;
    self->flags2 = result;
}

/* `flags` bit 5 clear/set/get accessors. */
void ClearSpriteObjFlag5(void *selfArg)
{
    struct gobj *self = selfArg;
    MATCH_HOLD_REG(s32, mask, r1) = -0x21;
    MATCH_HOLD_REG(s32, byte, r2) = self->flags;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask & byte;
    self->flags = result;
}

void SetSpriteObjFlag5(void *selfArg)
{
    struct gobj *self = selfArg;
    MATCH_HOLD_REG(s32, mask, r1) = 0x20;
    MATCH_HOLD_REG(s32, byte, r2) = self->flags;
    MATCH_HOLD_REG(s32, result, r1);

    result = mask | byte;
    self->flags = result;
}

u8 GetSpriteObjFlag5(void *selfArg)
{
    struct gobj *self = selfArg;
    return (self->flags >> 5) & 1;
}

/* `ctrl` getter (the same "record" field `DestroyMovingSprite`/`UpdateMovingSprite`
 * fire their trampolines through). */
s32 GetMovingSpriteCtrl(void *selfArg)
{
    return (s32)((struct gfx_part *)selfArg)->ctrl;
}
