#include "ctrl.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "util.h"
}

/* The controllers' base class, Ctrl (gCtrlVtable's own methods), ROM
 * 0x0800C9A4-0x0800CBF4 (#664, docs/cplusplus.md). It's old_agbcp's: the
 * file is in the Makefile's OLD_AGBCC_OBJS (SetTargetAnim's `movs r0, #9;
 * negs r0, r0` before the `ldrb` is old_agbcc's tell). As C, built with
 * agbcc, it needed 7 MATCH_* pins and 2 asm statements; as C++ it needs
 * none. */

/* Looks up the `index`-th {X, Y} pair of the controller's entry set and
 * starts the Y record it names (virtual: a subclass can override
 * StartTargetMotionY). The record is computed before the call: a virtual
 * call evaluates `this` and the slot first, the arguments after. */
void Ctrl::StartTargetMotionYFromSet(MovingSprite *part, s32 index)
{
    const speed_ramp *rec = &gCtrlMotionRecords[animSet->entries[index][1]];

    StartTargetMotionY(part, rec);
}

/* Scales `vec` by the entry set's `scale` and writes it into `part`'s X
 * speed ramp (`rampX`: start, step, target), negating the start and the
 * target when `part` is X-mirrored (`mirror` bit 4). */
void Ctrl::SetTargetMotionX(MovingSprite *part, const s32 *vec)
{
    if ((s32)(part->mirror << 27) < 0) {
        s32 x = -FixedMul(vec[0], animSet->scale);
        s32 y = FixedMul(vec[1], animSet->scale);
        s32 z = -FixedMul(vec[2], animSet->scale);

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    } else {
        s32 x = FixedMul(vec[0], animSet->scale);
        s32 y = FixedMul(vec[1], animSet->scale);
        s32 z = FixedMul(vec[2], animSet->scale);

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
}

/* Same scaled ramp write as SetTargetMotionX, also loading the (possibly
 * negated) start into `speedX`: the scaled-copy counterpart of
 * StartTargetMotionY's plain-copy `speedY` load. */
void Ctrl::StartTargetMotionX(MovingSprite *part, const s32 *vec)
{
    if ((s32)(part->mirror << 27) < 0) {
        s32 x = -FixedMul(vec[0], animSet->scale);
        s32 y = FixedMul(vec[1], animSet->scale);
        s32 z = -FixedMul(vec[2], animSet->scale);

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    } else {
        s32 x = FixedMul(vec[0], animSet->scale);
        s32 y = FixedMul(vec[1], animSet->scale);
        s32 z = FixedMul(vec[2], animSet->scale);

        part->speedX = x;
        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
}

/* Same as StartTargetMotionYFromSet, with the pair's X record and
 * StartTargetMotionX. */
void Ctrl::StartTargetMotionXFromSet(MovingSprite *part, s32 index)
{
    const speed_ramp *rec = &gCtrlMotionRecords[animSet->entries[index][0]];

    StartTargetMotionX(part, &rec->start);
}

/* The base controller ignores events (ActionCtrlHandleEvent,
 * PlayerCtrlHandleEvent and HitEnemy override it). */
void Ctrl::HandleEvent(MovingSprite *, s32, s32)
{
}

/* Sets `part`'s animation (`tag`) to `anim`, but only if it actually
 * changed - otherwise a no-op returning 0. On a real change, resets the
 * frame timer, the frame and the "done" flag like SetSpriteAnim (inlined
 * here, not called), clears `flags` bit 3, and returns 1. The result is
 * a full int: ActionCtrl's override passes it through without the
 * zero-extension a `u8` return would add (the code here is the same). */
s32 Ctrl::SetTargetAnim(MovingSprite *part, s32 anim)
{
    s32 result = 0;

    if (part->tag != anim) {
        part->tag = anim;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->f.b.bit3 = 0;
        result = 1;
    }
    return result;
}

/* Attaches the controller to sprite object `owner`. AttachSpriteCtrl
 * calls it through the vtable. */
void Ctrl::Attach(MovingSprite *owner)
{
    this->owner = owner;
}

/* g++ sets the vtable pointer back to gCtrlVtable and, the root class's
 * destructor, frees the object when bit 0 of `__in_chrg` is set (a
 * `delete`). */
Ctrl::~Ctrl()
{
}

/* The vtable pointer, then the body. */
Ctrl::Ctrl()
{
    state = 0;
}

/* `state` getter. */
s32 Ctrl::GetMode()
{
    return state;
}
