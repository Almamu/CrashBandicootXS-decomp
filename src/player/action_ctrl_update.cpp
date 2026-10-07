#include "action_ctrl.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"

extern "C" {
#include "system.h"
#include "audio.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
}

/* ActionCtrl's Update, and its double jump and in-air input helpers
 * (include/action_ctrl.hpp; #664, docs/cplusplus.md). Built by old_agbcp
 * (the Makefile's OLD_AGBCC_OBJS), like the old_agbcc C it replaces. The
 * C wrote the state dispatch out as 20 lines of pointer-to-member
 * arithmetic; here it is `(this->*stateFuncs[state])()`. */

/* `in & mask` through an inline (Update's R test): the 0x100 then goes
 * straight into the `ands` operand, not into a register IsSlippery's
 * offset would share. */
static inline u32 KeysHeld(u32 in, u32 mask)
{
    return in & mask;
}


/* Each frame: clears `slamBlocked` once R is released; reruns the skid
 * animation (UpdateSkidAnim) when the player's `slippery` changed; below
 * the level's bottom edge (`gLevelLayers`'s layer 0 height) stops the
 * player's X motion and makes him intangible, and further down kills him
 * (event 1, a plain hit) with the Aku Aku mask gone. Then the spin
 * cooldown and the crate bump countdown (which re-queues the X motion the
 * bump cancelled), the state method, the end of a jump's queued rise when
 * the player hit a ceiling (`hitAxes` bit 3), the queued motion
 * (ApplyMotion), and the player's attack kind for the hit handlers
 * (`kind`: 0x13 spinning, 0x14 sliding, 0x15/0x16 body slamming, else 1). */
void ActionCtrl::Update(MovingSprite *)
{
    u32 in = gKeys.all;

    if (slamBlocked != 0) {
        u16 held = KeysHeld(in, R_BUTTON);

        if (held == 0)
            slamBlocked = held;
    }
    if (prevSlippery != IsSlippery(part))
        UpdateSkidAnim();
    prevSlippery = IsSlippery(part);
    {
        Player *p = part;
        s32 y = p->y;

        if (y > INT_TO_Q8(gLevelLayers->layer0->heightPx) - 0x1400) {
            p->f.flags &= 0x7F;
            {
                Player *q = part;

                if (IsSlippery(q) == 0)
                    q->speedX = 0;
                q->rampX.start = 0;
                q->rampX.step = 0;
                q->rampX.target = 0;
            }
            {
                Player *r = part;
                s32 y2 = r->y;

                if (y2 > INT_TO_Q8(gLevelLayers->layer0->heightPx) + 0x1400) {
                    r->deadline = 0;
                    SetMaskLevel(gLevelState, MASK_LEVEL_NONE);
                    HandleEvent(0, 1, 0);
                }
            }
        }
    }
    if (spinCooldown)
        spinCooldown--;
    if (bumpTimer != 0) {
        s32 count = part->listCount;

        if (count <= 1) {
            u8 left = --bumpTimer;

            if (left == 0) {
                if (motionX == 0) {
                    u8 entry = bumpedMotionX;

                    motionXKeepSpeed = left;
                    motionXPending = 1;
                    motionX = entry;
                    bumpedMotionX = left;
                }
                gPlayer->bumped = left;
            }
        }
    }
    (this->*stateFuncs[state])();
    part->hitAxes &= 8;
    if (part->hitAxes == 8) {
        if (motionY == 4 || motionY == 5) {
            motionYKeepSpeed = 0;
            motionYPending = 1;
            motionY = 0;
        }
    }
    ApplyMotion();
    {
        Player *p = part;
        u32 top = p->f.flags >> 7;

        if (top == 0) {
            if (IsSlippery(p) == 0)
                p->speedX = top;
            p->rampX.start = top;
            p->rampX.step = top;
            p->rampX.target = top;
        }
    }
    switch (state) {
    case ACTION_STATE_SPIN:
    case ACTION_STATE_AIR_SPIN:
    case ACTION_STATE_TORNADO_SPIN:
    case ACTION_STATE_HANG_SPIN:
        part->kind = 0x13;
        break;
    case ACTION_STATE_SLIDE:
        part->kind = 0x14;
        break;
    case ACTION_STATE_AIRBORNE_BODY_SLAM:
        part->kind = 0x15;
        break;
    case ACTION_STATE_AIRBORNE_SUPER_BODY_SLAM:
        part->kind = 0x16;
        break;
    default:
        part->kind = 1;
        break;
    }
}

/* The double jump (HasDoubleJump), on A pressed in the air, once per
 * jump (`frame` counts it): from a jump's rise (animation 6) a flip jump,
 * from a high jump's (0xB) or a slide jump's (0xC) a higher one. Returns
 * whether it jumped. */
