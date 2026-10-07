#include "player_ctrl.hpp"
#include "spawners.hpp"
#include "player.hpp"

extern "C" {
#include "util.h"
#include "system.h"
#include "audio.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
}

/* GitHub issue #19: 0x080159F8-0x08015FDC, the three jump-table
 * dispatchers of the swim controller, PlayerCtrl (include/player_ctrl.hpp;
 * #664, docs/cplusplus.md), documented in
 * docs/matching/archive/issue-19-0x08015840-actor.md. All three are called
 * from swim_ctrl.cpp's state methods.
 *
 * Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS), like swim_ctrl.cpp
 * and the old_agbcc C it replaces. The "extra scratch-register copy" that
 * kept them NAKED under the current agbcc is simply old_agbcc's register
 * allocation.
 *
 * StartStroke and StartSpin dispatch on `tilt` (a 13-step direction
 * index). The case bodies appear in the ROM in the order below (the
 * source order); the shared tails are gcc's cross-jumping, not gotos.
 *
 * The facing tests read `mirrorBits.flipX < 0`, the signed view: as C++
 * the unsigned `mirrorFlags.mirrorX` test compiles to an `ands #16`, where
 * the ROM (and the C front end) has `lsls #27` and a sign test. */

/* `v`, mirrored when the target faces left */
#define SIGNED_X(t, v) ((t)->mirrorBits.flipX < 0 ? -(v) : (v))

/* Sets the target's speed for the current `tilt` (state 4, the turn,
 * instead reads the frame-indexed stack copy of gPlayerCtrlTurnSpeeds,
 * negated unless `mode` is 6) and steps `tilt` towards 0/3/6/9/12. */
void PlayerCtrl::StartStroke()
{
    Player *t;

    deadline = gRoomFrameCount + 16;
    if (state == 4) {
        struct speed_table tbl = gPlayerCtrlTurnSpeeds;

        if (mode == 6)
            target->speedX = tbl.v[target->frame];
        else
            target->speedX = -tbl.v[target->frame];
        return;
    }

    t = target;
    /* the ROM re-stores the byte it just read (ldrb/strb); a plain
     * self-assignment is deleted by the optimizer */
    *(volatile u8 *)&t->tag = t->tag;
    ResetSpriteFrameTimer(t);
    ResetSpriteFrameIndex(t);
    SetSpriteAnimDone(t, 0);
    SetState(2, 2, CTRL_KEEP, CTRL_KEEP);

    switch (tilt) {
    case 1:
        target->speedX = SIGNED_X(target, 176);
        target->speedY = -704;
        tilt = 0;
        break;
    case 2:
        target->speedX = SIGNED_X(target, 352);
        target->speedY = -704;
        tilt = 3;
        break;
    case 4:
        target->speedX = SIGNED_X(target, 704);
        target->speedY = -352;
        tilt = 3;
        break;
    case 5:
        target->speedX = SIGNED_X(target, 704);
        target->speedY = -176;
        tilt = 6;
        break;
    case 7:
        target->speedX = SIGNED_X(target, 704);
        target->speedY = 176;
        tilt = 6;
        break;
    case 8:
        target->speedX = SIGNED_X(target, 704);
        target->speedY = 352;
        tilt = 9;
        break;
    case 10:
        target->speedX = SIGNED_X(target, 352);
        target->speedY = 704;
        tilt = 9;
        break;
    case 11:
        target->speedX = SIGNED_X(target, 176);
        target->speedY = 704;
        tilt = 12;
        break;
    case 6:
        {
            s32 v = SIGNED_X(target, 704);
            target->speedX = v;
        }
        break;
    case 3:
        target->speedX = SIGNED_X(target, 528);
        target->speedY = -528;
        break;
    case 0:
        target->speedY = -704;
        break;
    case 9:
        target->speedX = SIGNED_X(target, 528);
        target->speedY = 528;
        break;
    case 12:
        target->speedY = 704;
        break;
    }
}

/* The same dispatch with a fixed speed of 960 and a 3/4 factor on the
 * diagonals. `speed` is a u16: gcc then knows `speed * 3` is non-negative
 * and divides by 4 with a plain shift, while `-speed * 3 / 4` gets the
 * round-toward-zero adjustment (fold() distributes `* 3 / 4` over the
 * SIGNED_X ternary, so each arm is divided separately). `flag` zeroes the
 * horizontal speed when the target's +0x68 bits are set and the D-pad is
 * not held sideways. */
