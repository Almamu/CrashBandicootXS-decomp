#include "enemy_ctrl.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "globals.h"
#include "player.h"
}

/* EnemyCtrl's motion updaters (include/enemy_ctrl.hpp), from the
 * 0x0800B8DC-0x0800D040 cluster
 * (docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md).
 *
 * UpdateHomingX/UpdateHomingY home the target in on the player along
 * one axis: more than 20 (Q8) away, it heads for the player at `speed`
 * (ramping by `accel`) until it reaches its homing bound (rangeX/rangeY),
 * where it ramps to a stop; within 20 it stops. Each writes the axis's
 * {start, step, target} speed ramp.
 *
 * Each of the five cases does its own `{a, b, a}` store triple through
 * a block-scoped `a`/`b` pair (SET_VEL below); the compiler's cross-jump
 * then merges four of them into the shared tail and leaves the `0, 0x10`
 * moves duplicated, exactly as in the ROM (issue #10 NAKED retry,
 * docs/matching/archive/issue-10-naked-retry.md). The `-speed` case keeps
 * its own stores because its registers differ. Same bytes under agbcp
 * and old_agbcp. */

#define SET_VEL(v, a_, b_) \
    {                      \
        s32 a = (a_);      \
        s32 b = (b_);      \
        (v).start = a;     \
        (v).step = b;      \
        (v).target = a;    \
    }

void EnemyCtrl::UpdateHomingX()
{
    MovingSprite *part = target;
    s32 x = part->x;
    s32 d = x - gPlayer->x;

    if (d > 20) {
        if (x < rangeX[0])
            SET_VEL(part->rampX, 0, 0x10)
        else
            SET_VEL(part->rampX, -speed, accel)
    } else if (d < -20) {
        if (x > rangeX[1])
            SET_VEL(part->rampX, 0, 0x10)
        else
            SET_VEL(part->rampX, speed, accel)
    } else {
        SET_VEL(part->rampX, 0, accel)
    }
}

/* The Y-axis version of UpdateHomingX: `target->y`, `rampY`, and the
 * `rangeY` bounds tested in the opposite order. */
void EnemyCtrl::UpdateHomingY()
{
    MovingSprite *part = target;
    s32 y = part->y;
    s32 d = y - gPlayer->y;

    if (d > 20) {
        if (y < rangeY[1])
            SET_VEL(part->rampY, 0, 0x10)
        else
            SET_VEL(part->rampY, -speed, accel)
    } else if (d < -20) {
        if (y > rangeY[0])
            SET_VEL(part->rampY, 0, 0x10)
        else
            SET_VEL(part->rampY, speed, accel)
    } else {
        SET_VEL(part->rampY, 0, accel)
    }
}

/* Update's state 8: the hopping enemy. Nothing happens until the target
 * has come back down to baseY; then it restarts both motions (mode 0),
 * is put back at baseY, and:
 * - once the animation is done, in mode 0 it plays animation mode 1, in
 *   mode 1 it turns around (the X mirror bit) and plays mode 0;
 * - otherwise, at keyframe 8 (tick 8, timer 0), it hops: Y motion 3,
 *   X motion 3 (mode 0) or 0 (mode 1), and the hop sound.
 *
 * The target is put back at baseY through Entity::SetPos with its x
 * unchanged, as EnemyCtrl's oscillators and UpdateTriggerBox's vulture
 * clamp do (enemy_ctrl.cpp, #662 round 5). The x is a pseudo in r0
 * between the baseY load and the store, which is what the ROM's r2/r1
 * split needs (#662 round 4: local-alloc gives the baseY copy the
 * lowest register free over its life); after reload, reload_cse_regs
 * deletes the x store as a no-op and flow2 its load. That is the
 * self-assignment the permuter found on a C port in round 4; with a
 * plain `t->y = baseY` the store takes r0/r1 (an r1 pin until round
 * 4). */
void EnemyCtrl::UpdateHop()
{
    MovingSprite *t;

    if (target->y < baseY)
        return;
    SetMotionX(0);
    SetMotionY(0);
    t = target;
    t->SetPos(t->x, baseY);
    if (t->animDone) {
        switch (mode) {
        case 0:
            SetAnimMode(1);
            break;
        case 1:
            {
                u32 m = t->mirrorFlags.mirrorX;
                t->mirrorFlags.mirrorX = !m;
            }
            SetAnimMode(0);
            break;
        }
    } else if (t->frame == 8 && t->stepTimer == 0) {
        switch (mode) {
        case 0:
            SetMotionX(3);
            SetMotionY(3);
            gAudioContext->PlaySfx(SFX_ENEMY_HOP, 0x100);
            break;
        case 1:
            SetMotionX(0);
            SetMotionY(3);
            gAudioContext->PlaySfx(SFX_ENEMY_HOP, 0x100);
            break;
        }
    }
}

/* Update's state 7: once the target's animation is done, steps through
 * a turn cycle keyed off `mode`:
 * - mode 0: on odd `counter`s it turns around (the X mirror bit) and
 *   plays animation mode 1, otherwise mode 6; both motions restart
 *   (mode 0);
 * - mode 1: advances `counter` (0-3, wrapping) and restarts: X and Y
 *   motion 2, animation mode 0;
 * - mode 6: flips the Y mirror bit, then the same as mode 1.
 *
 * The bit toggles read the bit into a local first (`m = bit; bit =
 * !m;`), which gives the ROM's order: load, shift-test, then the 0/1
 * materialized, shifted and merged with the `-0x11` mask (issue #10
 * NAKED retry, docs/matching/archive/issue-10-naked-retry.md). Same
 * bytes under agbcp and old_agbcp. */
void EnemyCtrl::UpdateFlipCycle()
{
    MovingSprite *part = target;

    if (!part->animDone)
        return;
    switch (mode) {
    case 0:
        if (counter & 1) {
            u32 m = part->mirrorFlags.mirrorX;
            part->mirrorFlags.mirrorX = !m;
            SetAnimMode(1);
        } else {
            SetAnimMode(6);
        }
        SetMotionX(0);
        SetMotionY(0);
        break;
    case 1:
        counter = (counter + 1) % 4;
        SetMotionX(2);
        SetMotionY(2);
        SetAnimMode(0);
        break;
    case 6:
        {
            u32 m = part->mirrorFlags.mirrorY;
            part->mirrorFlags.mirrorY = !m;
        }
        counter = (counter + 1) % 4;
        SetMotionX(2);
        SetMotionY(2);
        SetAnimMode(0);
        break;
    }
}
