#include "core.h"
#include "match.h"
#include "gobj_1a794.h"
#include "objects.h"

/* Same `record->table+0x10/0x14`-driven trampoline shape as
 * `DestroyMovingSprite`/`UpdateMovingSprite`, but forwarding `arg1`/`arg2`/`arg3`
 * straight through as `_call_via_r4`'s own arg1-arg3 instead of
 * building them locally. The `table+0x14` function pointer read is a
 * genuine "dead read" - loaded into `r4` but never actually passed to
 * `_call_via_r4` (a plain 4-argument function, not itself a trampoline)
 * - the same idiom already confirmed and documented for
 * `CheckSpritePickup`'s own `_call_via_r4` call in `sprite.cpp`. */
void HitMovingSprite(struct gobj *self, s32 arg1, s32 arg2, s32 arg3)
{
    struct mover *rec = self->mover;

    if (rec != 0) {
        struct mover_vtable *tbl = rec->vtable;
        void *addr = (u8 *)rec + tbl->m10.thisOffset;
        MATCH_HOLD_REG(void *, deadRead, r4) = *(void *volatile *)&tbl->m10.fn;
        (void)deadRead;

        _call_via_r4(addr, arg1, arg2, arg3);
    }
}

/* Builds `part`'s primary AABB (`GetSpriteAttackBox`) and tests it against
 * `region` (`AabbOverlaps`, the same collision-test function used by
 * `CheckSpritePickup`/`IsSpriteObjInsideRect`'s sibling); if that already overlaps,
 * returns 2. Otherwise builds the secondary AABB (`GetSpriteBodyBox`) and
 * re-tests; if that misses, returns 0. If it hits, returns 2 unless
 * `part->flags` bit 6 is set, in which case it returns the (nonzero)
 * hit-test result itself. Needed the flags byte loaded into `part`'s
 * own dying register (`r5`, matching the ROM's `ldrb r5, [r5, #0xc]`
 * self-overwrite - `part` is never used again afterward) and read
 * through a `u32` (not `s32`) intermediate so the `>> 6` compiles to
 * a logical `lsr` instead of an arithmetic `asr`. */
s32 ClassifySpriteContact(void *part, void *region)
{
    struct aabb box;
    s32 result;

    GetSpriteAttackBox(&box, part);
    if ((u8)AabbOverlaps(&box, region) != 0) {
        goto returnTwo;
    }

    {
        struct aabb box2;

        GetSpriteBodyBox(&box2, part);
        box = box2;

        result = (u8)AabbOverlaps(&box, region);
        if (result == 0) {
            goto end;
        }
        {
            MATCH_HOLD_REG(u32, flags, r5) = ((struct gobj *)part)->flags;
            MATCH_HOLD_REG(u32, shifted, r0) = flags >> 6;
            MATCH_HOLD_REG(u32, test, r0);
            MATCH_HOLD_REG(u32, mask, r1) = 1;

            test = shifted & mask;
            if (test != 0) {
                goto end;
            }
        }
    }
returnTwo:
    result = 2;
end:
    return result;
}

/* Calls the object's `checkContact` method (CheckPlayerContact) via
 * `_call_via_r1` and always returns 0. Needed the trampoline's `addr = self
 * + offset` computed before the `fn` load (reusing the adjusted table
 * pointer's own dying register for `fn`), the same accumulator-style
 * fix established for `DestroyMovingSprite`/`UpdateMovingSprite` above. */
s32 CollideMovingSprite(struct gobj *self)
{
    MATCH_HOLD_REG(struct actor_method *, m, r1) = &self->vtable->checkContact;
    MATCH_HOLD_REG(s32, offset, r2) = m->thisOffset;
    MATCH_HOLD_REG(void *, addr, r0);
    MATCH_HOLD_REG(void *, fn, r1);

    addr = (u8 *)self + offset;
    fn = m->fn;
    _call_via_r1(addr, fn);
    return 0;
}

