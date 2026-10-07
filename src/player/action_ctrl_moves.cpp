#include "action_ctrl.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"

extern "C" {
#include "match.h"
#include "audio.h"
#include "level.h"
#include "globals.h"
}

/* ActionCtrl's moves (include/action_ctrl.hpp; #664, docs/cplusplus.md):
 * the tornado fall, the end and steering of a spin, SetMode, the starts
 * of the spin, hang spin, run, high jump and mask-hit jump, Attach, and
 * the short state methods (hang grab, hang spin, warp out, crawl stop,
 * body slam start). Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS),
 * like the old_agbcc C it replaces. */

/* The tornado spin's slow descent, once per spin (`tornadoFallQueued`):
 * called once the spinning player leaves the ground (StateTornadoSpin)
 * or, in the air spin, starts falling (HandleAirInput). Queues Y motion
 * entry 0x18/0x19/0x1A by `tornadoTurn` (0-1, 2, 3-4; nothing above 4).
 * Those entries are gCtrlMotionRecords 29-31, {8, 18/14/6, 1280}: the
 * more turns, the slower the fall speeds up. Then sets the player's
 * `flags2` bit 0 and clears `slamBlocked`. */
void ActionCtrl::StartTornadoFall()
{
    if (tornadoFallQueued != 0)
        return;
    tornadoFallQueued = 1;
    {
        /* Two pins, both still needed in C++ (the C had seven and a
         * hand-written jump table): unpinned, `this` and `entry` swap r2
         * and r3, and the 0 for `slamBlocked` is loaded before the
         * player's `flags2` (into r2, pushing the `ldrb` to r4). */
        MATCH_HOLD_REG(s32, entry, r2);

        switch (tornadoTurn) {
        case 0:
        case 1:
            entry = 0x18;
            break;
        case 2:
            entry = 0x19;
            break;
        case 3:
        case 4:
            entry = 0x1A;
            break;
        default:
            goto queued;
        }
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = entry;
    }
queued:
    part->f.bytes.flags2 |= 1;
    {
        u8 *p = &slamBlocked;
        MATCH_HOLD_REG(s32, zero, r0) = 0;

        *p = zero;
    }
}

/* The end of a ground spin: the spin cooldown starts (12 frames). With
 * the D-pad sideways (`mode` 3/4) the run restarts: the turbo run if L
 * is held (`flags` is the input word) and HasTurboRun allows it,
 * otherwise StartRun. Any other direction goes back to idle. */
void ActionCtrl::EndSpin(u8 mode, s32 flags)
{
    spinCooldown = 0xC;
    switch (mode) {
    case 3:
    case 4:
        {
            s32 m = L_BUTTON;
            s32 m2;

            /* The ROM builds 0x200 in r1 and ANDs through a copy in r0,
             * into flags' own r2. Kept from the C: the MATCH_CONST escape
             * keeps the copy (m2) apart from m, and the volatile use of m
             * and flags right after the `and` stops combine from sinking
             * it into the test and regmove from retargeting it onto m2.
             * The natural `flags & L_BUTTON` ANDs into the constant's
             * register instead. */
            MATCH_CONST(m2, m);
            flags &= m2;
            asm volatile("" : "+r"(flags) : "r"(m));
        }
        if (flags != 0 && (u8)HasTurboRun(gLevelState)) {
            turboRun = 1;
            SetMode(ACTION_STATE_TURBO_RUN);
            SetTargetAnim(part, 0x18);
            QueueNowX(0x1B);
        } else {
            StartRun();
        }
        break;
    default:
        SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, 0);
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = 0;
        break;
    }
}

/* Steering during a spin: with no X motion queued and no crate bump, the
 * D-pad sideways (`mode` 3/4) queues X entry 0x17; `mode` 0-2 queues
 * entry 0. Then the facing (UpdateFacing). */
void ActionCtrl::SteerSpin(u8 mode)
{
    if (motionX == 0 && bumpTimer == 0) {
        switch (mode) {
        case 3:
        case 4:
            QueueNowX(0x17);
            break;
        }
    }
    if (mode <= 2)
        QueueNowX(0);
    UpdateFacing();
}

/* Enters state `mode`, keeping the old one in `prevState`, and cancels a
 * crate bump. Unless the new state is a spin (0xD, 0xE), the player's
 * `bumped`, `bounce` and `listCount` are cleared (the last one twice). */
void ActionCtrl::SetMode(s32 mode)
{
    idleFidget = 0;
    prevState = state;
    state = mode;
    bumpedMotionX = 0;
    bumpTimer = 0;
    if ((u32)(mode - ACTION_STATE_SPIN) > 1) {
        part->bumped = 0;
        gPlayer->bounce = 0;
        gPlayer->listCount = 0;
        gPlayer->listCount = 0;
    }
}

/* Starts a ground spin unless the cooldown runs: animation 0x10 for 0x18
 * frames, the tornado turns reset. */
