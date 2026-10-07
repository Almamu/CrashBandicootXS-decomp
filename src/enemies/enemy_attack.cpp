#include "enemy_ctrl.hpp"
#include "spawners.hpp"
#include "player.hpp"

extern "C" {
#include "match.h"
#include "util.h"
#include <libgcc.h>
#include "audio.h"
#include "player.h"
#include "objects.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
}

/* GitHub issue #9/#10: EnemyCtrl's attack updaters and state setter
 * (include/enemy_ctrl.hpp), from the 0x0800B8DC-0x0800D040 cluster
 * (docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md). The whole
 * file is old_agbcc's code (issue #10 NAKED retry,
 * docs/matching/archive/issue-10-naked-retry.md).
 *
 * UpdateAttackCycle (Update's states 4, 13, 14, 16 and 18) runs the
 * enemy's attack cycle off gRoomFrameCount: every idleTime + attackTime
 * frames, offset by cycleOffset, an idle enemy (mode 0) starts its
 * attack (animation mode 3, or 4 when the table maps mode 3 to
 * animation 8), and attackTime frames later an attacking one (mode 4)
 * ends it (mode 5, or straight back to 0 likewise). Penguins also stop
 * and restart their walk; crushers are solid only while slammed down,
 * and play the slam sound; the flamethrower lab assistant spawns its
 * flame at keyframe 9.
 * `__modsi3` is a remainder (`a % b`).
 *
 * UpdateTriggerBox (states 3 and 17) waits in mode 0 for the player to
 * touch its trigger box (boxL..boxB around the target, mirrored with
 * it), then plays animation mode 2; a vulture also swoops (a fixed
 * speed ramp), and is never let above baseY. Mode 2 goes back to 0 once
 * the animation is done.
 *
 * The spawn is an inline copy of LaunchHarmfulEffectPart (enemy_ctrl.cpp):
 * passing the arguments through inline parameters is what materializes
 * them in the ROM's order, and the `+0xC` flag writes are bitfield
 * stores (QImode `-0x41`/`-9` masks). */

/* LaunchHarmfulEffectPart (enemy_ctrl.cpp), inlined. */
static inline MovingSprite *SpawnPart(s32 a, s32 b, s32 c, s32 d, s32 e, MovingSprite *f)
{
    MovingSprite *obj = gEntitySpawner->LaunchEffectPart(a, b, c, d, e, f);

    obj->f.b.visible = 1;
    obj->f.b.vulnerable = 0;
    return obj;
}

void EnemyCtrl::UpdateAttackCycle()
{
    switch (mode) {
    case 0:
        if (attackTime > 0 &&
            __modsi3(gRoomFrameCount + (idleTime + attackTime) * 2 - cycleOffset - idleTime,
                     idleTime + attackTime) == 0) {
            if (anims[3] != 8)
                SetAnimMode(3);
            else
                SetAnimMode(4);
            if (kind == ENEMY_KIND_PENGUIN) {
                SetMotionX(0);
            } else if (kind == ENEMY_KIND_WOODEN_CRUSHER || kind == ENEMY_KIND_PISTON_CRUSHER) {
                target->solid = 0;
            }
        }
        break;
    case 4:
        if (idleTime > 0 && __modsi3(gRoomFrameCount + idleTime + attackTime - cycleOffset,
                                     idleTime + attackTime) == 0) {
            if (anims[5] != 8)
                SetAnimMode(5);
            else
                SetAnimMode(0);
        }
        break;
    case 3:
        if (target->animDone) {
            SetAnimMode(4);
            if (kind == ENEMY_KIND_WOODEN_CRUSHER || kind == ENEMY_KIND_PISTON_CRUSHER) {
                target->solid = 1;
                PlaySfx(gAudioContext, SFX_CRUSHER_SLAM, 0x100);
            } else if (kind == ENEMY_KIND_PENGUIN) {
                PlaySfx(gAudioContext, SFX_UNKNOWN_09, 0x100);
            }
        }
        if (kind == ENEMY_KIND_FLAMETHROWER_LAB_ASSISTANT && target->tick == 9 &&
            target->timer == 0) {
            SpawnPart(0x17, 4, -0x2d, 2, 0, sprite)->kind = 2;
            PlaySfx(gAudioContext, SFX_FLAMETHROWER, 0x100);
        }
        break;
    case 5:
        if (target->animDone) {
            SetAnimMode(0);
            if (kind != ENEMY_KIND_PENGUIN)
                break;
            SetMotionX(1);
        }
        if (kind == ENEMY_KIND_PENGUIN && target->tick == 8 && target->timer == 0)
            PlaySfx(gAudioContext, SFX_UNKNOWN_23, 0x100);
        break;
    }
}

void EnemyCtrl::UpdateTriggerBox()
{
    s32 m;
    struct aabb box;
    s32 x, y, w, h;

    if (kind == ENEMY_KIND_VULTURE) {
        /* r1 pin: the allocator otherwise swaps target/baseY (r2/r1),
         * under C++ as under C. */
        MATCH_HOLD_REG(struct ctrl_target *, t, r1) = target;
        if (t->y < baseY) {
            t->y = baseY;
            SetMotionY(0);
        }
    }
    switch (m = mode) {
    case 0:
        x = Q8_TO_INT(target->x);
        y = Q8_TO_INT(target->y);
        w = boxR - boxL;
        h = boxB - boxT;
        SetAabbPos(&box, x + boxL, y + boxT);
        SetAabbSize(&box, w, h);
        /* X-mirrored: bit 4 as a sign test (`lsl #27`); g++ tests the
         * bitfield with an `and`. */
        if ((s32)(sprite->mirror << 27) < 0)
            box.x = Q8_TO_INT(target->x) * 2 - (box.x + box.w);
        if (gPlayer->TouchesBox(&box)) {
            SetAnimMode(2);
            if (kind == ENEMY_KIND_VULTURE) {
                struct ctrl_target *part = target;
                s32 a = 0x300, b = 0x20, c;

                part->speedY = a;
                part->rampY[0] = a;
                part->rampY[1] = b;
                part->rampY[2] = m;
                c = -0x200;
                part->speedX = m;
                part->rampX[0] = m;
                part->rampX[1] = b;
                part->rampX[2] = c;
            }
        }
        break;
    case 2:
        if (target->animDone)
            SetAnimMode(0);
        break;
    }
}