void PlayerCtrl::StartSpin()
{
    u16 speed;
    u8 flag;

    if (spinCooldown != 0)
        return;

    speed = 960;
    flag = 0;
    PlaySfx(gAudioContext, SFX_UNKNOWN_09, 256);
    if ((target->hitAxes & 3) && GetDpadDirection(gInput) <= 2)
        flag = 1;

    if (state == 4) {
        if (flag) {
            target->speedX = 0;
        } else {
            s32 v = speed;
            if (mode == 7)
                v = -speed;
            target->speedX = v;
        }
        return;
    }

    SetState(3, 3, 0, 24);
    switch (tilt) {
    case 6:
        if (!flag) {
            s32 v;
            if (target->mirrorBits.flipX < 0)
                v = -speed;
            else
                v = speed;
            target->speedX = v;
        } else
            target->speedX = 0;
        break;
    case 3:
        if (!flag) {
            s32 v = SIGNED_X(target, speed) * 3 / 4;
            target->speedX = v;
        } else
            target->speedX = 0;
        target->speedY = -speed * 3 / 4;
        break;
    case 0:
        {
            s32 v = -speed;
            target->speedY = v;
        }
        break;
    case 9:
        if (!flag) {
            s32 v = SIGNED_X(target, speed) * 3 / 4;
            target->speedX = v;
        } else
            target->speedX = 0;
        target->speedY = speed * 3 / 4;
        break;
    case 12:
        target->speedY = speed;
        break;
    }
}

/* GitHub issue #19: 0x08015DF8-0x08015FDC, the third jump-table
 * dispatcher, after StartStroke/StartSpin above - see
 * docs/matching/archive/issue-19-0x08015840-actor.md. The
 * "scratch-register copy" before each `>> 2` (`adds r5,r0,r5; adds
 * r1,r5,#0; asrs r6,r1,#2`) that kept this NAKED under the current agbcc
 * is what old_agbcc (and old_agbcp) emits for plain code. */

/* Picks three tuning values by `state` - `mag` (always 300), `valB` and
 * `valA` - reads the D-pad direction (GetDpadDirection), on a 1-in-128 frame
 * tick and a coin flip spawns a kind-4 object at the target's position via
 * SpawnEffectPart (clearing its +0x0C bit 2), then feeds the direction's
 * (valB/valA, +-mag) pair, 3/4-scaled on the diagonals, to SetDriftX and
 * SetDriftY. `mag`/`valB`/`valA` are unsigned, so `x * 3 / 4` is a plain
 * shift and only `-mag * 3 / 4` rounds toward zero. */
void PlayerCtrl::ApplySwimDrift()
{
    u16 mag;
    u8 valB;
    u8 valA;
    u8 dir;

    if (state == 2) {
        mag = 300;
        valB = 20;
        valA = 30;
    } else if (state == 3) {
        mag = 300;
        valB = 20;
        valA = 32;
    } else {
        mag = 300;
        valB = 15;
        valA = 5;
    }

    dir = GetDpadDirection(gInput);
    if ((gRoomFrameCount & 0x7F) == 0 && (u16)RandRange(2) == 0) {
        Player *t = target;
        s32 x = Q8_TO_INT(t->x);
        s32 y = Q8_TO_INT(t->y) - 20;
        s32 flip = t->mirrorFlags.mirrorX;
        MovingSprite *obj = gEntitySpawner->SpawnEffectPart(40, 4, x, y, flip);

        if (obj != NULL)
            obj->f.b.visible = 0;
    }

    switch (dir) {
    case 5:
        SetDriftX(0, valB * 3 / 4, -mag * 3 / 4);
        SetDriftY(0, valB * 3 / 4, -mag * 3 / 4);
        break;
    case 6:
        SetDriftX(0, valB * 3 / 4, mag * 3 / 4);
        SetDriftY(0, valB * 3 / 4, -mag * 3 / 4);
        break;
    case 8:
        SetDriftX(0, valB * 3 / 4, mag * 3 / 4);
        SetDriftY(0, valB * 3 / 4, mag * 3 / 4);
        break;
    case 7:
        SetDriftX(0, valB * 3 / 4, -mag * 3 / 4);
        SetDriftY(0, valB * 3 / 4, mag * 3 / 4);
        break;
    case 0:
        SetDriftY(0, valA, 0);
        SetDriftX(0, valA, 0);
        break;
    case 3:
        SetDriftX(0, valB, -mag);
        SetDriftY(0, valA, 0);
        break;
    case 4:
        SetDriftX(0, valB, mag);
        SetDriftY(0, valA, 0);
        break;
    case 1:
        SetDriftX(0, valA, 0);
        SetDriftY(0, valB, -mag);
        break;
    case 2:
        SetDriftX(0, valA, 0);
        SetDriftY(0, valB, mag);
        break;
    }
}
