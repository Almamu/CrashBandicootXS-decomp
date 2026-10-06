#include "core.h"
#include "match.h"
#include "actor.h"
#include "vtable.h"
#include "util.h"
#include "objects.h"
#include "memory.h"
#include "gobj_1a794.h"

/* Looks up the `index`-th {a, b} pair of the controller's entry set
 * (`animSet`), uses its `b` as an index into gCtrlMotionRecords, and calls
 * slot 6 of the controller's method table (StartCtrlTargetMotionY) with that
 * record - the same base+offset+fn-pointer convention already seen in
 * `DestroyPlayer`/`ResolvePlayerContact`. */
void StartCtrlTargetMotionYFromSet(void *selfArg, void *arg1, s32 index)
{
    struct ctrl *self = selfArg;
    const struct entry_set *set = self->animSet;
    MATCH_HOLD_REG(const u32 *, arr, r3) = *set->entries;
    MATCH_HOLD_REG(s32, recOffset, r2) = index * 8;
    const u32 *rec;
    s32 type;
    const struct speed_ramp *tableEntry;
    const struct vtable_slot *vtbl;
    s16 offset;
    void *addr;
    void *fn;

    asm("add %0, %0, %1" : "+r"(recOffset) : "r"(arr));
    rec = (const u32 *)recOffset;
    type = rec[1];
    tableEntry = &gCtrlMotionRecords[type];
    vtbl = self->vtable;
    offset = vtbl[6].delta;
    addr = (u8 *)self + offset;
    fn = vtbl[6].fn;

    _call_via_r3(addr, arg1, (s32)tableEntry, fn);
}

/* Scales `vec` by the entry set's `scale` and writes it into `part`'s X
 * speed ramp (`rampX`: start, step, target), negating the start and the
 * target when `part` is X-mirrored (`mirror` bit 4). */
void SetCtrlTargetMotionX(void *selfArg, void *partArg, s32 *vec)
{
    struct ctrl *self = selfArg;
    struct gobj *part = partArg;

    if ((s32)(part->mirror << 27) < 0) {
        s32 x = -FixedMul(vec[0], self->animSet->scale);
        s32 y = FixedMul(vec[1], self->animSet->scale);
        s32 z = -FixedMul(vec[2], self->animSet->scale);

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    } else {
        s32 x = FixedMul(vec[0], self->animSet->scale);
        s32 y = FixedMul(vec[1], self->animSet->scale);
        s32 z = FixedMul(vec[2], self->animSet->scale);

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
}

/* Same scaled ramp write as `SetCtrlTargetMotionX`, also loading the
 * (possibly negated) start into `speedX` - the scaled-copy counterpart
 * of `StartCtrlTargetMotionY`'s plain-copy `speedY` load. */
void StartCtrlTargetMotionX(void *selfArg, void *partArg, s32 *vec)
{
    struct ctrl *self = selfArg;
    struct gobj *part = partArg;

    if ((s32)(part->mirror << 27) < 0) {
        s32 x = -FixedMul(vec[0], self->animSet->scale);
        s32 y = FixedMul(vec[1], self->animSet->scale);
        s32 z = -FixedMul(vec[2], self->animSet->scale);

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    } else {
        s32 x = FixedMul(vec[0], self->animSet->scale);
        s32 y = FixedMul(vec[1], self->animSet->scale);
        s32 z = FixedMul(vec[2], self->animSet->scale);

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
}

/* Same shape as `StartCtrlTargetMotionYFromSet`, reading the pair's `a`
 * as the record index instead of its `b`, and method table slot 5
 * (StartCtrlTargetMotionX) instead of slot 6. */
void StartCtrlTargetMotionXFromSet(void *selfArg, void *arg1, s32 index)
{
    struct ctrl *self = selfArg;
    const struct entry_set *set = self->animSet;
    MATCH_HOLD_REG(const u32 *, arr, r3) = *set->entries;
    MATCH_HOLD_REG(s32, recOffset, r2) = index * 8;
    const u32 *rec;
    s32 type;
    const struct speed_ramp *tableEntry;
    const struct vtable_slot *vtbl;
    s16 offset;
    void *addr;
    void *fn;

    asm("add %0, %0, %1" : "+r"(recOffset) : "r"(arr));
    rec = (const u32 *)recOffset;
    type = rec[0];
    tableEntry = &gCtrlMotionRecords[type];
    vtbl = self->vtable;
    offset = vtbl[5].delta;
    addr = (u8 *)self + offset;
    fn = vtbl[5].fn;

    _call_via_r3(addr, arg1, (s32)tableEntry, fn);
}

/* gCtrlVtable slot 2, the base controller's event handler: empty
 * (ActionCtrlHandleEvent, PlayerCtrlHandleEvent and HitEnemy override it). */
void CtrlHandleEvent(void)
{
}
asm(".align 2, 0");

/* Sets `part`'s animation (`tag`) to `newVal`, but only if it actually
 * changed - otherwise a no-op returning 0. On a real change, resets the
 * sub-counter/frame-counter/"done" flag exactly like `SetSpriteAnim` (not
 * called directly here - inlined instead), clears `flags` bit 3, and
 * returns 1. */
u8 SetCtrlTargetAnim(void *unused, void *partArg, s32 newVal)
{
    struct gobj *part = partArg;
    u8 result = 0;

    if (part->tag != newVal) {
        part->tag = newVal;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        {
            MATCH_HOLD_REG(s32, mask, r0) = -9;
            MATCH_HOLD_REG(s32, byte, r1) = part->flags;
            MATCH_HOLD_REG(s32, masked, r0);

            masked = mask & byte;
            part->flags = masked;
        }
        result = 1;
    }
    return result;
}

/* `self+0` word setter. */
void AttachCtrl(void *selfArg, s32 val)
{
    *(s32 *)selfArg = val;
}

/* Resets the method table to `gCtrlVtable`, then (if bit 0 of `flags`
 * is set) fires `OperatorDelete` on `self`. */
void DestroyCtrl(void *selfArg, s32 flags)
{
    struct ctrl *self = selfArg;

    self->vtable = gCtrlVtable;
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Resets the method table to `gCtrlVtable` and clears `state`. */
void InitCtrl(void *selfArg)
{
    struct ctrl *self = selfArg;

    self->vtable = gCtrlVtable;
    self->state = 0;
}

/* `state` getter. */
s32 GetCtrlMode(void *selfArg)
{
    return ((struct ctrl *)selfArg)->state;
}