/* SetState's inline copies of SetMotionY, SetMotionX and SetAnimMode
 * (enemy_ctrl.cpp): its `bl`s go to Ctrl::StartTargetMotion*FromSet
 * directly, never to those three, and SetTargetAnim is called through
 * the vtable in place. In SetState (old_agbcc, issue #10 NAKED retry):
 *  - The groups the ROM keeps apart ({1,3,17} vs {6,9,10,11}) are
 *    separate cases; reload picks a different scratch register for the
 *    trigger's `ldrsh` in each, so cross-jumping can't merge them.
 *  - That scratch register is picked round-robin in insn order, so the
 *    {4,14,16} keyframe tail has to be written out in both branches
 *    (cross-jumping then merges the copies): with a single shared tail
 *    the else branch's `ldrsh` gets r4 instead of the ROM's r1.
 *  - State 5's velocity stores go through inline setters, which puts
 *    the shared 0 in r2 before the first store. */

static inline void SetMotionYInline(EnemyCtrl *self, s32 mode)
{
    self->modeA = mode;
    self->Ctrl::StartTargetMotionYFromSet(self->sprite, mode);
}

static inline void SetMotionXInline(EnemyCtrl *self, s32 mode)
{
    self->modeB = mode;
    self->Ctrl::StartTargetMotionXFromSet(self->sprite, mode);
}

static inline void SetAnimModeInline(EnemyCtrl *self, s32 mode)
{
    self->mode = mode;
    self->SetTargetAnim(self->sprite, self->anims[mode]);
}

static inline void SetVelX(struct ctrl_target *t, s32 v, s32 w)
{
    t->speedX = v;
    t->rampX[0] = v;
    t->rampX[1] = w;
    t->rampX[2] = v;
}

static inline void SetVelY(struct ctrl_target *t, s32 v, s32 w)
{
    t->speedY = v;
    t->rampY[0] = v;
    t->rampY[1] = w;
    t->rampY[2] = v;
}

/* Sets Update's `state` (1-18) and starts it: the motion and animation
 * modes it begins with (state 5 falls: a fixed speed ramp), then latches
 * the target's position into baseX/baseY. The level spawners call it
 * once the enemy is set up. */
void EnemyCtrl::SetState(s32 newState)
{
    state = newState;
    switch (newState) {
    case 5:
        {
            struct ctrl_target *part = target;

            SetVelX(part, -0x180, 0);
            SetVelY(part, 0x400, 0);
            part->flag7 = 1;
        }
        break;
    case 6:
    case 9:
    case 10:
    case 11:
        SetAnimModeInline(this, 0);
        break;
    case 2:
    case 15:
        SetMotionXInline(this, 1);
        SetAnimModeInline(this, 0);
        break;
    case 13:
    case 18:
        SetMotionXInline(this, 1);
    case 4:
    case 14:
    case 16:
        if (cycleOffset >= idleTime) {
            SetAnimModeInline(this, 4);
            {
                struct ctrl_target *part = target;
                part->tick = (*part->keyframes)[part->frame].steps - 1;
            }
        } else {
            SetAnimModeInline(this, 0);
            if (kind == ENEMY_KIND_STATIONARY_SPACE_ENEMY) {
                struct ctrl_target *part = target;
                part->tick = (*part->keyframes)[part->frame].steps - 1;
            }
        }
        break;
    case 1:
    case 3:
    case 17:
        SetAnimModeInline(this, 0);
        break;
    case 8:
        SetMotionXInline(this, 3);
        SetMotionYInline(this, 3);
        SetAnimModeInline(this, 0);
        break;
    case 7:
        SetMotionXInline(this, 2);
        SetAnimModeInline(this, 0);
        counter = 0;
        break;
    case 12:
        break;
    }
    baseX = target->x;
    baseY = target->y;
}

/* Sets the X homing bounds to the target's x +/- `radius` (Q8) and
 * caches the homing speed pair. */
void EnemyCtrl::SetRangeXSpeed(s32 radius, s32 newSpeed, s32 newAccel)
{
    s32 x = target->x;
    rangeX[0] = x - INT_TO_Q8(radius);
    rangeX[1] = target->x + INT_TO_Q8(radius);
    accel = newAccel;
    speed = newSpeed;
}

/* The Y-axis version of SetRangeXSpeed. */
void EnemyCtrl::SetRangeYSpeed(s32 radius, s32 newSpeed, s32 newAccel)
{
    s32 y = target->y;
    rangeY[1] = y - INT_TO_Q8(radius);
    rangeY[0] = target->y + INT_TO_Q8(radius);
    accel = newAccel;
    speed = newSpeed;
}

/* SetRangeXSpeed's bounds without the speed pair. */
void EnemyCtrl::SetRangeX(s32 radius)
{
    s32 x = target->x;
    rangeX[0] = x - INT_TO_Q8(radius);
    rangeX[1] = target->x + INT_TO_Q8(radius);
}
