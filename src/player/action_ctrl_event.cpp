#include "action_ctrl.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "key_input.hpp"
#include "camera.hpp"

extern "C" {
#include "gfx.h"
#include "level.h"
#include "sprite_bank.h"
#include "globals.h"
#include "math_util.h"
#include "system.h"
}

/* ActionCtrl's Reset and event handler (include/action_ctrl.hpp; #664,
 * docs/cplusplus.md). Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS),
 * like the old_agbcc C it replaces: under agbcc the handler's C was 36
 * halfwords off. */

/* Clears the controller's state, the player, the motion queue (both
 * entries pending), the spin and bump timers and the flags. InitActionCtrl
 * runs it. */
void ActionCtrl::Reset()
{
    turboRun = 0;
    state = ACTION_STATE_IDLE;
    bumpedMotionX = 0;
    motionX = 0;
    motionY = 0;
    motionXPending = 1;
    motionYPending = 1;
    unk_14 = 0;
    part = 0;
    spinCooldown = 0;
    unk_2A = 0;
    dpadLockTimer = 0;
    bumpTimer = 0;
    frame = 0;
    frames = 0;
    idleFidget = 0;
    slamBlocked = 0;
}

/* Stores to the player's `hanging` and `bumped`: as inline parameters,
 * the values are materialized before the fields' addresses. */
static inline void SetHanging(Player *p, s32 hanging)
{
    p->hanging = hanging;
}

static inline void SetBumped(Player *p, s32 bumped)
{
    p->bumped = bumped;
}

/* The player's Y speed and its ramp (start, step, target). */
static inline void SetSpeedY(Player *p, s32 speed, s32 step, s32 target)
{
    p->speedY = speed;
    p->rampY.start = speed;
    p->rampY.step = step;
    p->rampY.target = target;
}

/* The events sent to the player's controller (PlayerHandleEvent,
 * CollidePlayer, the bounce pads), ignored while dying:
 * - EVENT_HANG_GRAB/RELEASE: hanging (animation 0x1D, the player moved by
 *   the change of his frame's anchor) and the fall from it;
 * - EVENT_BUMP: a crate's side stopped the X motion (`arg` the contact
 *   axes); unless idle, the queued X motion is kept in bumpedMotionX for
 *   3 frames if it went into the crate; a slide is ended;
 * - EVENT_BOUNCE/BOUNCE_HIGH: the bounce off a crate (animation 0x13, Y
 *   entry 0xF/0x11, or 0x10/0x12 with A held), and EVENT_LAUNCH_PAD the
 *   launch pad's air spin (Y entry 0x13, 0x14 with A held, 3 tornado
 *   turns); R held blocks the next body slam (`slamBlocked`);
 * - the warps: the warp out (animation 0x24), the music faded for the
 *   bonus round;
 * - the hits: KillPlayer with the death animation of each, the plain hits
 *   also stopping the player and setting the camera mode 3;
 * - EVENT_MASK_HIT: the Aku Aku mask absorbed a hit: on the ground and
 *   with room for it, StartMaskHitJump. */
