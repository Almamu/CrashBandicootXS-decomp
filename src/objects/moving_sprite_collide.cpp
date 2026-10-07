#include "sprite_obj.hpp"
#include "ctrl.hpp"

extern "C" {
#include "util.h"
}

/* MovingSprite's events, contact test and accessors (#664,
 * include/sprite_obj.hpp). */

/* An event goes to the controller. */
void MovingSprite::HandleEvent(s32 from, s32 event, s32 arg)
{
    if (mover != 0)
        mover->HandleEvent((SpriteObj *)from, event, arg);
}

/* How the sprite touches `region`: 2 when its attack box does, or when
 * its body box does and it isn't vulnerable; 1 when only its body box
 * does and it is (the player hits it); 0 when neither does. */
s32 MovingSprite::ClassifyContact(struct aabb *region)
{
    struct aabb box = GetAttackBox();
    s32 result;

    if ((u8)AabbOverlaps(&box, region) != 0) {
        result = 2;
    } else {
        box = GetBodyBox();
        result = (u8)AabbOverlaps(&box, region);
        if (result != 0) {
            s32 vulnerable = (f.flags >> 6) & 1;

            if (!vulnerable)
                result = 2;
        }
    }
    return result;
}

/* Slot 1: the contact with the player (TouchPlayer); never a pickup. */
s32 MovingSprite::CheckPlayerContact()
{
    TouchPlayer();
    return 0;
}

s32 MovingSprite::GetHitMask()
{
    return hitMask;
}

s32 MovingSprite::HasHitMask()
{
    return (u32)(-hitMask | hitMask) >> 31;
}

void MovingSprite::ClearHitMask()
{
    hitMask = 0;
}

void MovingSprite::AddHitMask(s32 mask)
{
    hitMask |= mask;
}

void MovingSprite::SetHitAxes(u8 value)
{
    hitAxes = value;
}

u8 MovingSprite::GetHitAxes()
{
    return hitAxes;
}

void MovingSprite::SetSpeedY(s32 value)
{
    speedY = value;
}

void MovingSprite::SetSpeedX(s32 value)
{
    speedX = value;
}

s32 MovingSprite::GetSpeedX()
{
    return speedX;
}

s32 MovingSprite::GetSpeedY()
{
    return speedY;
}

Ctrl *MovingSprite::GetCtrl()
{
    return mover;
}

void MovingSprite::AttachCtrl(Ctrl *ctrl)
{
    mover = ctrl;
    ctrl->Attach((SpriteObj *)this);
}

void MovingSprite::StartMotionY(s32 speed, s32 step, s32 target)
{
    speedY = speed;
    rampY.start = speed;
    rampY.step = step;
    rampY.target = target;
}

void MovingSprite::SetMotionY(s32 start, s32 step, s32 target)
{
    rampY.start = start;
    rampY.step = step;
    rampY.target = target;
}

void MovingSprite::StartMotionX(s32 speed, s32 step, s32 target)
{
    speedX = speed;
    rampX.start = speed;
    rampX.step = step;
    rampX.target = target;
}

void MovingSprite::SetMotionX(s32 start, s32 step, s32 target)
{
    rampX.start = start;
    rampX.step = step;
    rampX.target = target;
}

u8 MovingSprite::GetProbeTries()
{
    return probeTries;
}
