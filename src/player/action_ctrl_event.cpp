#include "action_ctrl.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "match.h"
#include "audio.h"
#include "gfx.h"
#include "level.h"
#include "sprite_bank.h"
#include "globals.h"
#include "math_util.h"
}

/* ActionCtrl's event handler (include/action_ctrl.hpp; #664,
 * docs/cplusplus.md). Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS),
 * like the old_agbcc C it replaces: under agbcc the same C was 36
 * halfwords off. */

/* GetSpriteFrameAnchor inlined: the player's current frame's anchor
 * point (the 3-box and 1-box frame layouts have one; the others use
 * gEmptySpritePoint). */
static inline const struct sprite_point *FrameAnchor(struct player *p)
{
    const struct sprite_frame *info =
        (const struct sprite_frame *)GetSpriteFrame((struct gfx_part *)p);

    switch (info->pieces[0] >> 4) {
    case 0:
        return &((const struct sprite_frame_3box_anchor *)info)->anchor;
    case 1:
        return &gEmptySpritePoint;
    case 2:
        return &gEmptySpritePoint;
    case 3:
        return &gEmptySpritePoint;
    case 4:
        return &gEmptySpritePoint;
    case 5:
        return &gEmptySpritePoint;
    case 6:
        return &((const struct sprite_frame_1box_anchor *)info)->anchor;
    default:
        return &gEmptySpritePoint;
    }
}

/* Stores to the player's `hanging` and `bumped`: as inline parameters,
 * the values are materialized before the fields' addresses. */
static inline void SetHanging(struct player *p, s32 hanging)
{
    p->hanging = hanging;
}

static inline void SetBumped(struct player *p, s32 bumped)
{
    p->bumped = bumped;
}

/* The player's Y speed and its ramp (start, step, target). */
static inline void SetSpeedY(struct player *p, s32 speed, s32 step, s32 target)
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
void ActionCtrl::HandleEvent(SpriteObj *, s32 event, s32 arg)
{
    if (state == ACTION_STATE_DYING)
        return;
    switch (event) {
    case EVENT_HANG_GRAB:
        {
            const struct sprite_point *from = FrameAnchor(part);
            const struct sprite_point *to;

            SetHanging(part, 1);
            SetModeAnim(ACTION_STATE_HANG_GRAB, 0x1d, 0, 0);
            QueueNowX(0);
            QueueNowY(0);
            to = FrameAnchor(part);
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
                if (motionX != 0 && (s8)(gPlayer->mirror.all << 3) < 0) {
                    bumpedMotionX = motionX;
                    QueueNowX(0);
                    part->speedX = 0;
                }
            } else if (m == 1) {
                if (motionX != 0 && !((u32)(gPlayer->mirror.all << 27) >> 31)) {
                    bumpedMotionX = motionX;
                    QueueX(0, m, 0);
                    part->speedX = 0;
                }
            } else {
                goto check_slide;
            }
            /* The ROM has a dead load of the state here. */
            *(volatile s32 *)&state;
            bumpTimer = 3;
            SetBumped(part, 1);
        }
    check_slide:
        if (state == ACTION_STATE_SLIDE && gPlayer->frame != 0) {
            struct player *pl = gPlayer;

            frame = frames;
            bumpTimer = 0;
            pl->frame = pl->anim->records[pl->tag].frameCount - 1;
            break;
        }
        part->hitAxes |= arg;
        part->speedX = 0;
        break;
    case EVENT_BOUNCE:
        {
            u32 in = gKeys.all;
            u32 held = in;
            s32 fire;
            s32 one;

            if (held & R_BUTTON)
                slamBlocked = 1;
            MATCH_CONST(one, 1);
            fire = held & 1;
            if (fire) {
                SetModeAnim(ACTION_STATE_JUMP, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                part->speedY = 0;
                QueueY(0, one, 0x10);
            } else {
                SetModeAnim(ACTION_STATE_JUMP, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                part->speedY = fire;
                QueueY(0, one, 0xF);
            }
        }
        frame = 0;
        break;
    case EVENT_BOUNCE_HIGH:
        {
            u32 in = gKeys.all;
            u32 held = in;
            s32 fire;
            s32 one;

            if (held & R_BUTTON)
                slamBlocked = 1;
            MATCH_CONST(one, 1);
            fire = held & 1;
            if (fire) {
                SetModeAnim(ACTION_STATE_JUMP, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                part->speedY = 0;
                QueueY(0, one, 0x12);
            } else {
                SetModeAnim(ACTION_STATE_JUMP, 0x13, 0x7FFFFFFF, 0x7FFFFFFF);
                part->speedY = fire;
                QueueY(0, one, 0x11);
            }
        }
        frame = 0;
        break;
    case EVENT_LAUNCH_PAD:
        {
            u32 in = gKeys.all;
            s32 fire;
            s32 one;

            PlaySfx(gAudioContext, SFX_SPIN, 0x100);
            MATCH_CONST(one, 1);
            fire = in & 1;
            if (fire) {
                SetModeAnim(ACTION_STATE_AIR_SPIN, 0x10, 0, 0x18);
                tornadoTurn = 0;
                tornadoFallQueued = 0;
                tornadoUnwinding = 0;
                charge = 3;
                part->speedY = 0;
                QueueY(0, one, 0x14);
            } else {
                SetModeAnim(ACTION_STATE_AIR_SPIN, 0x10, 0, 0x18);
                tornadoTurn = fire;
                tornadoFallQueued = fire;
                tornadoUnwinding = fire;
                charge = 3;
                part->speedY = fire;
                QueueY(0, one, 0x13);
            }
            ApplyMotion();
        }
        frame = 0;
        break;
    case EVENT_WARP_BONUS_ROUND:
        FadeOutMusic(gAudioContext, 0);
        /* fallthrough */
    case EVENT_WARP_GEM_PATH:
    case EVENT_WARP_EXIT:
        PlaySfx(gAudioContext, SFX_WARP, 0x100);
        {
            u8 *f = &gPlayer->flags.all;

            *f &= 0x7f;
        }
        SetModeAnim(ACTION_STATE_WARP_OUT, 0x24, 0x7FFFFFFF, 0x7FFFFFFF);
        LoadPaletteSlot(gPaletteCache, part->slot, part->anim->records[part->tag].paletteId);
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
    case 8:
        KillPlayer(0x2f);
        break;
    case EVENT_HIT_CORTEX_SHOT:
        KillPlayer(0x2d);
        break;
    case EVENT_HIT:
    case EVENT_HIT_EXPLOSION:
    case EVENT_HIT_BITE:
        {
            struct player *p;
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
        if ((gPlayer->hitAxes & 8) && PlayerHasRoomForAnim((struct box_part *)part, 0xb) == 1) {
            ActAndFlags0D(part, -2);
            ActAndFlags0D(part, -3);
            StartMaskHitJump();
        }
        break;
    }
}