u8 ActionCtrl::TryDoubleJump()
{
    u32 in = gKeys.all;
    u16 pressed;
    s32 one;

    if (state == ACTION_STATE_AIR_SPIN)
        return 0;
    pressed = INPUT_PRESSED(in) & 1;
    one = 1;
    if (pressed) {
        s32 jumps = frame;

        if (jumps == 0 && (u8)HasDoubleJump(gLevelState)) {
            if (part->tag == 6 && part->frame >= 0) {
                frame++;
                gPlayer->StoreSlippery(jumps);
                SetTargetAnim(part, 0x12);
                SetMode(ACTION_STATE_AIRBORNE_FLIP_JUMP);
                SetTargetAnim(part, 6);
                QueueX(jumps, one, 0xD);
                QueueY(jumps, one, 0xD);
                PlaySfx(gAudioContext, SFX_HIGH_JUMP, 0x100);
                return 1;
            } else if (part->tag == 0xB && part->frame >= 0) {
                frame++;
                SetMode(ACTION_STATE_AIRBORNE_HIGH_JUMP);
                SetTargetAnim(part, 0xA);
                QueueNowX(0xE);
                QueueNowY(0xE);
                PlaySfx(gAudioContext, SFX_HIGH_JUMP, 0x100);
                return 1;
            } else if (part->tag == 0xC) {
                frame++;
                SetMode(ACTION_STATE_AIRBORNE_HIGH_JUMP);
                SetTargetAnim(part, 0xA);
                QueueNowX(0xC);
                QueueNowY(0xC);
                PlaySfx(gAudioContext, SFX_HIGH_JUMP, 0x100);
                return 1;
            }
        }
    }
    return 0;
}

/* The airborne states' input: near the top of the jump (falling speed
 * up to 0x27F) the double jump (TryDoubleJump); then, past each jump's
 * apex, the fall (state 0x1A, the player's `flags2` bit 0 set); in the
 * air spin, the tornado fall once falling. R near the top starts a body
 * slam (the flip body slam after a flip jump) unless `slamBlocked`. The
 * D-pad steers (X entries 7, 0xD after a double jump, 0x1C on ice). */
void ActionCtrl::HandleAirInput()
{
    u8 near = 0;
    u32 in;

    if (part->speedY <= 0x27F) {
        near = 1;
        if (TryDoubleJump())
            return;
    }
    in = gKeys.all;
    {
        Player *p;

        if (state == ACTION_STATE_AIRBORNE_JUMP) {
            p = part;
            if (-p->speedY > 0x1BF)
                goto done;
            if (-p->speedY > 0x17F)
                goto done;
            goto hit;
        } else if (state == ACTION_STATE_AIRBORNE_FLIP_JUMP) {
            p = part;
            if (-p->speedY > 0x1BF)
                goto done;
            if (-p->speedY > 0x7F)
                goto done;
            goto hit;
        } else if (state == ACTION_STATE_AIRBORNE_HIGH_JUMP) {
            p = part;
            if (-p->speedY > 0xFF)
                goto done;
            if (-p->speedY > 0x1F)
                goto done;
        hit:
            ActOrFlags0D(p, 1);
            SetMode(ACTION_STATE_AIRBORNE_FALL);
        } else if (state == ACTION_STATE_AIR_SPIN) {
            if (tornadoTurn) {
                s32 speed;

                p = part;
                speed = p->speedY;
                if (-speed <= 0x7F)
                    ActOrFlags0D(p, 1);
                if (speed > 0)
                    StartTornadoFall();
            }
        }
    }
done:
    if (state != ACTION_STATE_AIR_SPIN && state != ACTION_STATE_AIRBORNE_HIGH_JUMP && near &&
        (in & R_BUTTON)) {
        u8 blocked = slamBlocked;

        if (blocked == 0) {
            u8 prev;

            /* flags2 through a pointer to it: `part->flags2 |= 1`
             * puts the part in r0 and the 1 in r1 (the ROM has them the
             * other way round), and ActOrFlags0D's 1 is reused for the
             * motion queue's stores. */
            {
                u8 *flags2 = &part->f.bytes.flags2;

                *flags2 |= 1;
            }
            prev = prevState;
            if (prev == ACTION_STATE_AIRBORNE_FLIP_JUMP ||
                state == ACTION_STATE_AIRBORNE_FLIP_JUMP) {
                SetMode(ACTION_STATE_FLIP_BODY_SLAM_START);
                motionXKeepSpeed = blocked;
                motionXPending = 1;
                motionX = blocked;
                QueueY(blocked, 1, 0x16);
                StateFlipBodySlamStart();
                unk_2A = blocked;
                return;
            }
            if (prev == ACTION_STATE_AIRBORNE_JUMP) {
                SetMode(ACTION_STATE_BODY_SLAM_START);
                SetTargetAnim(part, 0x19);
                motionXKeepSpeed = blocked;
                motionXPending = 1;
                motionX = blocked;
                QueueY(blocked, 1, 0x15);
            }
        }
    }
    if (state == ACTION_STATE_AIRBORNE_JUMP || state == ACTION_STATE_AIRBORNE_FLIP_JUMP ||
        state == ACTION_STATE_AIRBORNE_HIGH_JUMP || state == ACTION_STATE_AIR_SPIN ||
        state == ACTION_STATE_AIRBORNE_FALL) {
        if (GetDpadDirection(gInput) <= 2) {
            QueueNowX(0);
        } else {
            if (motionX == 0x1B || motionX == 0x1C) {
                QueueNowXKeepSpeed(0x1C);
            } else if (state == ACTION_STATE_AIR_SPIN) {
                QueueNowX(7);
            } else if (frame != 0) {
                if (motionX != 0xD)
                    QueueNowX(0xD);
            } else {
                QueueNowX(7);
            }
        }
        if (gPlayer->slippery)
            motionXKeepSpeed = 1;
    }
}
