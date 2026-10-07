#include "action_ctrl.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "util.h"
#include "system.h"
#include "audio.h"
#include "level.h"
#include "globals.h"
}

/* ActionCtrl's ApplyMotion and its idle state (include/action_ctrl.hpp;
 * #664, docs/cplusplus.md). Built by old_agbcp (the Makefile's
 * OLD_AGBCC_OBJS), like the old_agbcc C it replaces. */

/* Applies the queued motion. First, while pushed (`pushLeft`/`pushRight`,
 * a conveyor) and standing (`hitAxes` 8), moves the player a pixel; in
 * idle, plays the idle animation once he stands still (unless a fidget
 * plays); in idle and crouching, stops a non-slippery slide. Then each
 * pending entry names a record of gCtrlMotionRecords through the entry
 * set's {X, Y} pairs, applied with the speed kept (SetTargetMotion*) or
 * started (StartTargetMotion*). On ice a standing, moving player keeps
 * his speed with a slower ramp (target * 1.5, step / 2); the slide's X
 * entry 0x1E starts it, faster on ice. */
void ActionCtrl::ApplyMotion()
{
    struct speed_ramp rec;
    struct player *p;

    p = gPlayer;
    if (p->pushLeft == 0) {
        if (p->pushRight == 0)
            goto skip;
    }
    if (part->hitAxes == 8) {
        if (p->pushLeft)
            p->x -= 0x100;
        else if (p->pushRight)
            p->x += 0x100;
        SetSpritePrevPos((struct gfx_part *)gPlayer, gPlayer->x, gPlayer->y);
    }
skip:
    if (state == ACTION_STATE_IDLE) {
        struct player *p = gPlayer;

        if (p->speedX == 0 && p->anim->animCount != 0x12 && idleFidget == 0) {
            StopSfx(gAudioContext, SFX_SKID);
            SetTargetAnim((SpriteObj *)gPlayer, 0x12);
        }
    }
    if (state == ACTION_STATE_IDLE || state == ACTION_STATE_CROUCH) {
        struct player *p = gPlayer;

        if (p->speedX != 0 && p->slippery == 0)
            QueueNowX(0);
    }
    {
        u8 pending = motionXPending;

        if (pending == 1) {
            struct player *q;

            rec = gCtrlMotionRecords[animSet->entries[motionX][0]];
            q = gPlayer;
            if (IsSlippery(q) && part->hitAxes == 8 && q->speedX != 0) {
                motionXKeepSpeed = pending;
                rec.target = FixedMul(rec.target, 0x180);
                rec.step /= 2;
            }
            if (motionX == 0x1E) {
                motionXKeepSpeed = 0;
                if (gPlayer->slippery) {
                    rec.step = FixedMul(rec.step, 0x200);
                    rec.start = FixedMul(rec.start, 0x180);
                }
            }
            if (motionXKeepSpeed)
                SetTargetMotionX(Sprite(), &rec.start);
            else
                StartTargetMotionX(Sprite(), &rec.start);
            motionXPending = 0;
        }
    }
    if (motionYPending == 1) {
        rec = gCtrlMotionRecords[animSet->entries[motionY][1]];
        if (motionYKeepSpeed)
            SetTargetMotionY(Sprite(), &rec);
        else
            StartTargetMotionY(Sprite(), &rec);
        motionYPending = 0;
    }
}

/* Idle: the D-pad lock counts down (released, it ends); the idle
 * animation resumes after a fidget; standing still (animation 0x12, on its
 * first frame) for 8, 20 or 30 seconds plays a fidget (0xE, 5, 0x1A).
 * Unless the player left the ground (CheckLeftGround): A jumps (state 5,
 * animation 0x13, Y entry 7), B spins (StartSpin), R crouches down
 * (animation 3); otherwise the D-pad: sideways runs (the turbo run with L
 * held and HasTurboRun), down crouches down, none slides to a stop on
 * ice (X entry 0x1F). Then the facing. */
void ActionCtrl::StateIdle()
{
    void *pad = gInput;
    u32 in = gKeys.all;
    u8 dir = GetDpadDirection(pad);
    s32 count;
    struct player *p;

    if (dpadLockTimer != 0) {
        dpadLockTimer--;
        if (dir == 0)
            dpadLockTimer = dir;
    }
    p = part;
    if (p->animDone) {
        SetTargetAnim((SpriteObj *)p, 0x12);
        idleFidget = 0;
    }
    count = ++frames;
    p = part;
    if (p->tag == 0x12 && p->frame == 0) {
        if (count > 0x708) {
            SetTargetAnim((SpriteObj *)p, 0x1A);
            frames = 0;
        } else if (count >= 0x49D && count <= 0x4C3) {
            SetTargetAnim((SpriteObj *)p, 5);
            frames = 0x4C4;
        } else if (count >= 0x1E1 && count <= 0x207) {
            SetTargetAnim((SpriteObj *)p, 0xE);
            frames = 0x208;
        } else {
            goto skip;
        }
        idleFidget = 1;
    }
skip:
    {
        u8 left = CheckLeftGround();
        u16 held;

        if (left)
            return;
        if (INPUT_PRESSED(in) & 1) {
            PlaySfx(gAudioContext, SFX_JUMP, 0x100);
            SetMode(ACTION_STATE_JUMP);
            SetTargetAnim(Sprite(), 0x13);
            frame = left;
            QueueNowY(7);
        } else {
            u16 alt = INPUT_PRESSED(in) & 2;

            if (alt) {
                StartSpin();
            } else {
                if ((held = INPUT_HELD(in) & R_BUTTON) == 0)
                    goto other;
                SetMode(ACTION_STATE_CROUCH_DOWN);
                SetTargetAnim(Sprite(), 3);
                frames = alt;
            }
        }
        UpdateFacing();
        return;
    other:
        turboRun = held;
        if (dir == 0) {
            struct player *q = part;

            if (IsSlippery(q) && motionX != 0x1F && q->speedX != 0)
                QueueNowXKeepSpeed(0x1F);
        } else {
            u8 wait = dpadLockTimer;

            if (wait == 0) {
                switch (dir) {
                case 3 ... 8:
                    if ((INPUT_HELD(in) & L_BUTTON) && (u8)HasTurboRun(gLevelState)) {
                        turboRun = 1;
                        SetMode(ACTION_STATE_TURBO_RUN);
                        SetTargetAnim(Sprite(), 0x18);
                        QueueX(wait, 1, 0x1B);
                    } else {
                        StartRun();
                    }
                    break;
                case 2:
                    SetMode(ACTION_STATE_CROUCH_DOWN);
                    SetTargetAnim(Sprite(), 3);
                    frames = wait;
                    break;
                }
            }
        }
        UpdateFacing();
    }
}