void ActionCtrl::HandleEvent(MovingSprite *, s32 event, s32 arg)
{
    if (state == ACTION_STATE_DYING)
        return;
    switch (event) {
    case EVENT_HANG_GRAB:
        {
            const struct sprite_point *from = part->FrameAnchor();
            const struct sprite_point *to;

            SetHanging(part, 1);
            SetModeAnim(ACTION_STATE_HANG_GRAB, 0x1d, 0, 0);
            QueueNowX(0);
            QueueNowY(0);
            to = part->FrameAnchor();
            {
                s32 d = to->y - from->y;
                s32 y = part->y;

                part->y = y - INT_TO_Q8(d);
            }
        }
        break;
    case EVENT_HANG_RELEASE:
        part->hanging = 0;
        SetModeAnim(ACTION_STATE_AIRBORNE_FALL, 0x1b, 0x7FFFFFFF, 0x7FFFFFFF);
        QueueNowY(4);
        break;
    case EVENT_BUMP:
        if (state == ACTION_STATE_IDLE) {
            QueueNowX(0);
            bumpedMotionX = 0;
            part->hitAxes |= arg;
            part->speedX = 0;
            break;
        }
        {
            s32 m = arg & 3;

            if (m == 2) {
                if (motionX != 0 && (s8)(gPlayer->mirror << 3) < 0) {
                    bumpedMotionX = motionX;
                    QueueNowX(0);
                    part->speedX = 0;
                }
            } else if (m == 1) {
                if (motionX != 0 && !((u32)(gPlayer->mirror << 27) >> 31)) {
                    bumpedMotionX = motionX;
                    QueueX(0, m, 0);
                    part->speedX = 0;
                }
            } else {
                goto check_slide;
            }
            /* Dead on purpose (owner decision, #662): the ROM loads
             * `state` here and never uses it. This test is what leaves
             * that load; its `m = 0` is dead (`m` isn't read after
             * this), so the compiler drops the store, then the empty
             * test, but not the load. gcc 2.9 does it in stages
             * (#662 rounds 3-5, from jump.c, flow.c and cse.c):
             * - jump1 can't delete the test, because its body still
             *   does something;
             * - cse1's delete_trivially_dead_insns keeps `m = 0`,
             *   because `m` is used elsewhere in the function;
             * - flow1 deletes the dead store but keeps the branch and
             *   its compare;
             * - jump2, after reload, deletes the empty branch and its
             *   compare, but not the load feeding them (the `if (!
             *   reload_completed)` in delete_computation), and no flow
             *   pass runs after it.
             * Without it the function is 1 instruction short (the load;
             * plus a halfword of padding). What doesn't work:
             * - an empty body, every release-mode assert or log macro
             *   (`if (!(c)) {}`, an empty inline DebugHalt with or
             *   without __FILE__/__LINE__, a debug-level gate,
             *   `((void)(c))`, `do {} while (0)`), and anything built
             *   from inlines (an unused parameter, a discarded return
             *   value): deleted with their load before flow1 or by
             *   cse1;
             * - a store to a global or static error flag: keeps the
             *   load, but also its own compare and store (+5);
             * - round 9's 3240 type and test variants: every natural
             *   test stays a real compare.
             * The only other form that leaves the load is an if/else
             * with identical arms. Player::HandleEvent's mask-level
             * load (player_update.cpp) is the same mechanism and the
             * same fix; Crate::QueuePlayerCollision's (crate_break.cpp)
             * is the same shape written for a reason. */
            if (state == ACTION_STATE_SLIDE)
                m = 0;
            bumpTimer = 3;
            SetBumped(part, 1);
        }
    check_slide:
        if (state == ACTION_STATE_SLIDE && gPlayer->frame != 0) {
            Player *pl = gPlayer;

            frame = frames;
            bumpTimer = 0;
            pl->frame = pl->bank->anims[pl->tag].frameCount - 1;
            break;
        }
        part->hitAxes |= arg;
        part->speedX = 0;
        break;
    case EVENT_BOUNCE:
        /* The A test through INPUT_HELD (a u16 view of the keys, as
         * TryDoubleJump and the hang states read theirs) and the queue
         * through QueueNowY: the ROM's two 1s (`movs r5, #1; movs r4, #1`),
         * the queue's apart from the test's. The halfword AND is expanded
         * as a failed HImode AND, whose leftover HImode 1 cse then gives
         * the queue's byte stores; the SImode AND keeps a 1 of its own
         * (#662 round 7). */
        {
            u32 in = gKeys.all;

            if (INPUT_HELD(in) & R_BUTTON)
                slamBlocked = 1;
            if (INPUT_HELD(in) & A_BUTTON) {
                SetModeAnim(ACTION_STATE_JUMP, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                part->speedY = 0;
                QueueNowY(0x10);
            } else {
                SetModeAnim(ACTION_STATE_JUMP, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                part->speedY = 0;
                QueueNowY(0xF);
            }
        }
        frame = 0;
        break;
    case EVENT_BOUNCE_HIGH:
        {
            u32 in = gKeys.all;

            if (INPUT_HELD(in) & R_BUTTON)
                slamBlocked = 1;
            if (INPUT_HELD(in) & A_BUTTON) {
                SetModeAnim(ACTION_STATE_JUMP, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                part->speedY = 0;
                QueueNowY(0x12);
            } else {
                SetModeAnim(ACTION_STATE_JUMP, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                part->speedY = 0;
                QueueNowY(0x11);
            }
        }
        frame = 0;
        break;
    case EVENT_LAUNCH_PAD:
        {
            u32 in = gKeys.all;

            gAudioContext->PlaySfx(SFX_SPIN, 0x100);
            if (INPUT_HELD(in) & A_BUTTON) {
                SetModeAnim(ACTION_STATE_AIR_SPIN, 0x10, 0, 0x18);
                tornadoTurn = 0;
                tornadoFallQueued = 0;
                tornadoUnwinding = 0;
                charge = 3;
                part->speedY = 0;
                QueueNowY(0x14);
            } else {
                SetModeAnim(ACTION_STATE_AIR_SPIN, 0x10, 0, 0x18);
                tornadoTurn = 0;
                tornadoFallQueued = 0;
                tornadoUnwinding = 0;
                charge = 3;
                part->speedY = 0;
                QueueNowY(0x13);
            }
            ApplyMotion();
        }
        frame = 0;
        break;
    case EVENT_WARP_BONUS_ROUND:
        gAudioContext->FadeOutMusic(0);
        /* fallthrough */
    case EVENT_WARP_GEM_PATH:
    case EVENT_WARP_EXIT:
        gAudioContext->PlaySfx(SFX_WARP, 0x100);
        {
            u8 *f = &gPlayer->f.flags;

            *f &= 0x7f;
        }
        SetModeAnim(ACTION_STATE_WARP_OUT, 0x24, 0x7FFFFFFF, 0x7FFFFFFF);
        gPaletteCache->LoadSlot(part->palette, part->bank->anims[part->tag].paletteId);
        QueueNowX(0);
        QueueNowY(0);
        break;
    case EVENT_HIT_FIRE:
        KillPlayer(0x2e);
        break;
    case EVENT_HIT_ELECTRIC:
        KillPlayer(0x2c);
        break;
    case 7:
        KillPlayer(0x2b);
        break;
    case EVENT_HIT_DART:
        KillPlayer(0x2f);
        break;
    case EVENT_HIT_CORTEX_SHOT:
        KillPlayer(0x2d);
        break;
    case EVENT_HIT:
    case EVENT_HIT_EXPLOSION:
    case EVENT_HIT_BITE:
        {
            Player *p;
            s32 z;

            KillPlayer(0x1c);
            p = part;
            z = 0;
            if (p->slippery == 0)
                p->speedX = z;
            p->rampX.start = z;
            p->rampX.step = z;
            p->rampX.target = z;
            SetSpeedY(part, -0x100, 0, -0x100);
            gCamera->mode = 3;
        }
        break;
    case EVENT_HIT_CRUSH:
        KillPlayer(0x2a);
        break;
    case EVENT_MASK_HIT:
        if ((gPlayer->hitAxes & 8) && part->HasRoomForAnim(0xb) == 1) {
            ActAndFlags0D(part, -2);
            ActAndFlags0D(part, -3);
            StartMaskHitJump();
        }
        break;
    }
}

/* ActionCtrl's KillPlayer, skid animation and facing (include/
 * action_ctrl.hpp; #664, docs/cplusplus.md). Built by old_agbcp (the
 * Makefile's OLD_AGBCC_OBJS): KillPlayer has old_agbcc's
 * constant-before-`ldrb`, which the C, built by agbcc, got with 44 pins
 * and an asm. */

/* UpdateFacing's two turns. Each takes the constant the ROM materializes
 * first as a parameter (the turbo run's 0, the pending X motion's 1): an
 * inline parameter is computed before the body, as there; the right turn
 * keeps the mask in SImode (`& -0x11`, then `| 0x10`). */
static inline void FaceLeft(ActionCtrl *ctrl, Player *p, s32 turboRun)
{
    p->mirrorFlags.mirrorX = 0;
    ctrl->motionXPending = 1;
    ctrl->turboRun = turboRun;
}

static inline void FaceRight(ActionCtrl *ctrl, Player *p, s32 pending)
{
    u8 *mirror = &p->mirror;
    s32 m = ~0x10;

    m &= *mirror;
    m |= 0x10;
    *mirror = m;
    ctrl->motionXPending = pending;
    ctrl->turboRun = 0;
}

/* The player is hit: plays the hurt sound, switches to the dying state
 * on death animation `anim` (0x1C, 0x2A-0x2F from HandleEvent), cancels
 * both motion entries, makes the player intangible and dead, takes a life
 * and reloads the player's palette. */
void ActionCtrl::KillPlayer(s32 anim)
{
    gAudioContext->PlaySfx(SFX_PLAYER_HURT, 0x100);
    SetTargetAnim(part, anim);
    SetMode(ACTION_STATE_DYING);
    QueueNowX(0);
    QueueNowY(0);
    ApplyMotion();
    part->StoreSlippery(0);
    part->pushLeft = 0;
    part->pushRight = 0;
    part->f.b.collides = 0;
    part->f.b.vulnerable = 0;
    part->dead = 1;
    gLevelState->LoseLife();
    gPaletteCache->LoadSlot(part->palette, part->bank->anims[part->tag].paletteId);
}

/* Update calls this when the player's `slippery` changed. On slippery
 * ground the idle animation (0x12, while still moving) becomes the skid
 * 0x25, the run and turbo run animations (0xD, 0x18) the skid 0x26 (as
 * SetTargetAnim does), restarted; off it, a skid animation stops the skid
 * sound and goes back to idle. */
void ActionCtrl::UpdateSkidAnim()
{
    Player *player = gPlayer;
    s32 slippery = player->slippery;

    if (slippery) {
        u8 *tagp = &player->tag;
        s32 tag = *tagp;
        /* The ROM tests 0x18 on a copy of the animation, in r2: a `u8`
         * copy (an `s32` one is folded into `tag`). */
        u8 tagCopy = tag;

        if (tag == 0x12)
            goto idle;
        if (tag > 0x12)
            goto above;
        if (tag == 0xD)
            goto run;
        return;
    above:
        if (tagCopy == 0x18)
            goto run;
        return;
    idle:
        if (player->speedX == 0)
            return;
        *tagp = 0x25;
        goto restart;
    run:
        player = gPlayer;
        tag = 0x26;
        player->tag = tag;
    restart:
        player->ResetFrameTimer();
        player->ResetFrameIndex();
        player->SetAnimDone(0);
    } else {
        s32 tag = player->tag;

        /* Two tests: `tag == 0x25 || tag == 0x26` folds into one range
         * check. */
        if (tag == 0x25)
            goto unskid;
        if (tag == 0x26) {
        unskid:
            gAudioContext->StopSfx(SFX_SKID);
            SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, slippery);
        }
    }
}

/* Turns the player to face the D-pad's direction (directions 3/5/7 face
 * right, 4/6/8 left) in the states that steer; turning queues the X
 * motion again and ends the turbo run. Also clears the player's `mirror`
 * bit 5 (Y mirrored) in those states. Returns whether he turned. */
s32 ActionCtrl::UpdateFacing()
{
    s32 dir = gInput->GetDpadDirection();
    s32 turned = 0;

    switch (state) {
    case ACTION_STATE_IDLE:
    case ACTION_STATE_RUN:
    case ACTION_STATE_TURBO_RUN:
    case ACTION_STATE_JUMP:
    case ACTION_STATE_AIRBORNE_JUMP:
    case ACTION_STATE_AIRBORNE_FLIP_JUMP:
    case ACTION_STATE_AIRBORNE_HIGH_JUMP:
    case ACTION_STATE_SPIN:
    case ACTION_STATE_AIR_SPIN:
    case ACTION_STATE_TORNADO_SPIN:
    case ACTION_STATE_CRAWL:
    case ACTION_STATE_AIRBORNE_FALL:
    case ACTION_STATE_HANG:
    case ACTION_STATE_HANG_SPIN:
    case ACTION_STATE_HANG_MOVE_START:
    case ACTION_STATE_HANG_MOVE:
        break;
    default:
        goto done;
    }
    part->mirrorFlags.mirrorY = 0;
    if (part->mirrorBits.flipX < 0 && (dir == 4 || dir == 6 || dir == 8)) {
        FaceLeft(this, part, 0);
    } else if ((s32)(part->mirror << 27) >= 0 && (dir == 3 || dir == 5 || dir == 7)) {
        FaceRight(this, part, 1);
    } else {
        goto done;
    }
    turned = 1;
done:
    return turned;
}