void ActionCtrl::StartSpin()
{
    if (spinCooldown == 0) {
        PlaySfx(gAudioContext, SFX_SPIN, 0x100);
        frame = 0;
        frames = 0x18;
        SetTargetAnim(part, 0x10);
        SetMode(ACTION_STATE_SPIN);
        tornadoVariant = 0;
        charge = 0;
        tornadoTurn = 0;
        tornadoFallQueued = 0;
        tornadoUnwinding = 0;
    }
}

/* StartSpin while hanging: animation 0x1E, state 0x21. */
void ActionCtrl::StartHangSpin()
{
    if (spinCooldown == 0) {
        PlaySfx(gAudioContext, SFX_SPIN, 0x100);
        frame = 0;
        frames = 0x18;
        tornadoVariant = 0;
        charge = 0;
        tornadoTurn = 0;
        tornadoFallQueued = 0;
        tornadoUnwinding = 0;
        SetTargetAnim(part, 0x1E);
        SetMode(ACTION_STATE_HANG_SPIN);
    }
}

/* Starts the run: the turbo run (animation 0x18, X entry 0x1B) if
 * `turboRun` is set, else the plain run (animation 0xD, X entry 1). On
 * slippery ground the speed is kept. */
void ActionCtrl::StartRun()
{
    if (turboRun != 0) {
        frame = 0;
        SetTargetAnim(part, 0x18);
        SetMode(ACTION_STATE_TURBO_RUN);
        QueueNowX(0x1B);
        if (part->slippery != 0)
            motionXKeepSpeed = 1;
    } else {
        SetTargetAnim(part, 0xD);
        frame = 0;
        SetMode(ACTION_STATE_RUN);
        QueueNowX(1);
        if (part->slippery != 0)
            motionXKeepSpeed = 1;
    }
}

/* The high jump: state 0xB, animation 0xB, Y entry 0xB. */
void ActionCtrl::StartHighJump()
{
    SetModeAnimNow(ACTION_STATE_AIRBORNE_HIGH_JUMP, 0xB, 0);
    QueueNowY(0xB);
    part->hitAxes = 0;
}

/* StartHighJump with Y entry 7 (the plain jump the A button queues in
 * StateRun) instead of 0xB. The hop Crash makes when the Aku Aku mask
 * absorbs a hit: HandleEvent's event 11 calls it, and the only sender of
 * event 11 is PlayerHandleEvent's hit cases (1-10), right after they drop
 * the mask level by one (mask level 1 or 2). */
void ActionCtrl::StartMaskHitJump()
{
    SetModeAnimNow(ACTION_STATE_AIRBORNE_HIGH_JUMP, 0xB, 0);
    QueueNowY(7);
    part->hitAxes = 0;
}

void ActionCtrl::Attach(MovingSprite *owner)
{
    part = (Player *)owner;
}

/* gActionCtrlStateTable's slot 0x27, a state nothing sets: runs
 * ReleaseHang. */
void ActionCtrl::StateUnusedHangRelease()
{
    ReleaseHang();
}

/* gActionCtrlStateTable's slot 0x23, a state nothing sets; the same code
 * as StateHangGrab (slot 0x1F). */
void ActionCtrl::StateUnusedHangGrab()
{
    if (part->animDone != 0) {
        SetModeAnimNow(ACTION_STATE_HANG, 0x1F, 0, 0);
    }
}

/* The spin while hanging: after `frames` frames, or once the animation is
 * done, back to hanging (animation 0x1F) with the spin cooldown started.
 * Then the facing. */
void ActionCtrl::StateHangSpin()
{
    frame += 1;
    if (frame >= frames || part->animDone != 0) {
        spinCooldown = 0xC;
        SetMode(ACTION_STATE_HANG);
        SetTargetAnim(part, 0x1F);
        frame = 0;
        frames = 0;
    }
    UpdateFacing();
}

/* Grabbing a ledge: once the grab animation is done, hanging (animation
 * 0x1F). */
void ActionCtrl::StateHangGrab()
{
    if (part->animDone != 0) {
        SetModeAnimNow(ACTION_STATE_HANG, 0x1F, 0, 0);
    }
}

/* Warping out: once the animation is done, the player's collision is
 * switched on and the room ends. */
void ActionCtrl::StateWarpOut()
{
    if (part->animDone != 0) {
        gPlayer->f.flags |= 0x80;
        RequestRoomExit();
    }
}

/* Once the animation is done: crouching (animation 4). */
void ActionCtrl::StateCrawlStop()
{
    if (part->animDone != 0) {
        SetMode(ACTION_STATE_CROUCH);
        SetTargetAnim(part, 4);
    }
}

/* Once the start animation is done: the super body slam (animation 7)
 * if HasSuperBodySlam allows it, else the body slam. */
void ActionCtrl::StateBodySlamStart()
{
    if (part->animDone != 0) {
        if ((u8)HasSuperBodySlam(gLevelState)) {
            SetMode(ACTION_STATE_AIRBORNE_SUPER_BODY_SLAM);
            SetTargetAnim(part, 7);
        } else {
            SetMode(ACTION_STATE_AIRBORNE_BODY_SLAM);
        }
    }
}