/* `hitMask` get/clear/OR-set accessors. */
s32 GetGroundSpriteHitMask(struct gobj *self)
{
    return self->hitMask;
}

/* `hitMask != 0`, via the branchless `(-x | x) >> 31` idiom rather
 * than a plain comparison. */
s32 HasGroundSpriteHitMask(struct gobj *self)
{
    s32 val = self->hitMask;
    return (u32)(-val | val) >> 31;
}

void ClearGroundSpriteHitMask(struct gobj *self)
{
    self->hitMask = 0;
}

void AddGroundSpriteHitMask(struct gobj *self, s32 val)
{
    self->hitMask |= val;
}

/* `hitAxes` get/set pair. */
void SetGroundSpriteHitAxes(struct gobj *self, u8 val)
{
    self->hitAxes = val;
}

u8 GetGroundSpriteHitAxes(struct gobj *self)
{
    return self->hitAxes;
}

/* `speedY`/`speedX` setters. */
void SetSpriteSpeedY(struct gobj *self, s32 val)
{
    self->speedY = val;
}

void SetSpriteSpeedX(struct gobj *self, s32 val)
{
    self->speedX = val;
}

/* `speedX`/`speedY` getters - the setters' siblings above. */
s32 GetSpriteSpeedX(struct gobj *self)
{
    return self->speedX;
}

s32 GetSpriteSpeedY(struct gobj *self)
{
    return self->speedY;
}

/* `mover` (the controller `DestroyMovingSprite`/`UpdateMovingSprite`/
 * `AttachSpriteCtrl` use) getter. */
struct mover *GetSpriteCtrl(struct gobj *self)
{
    return self->mover;
}

/* Sets `mover` to `rec`, then calls `rec`'s `m18` method (AttachCtrl in
 * gCtrlVtable) via `_call_via_r2` with `self` as the second argument.
 * Same `addr`-before-`fn` fix as `DestroyMovingSprite`/`UpdateMovingSprite`. */
void AttachSpriteCtrl(struct gobj *self, struct mover *rec)
{
    self->mover = rec;

    {
        MATCH_HOLD_REG(struct mover_vtable *, tbl, r2) = rec->vtable;
        MATCH_HOLD_REG(s32, offset, r1) = tbl->m18.thisOffset;
        MATCH_HOLD_REG(void *, addr, r0);
        MATCH_HOLD_REG(void *, fn, r2);

        addr = (u8 *)rec + offset;
        fn = tbl->m18.fn;
        _call_via_r2(addr, self, fn);
    }
}

/* `speedY` + `rampY` bulk setter - `speedY` and `rampY.start` both get
 * the same first argument. */
void StartSpriteMotionY(struct gobj *self, s32 a, s32 b, s32 c)
{
    self->speedY = a;
    self->rampY.start = a;
    self->rampY.step = b;
    self->rampY.target = c;
}

/* Same shape as `StartSpriteMotionY` above, without the `speedY` write. */
void SetSpriteMotionY(struct gobj *self, s32 a, s32 b, s32 c)
{
    self->rampY.start = a;
    self->rampY.step = b;
    self->rampY.target = c;
}

/* `speedX` + `rampX` bulk setter - the speed and ramp
 * `ApplySpriteVelocity` steps, same "shared first write" shape as
 * `StartSpriteMotionY`. */
void StartSpriteMotionX(struct gobj *self, s32 a, s32 b, s32 c)
{
    self->speedX = a;
    self->rampX.start = a;
    self->rampX.step = b;
    self->rampX.target = c;
}

/* Same shape as `StartSpriteMotionX` above, without the `speedX` write. */
void SetSpriteMotionX(struct gobj *self, s32 a, s32 b, s32 c)
{
    self->rampX.start = a;
    self->rampX.step = b;
    self->rampX.target = c;
}

/* `probeTries` (cleared by `ResetMovingSprite`) getter. */
u8 GetGroundSpriteProbeTries(struct gobj *self)
{
    return self->probeTries;
}
