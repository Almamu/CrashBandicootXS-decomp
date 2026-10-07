#include "action_ctrl.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"

extern "C" {
#include "match.h"
#include "system.h"
#include "audio.h"
#include "crates.h"
#include "gfx.h"
#include "level.h"
#include "sprite_bank.h"
#include "box_part.h"
#include "globals.h"
#include "math_util.h"
}

/* ActionCtrl's state methods for leaving the ground, dying, warping in
 * and hanging, the super body slam's shockwave and the tornado spin's
 * turns (include/action_ctrl.hpp; #664, docs/cplusplus.md). Built by
 * old_agbcp (the Makefile's OLD_AGBCC_OBJS), like the old_agbcc C it
 * replaces. */

/* The player's animation set to `tag`, restarted. */
static inline void SetTag(Player *part, s32 tag)
{
    part->tag = tag;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
}

/* The state CheckLeftGround enters when the player walks off the ground.
 * Back on it (`hitAxes` bit 3): the run or turbo run resumes from its
 * animation (0xD, 0x18), the air spin goes on as the ground spin,
 * anything else goes idle. Still off it: A jumps (animation 0x13, Y
 * entry 7), B spins, R crouches down; with the D-pad idle the X motion
 * stops.
 *
 * `hit` is assigned in the test: initialised at its declaration, the
 * test reads a copy of it (an extra `adds`). */
void ActionCtrl::StateLeftGround()
{
    u8 hit;

    if ((hit = part->hitAxes & 8) != 0) {
        u8 tag;
        /* the ROM's r5 zero, reused by the air spin's trio below */
        u8 z;

        /* flags2 through a pointer to it, as in HandleAirInput:
         * `part->flags2 |= 1` and ActOrFlags0D allocate differently. The
         * 0 is stored through a pointer so it is loaded after the
         * address. */
        {
            u8 *flags2 = &part->f.bytes.flags2;

            *flags2 |= 1;
        }
        {
            u8 *blocked = &slamBlocked;

            z = 0;
            *blocked = z;
        }
        switch (tag = part->tag) {
        case 0xD:
        case 0x18:
            if (tag == 0xD) {
                if (turboRun) {
                    frame = 0;
                    SetTargetAnim(part, 0x18);
                    SetMode(ACTION_STATE_TURBO_RUN);
                    QueuePendingX(0, 0x1B);
                } else {
                    frame = 0;
                    SetMode(ACTION_STATE_RUN);
                    if (motionX != 1)
                        QueueX(0, 1, 1);
                }
                QueueNowY(0);
            } else if (tag == 0x18) {
                SetMode(ACTION_STATE_TURBO_RUN);
                motionYKeepSpeed = 0;
                motionYPending = 1;
                motionY = 0;
                turboRun = 1;
                QueuePendingX(0, 0x1B);
            }
            break;
        default:
            if (state == ACTION_STATE_AIR_SPIN) {
                if (gKeys.all & DPAD_SIDEWAYS) {
                    QueuePendingX(0, 1);
                } else {
                    motionXKeepSpeed = z;
                    motionXPending = 1;
                    motionX = z;
                }
                QueueNowY(0);
                SetMode(ACTION_STATE_SPIN);
            } else {
                SetMode(ACTION_STATE_IDLE);
                motionYKeepSpeed = z;
                motionYPending = 1;
                motionY = 0;
                motionXKeepSpeed = 0;
                motionXPending = 1;
                motionX = 0;
            }
        }
        return;
    }
    {
        u32 in = gKeys.all;
        u16 p = INPUT_PRESSED(in);
        s32 fire = p & 1;

        if (fire) {
            SetMode(ACTION_STATE_JUMP);
            SetTargetAnim(part, 0x13);
            frame = hit;
            QueueY(hit, 1, 7);
        } else {
            s32 t = 2;
            u16 alt;

            t &= p;
            alt = t;

            if (alt) {
                ActOrFlags0D(part, 1);
                slamBlocked = fire;
                if (gKeys.all & DPAD_SIDEWAYS)
                    QueueX(fire, 1, 1);
                else
                    QueueX(0, 1, 0);
                if ((u32)(state - ACTION_STATE_SPIN) > 1)
                    StartSpin();
                else
                    SetMode(ACTION_STATE_SPIN);
            } else if (INPUT_HELD(in) & R_BUTTON) {
                ActOrFlags0D(part, 1);
                slamBlocked = alt;
                SetMode(ACTION_STATE_CROUCH_DOWN);
                SetTargetAnim(part, 3);
                frames = alt;
                QueueX(alt, 1, alt);
            }
        }
    }
    {
        u8 dir = GetDpadDirection(gInput);

        if (dir == 0) {
            motionXKeepSpeed = dir;
            motionXPending = 1;
            motionX = dir;
        }
    }
}

/* Dying: the death animation 0x2F's sound on its frame 3; once the
 * animation is done, the player is marked gone. */
void ActionCtrl::StateDying()
{
    Player *p = part;

    if (p->tag == 0x2F && p->frame == 3 && p->stepTimer == 0)
        PlaySfx(gAudioContext, SFX_UNKNOWN_2E, 0x100);
    p = part;
    if (p->animDone)
        p->MarkGone();
}

/* Warping in: once the animation is done, the player's collision is
 * switched on, idle, and the player's palette reloaded. */
void ActionCtrl::StateWarpIn()
{
    if (part->animDone) {
        gPlayer->f.b.collides = 1;
        SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, 0);
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = 0;
        LoadPaletteSlot(gPaletteCache, part->palette, part->anim->records[part->tag].paletteId);
    }
}

/* Hanging: the D-pad sideways starts moving along (animation 0x20, X
 * entry 0x20); A drops (ReleaseHang), B spins (StartHangSpin). Then the
 * facing. */
void ActionCtrl::StateHang()
{
    u8 dir = GetDpadDirection(gInput);
    u32 in = gKeys.all;

    if (dir != 0)
        switch (dir) {
        case 3 ... 8:
            QueueNowX(0x20);
            SetMode(ACTION_STATE_HANG_MOVE_START);
            SetTargetAnim(part, 0x20);
            break;
        }
    if (INPUT_PRESSED(in) & 1) {
        PlaySfx(gAudioContext, SFX_JUMP, 0x100);
        ReleaseHang();
        return;
    }
    if (INPUT_PRESSED(in) & 2) {
        StartHangSpin();
        UpdateFacing();
    } else {
        UpdateFacing();
    }
}

/* gActionCtrlStateTable's slot 0x22, a state nothing sets: StateHang
 * without the D-pad move (A drops, B starts the hang spin). */
void ActionCtrl::StateUnusedHang()
{
    u32 in = gKeys.all;
    s32 fire = INPUT_PRESSED(in) & 1;

    if (fire) {
        PlaySfx(gAudioContext, SFX_JUMP, 0x100);
        ReleaseHang();
        return;
    }
    if (INPUT_PRESSED(in) & 2) {
        StartHangSpin();
        UpdateFacing();
        motionXKeepSpeed = fire;
        motionXPending = 1;
        motionX = fire;
    }
}

/* Letting go: the player drops 6 px (0x600 in Q8) into the fall
 * (animation 0x1B, held on its last frame), Y entry 4.
 *
 * Kept from the C: reload loads the 0x600 into a spill register, and the
 * ROM's is r3. With r2 and r3 both free it takes r2, and every later
 * reload then rotates through {1,2,6} instead of the ROM's {1,3,6}.
 * `hold` keeps r2 live across the add, with empty asms only (no code). */
void ActionCtrl::ReleaseHang()
{
    Player *p;
    s32 count;
    MATCH_HOLD_REG(s32, hold, r2);

    part->hanging = 0;
    MATCH_HOLD(hold); /* r2 live from here: no code */
    part->y += 0x600;
    MATCH_USE(hold); /* ...to here, so the 0x600 reload takes r3 */
    SetMode(ACTION_STATE_AIRBORNE_FALL);
    SetTargetAnim(part, 0x1B);
    p = part;
    count = p->bank->anims[p->tag].frameCount;
    p->frame = count - 1;
    QueueNowY(4);
}

/* Starting to move along while hanging: A drops, B spins; the D-pad idle
 * stops (animation 0x22); sideways queues X entry 0x20, and once the
 * start animation is done the move itself (animation 0x21 from frame
 * 5). Then the facing. */
void ActionCtrl::StateHangMoveStart()
{
    void *pad = gInput;
    u32 in = gKeys.all;
    s32 v = INPUT_PRESSED(in) & 1;

    if (v) {
        PlaySfx(gAudioContext, SFX_JUMP, 0x100);
        QueuePendingX(0, 0);
        ReleaseHang();
        return;
    }
    if (INPUT_PRESSED(in) & 2) {
        StartHangSpin();
        UpdateFacing();
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        return;
    }
    v = GetDpadDirection(pad);
    if (v == 0) {
        SetMode(ACTION_STATE_HANG_STOP);
        SetTargetAnim(part, 0x22);
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        return;
    }
    {
        u8 cur = motionX;

        if (cur == 0) {
            switch (v) {
            case 3 ... 8:
                QueuePendingX(cur, 0x20);
            }
        }
        if (part->animDone) {
            Player *p;
            s32 zero = 0;
            s32 frame;
            s32 count;

            SetMode(ACTION_STATE_HANG_MOVE);
            SetTargetAnim(part, 0x21);
            this->frame = zero;
            p = part;
            frame = 5;
            count = p->bank->anims[p->tag].frameCount;
            CLAMP_INDEX(frame, count);
            p->frame = frame;
            QueuePendingX(zero, 0x20);
        }
    }
    UpdateFacing();
}

/* Moving along while hanging (animation 0x21, restarted when done): A
 * drops, B spins. With the D-pad idle, every 4 frames the stop animation
 * (0x22 or 0x23, by the move's frame) unless mid-step; sideways keeps X
 * entry 0x20. While the facing reports a step, the player moves by the
 * frame's anchor X (mirrored). */
void ActionCtrl::StateHangMove()
{
    u8 dir = GetDpadDirection(gInput);
    u32 in = gKeys.all;
    Player *p = part;
    s32 fire;
    u16 alt;

    if (p->animDone)
        SetTag(p, 0x21);
    fire = INPUT_PRESSED(in) & 1;
    if (fire) {
        PlaySfx(gAudioContext, SFX_JUMP, 0x100);
        QueuePendingX(0, 0);
        ReleaseHang();
        return;
    }
    alt = INPUT_PRESSED(in) & 2;
    if (alt) {
        StartHangSpin();
        UpdateFacing();
        motionXKeepSpeed = fire;
        motionXPending = 1;
        motionX = fire;
        return;
    }
    if (dir == 0) {
        if (++frame > 3) {
            s32 f;

            frame = alt;
            f = part->frame;
            if (f == 0) {
                SetMode(ACTION_STATE_HANG_STOP);
                SetTargetAnim(part, 0x22);
                motionXKeepSpeed = alt;
                motionXPending = 1;
                motionX = alt;
            } else if (f <= 4) {
                SetMode(ACTION_STATE_HANG_STOP);
                SetTargetAnim(part, 0x23);
                motionXKeepSpeed = alt;
                motionXPending = 1;
                motionX = alt;
            } else if (f > 9) {
                SetMode(ACTION_STATE_HANG_STOP);
                SetTargetAnim(part, 0x22);
                motionXKeepSpeed = alt;
                motionXPending = 1;
                motionX = alt;
            } else {
                motionXKeepSpeed = alt;
                motionXPending = 1;
                motionX = alt;
            }
        }
    } else {
        frame = alt;
        QueuePendingX(alt, 0x20);
    }
    if ((u8)UpdateFacing()) {
        const struct sprite_frame *sf = part->GetFrame();
        const struct sprite_point *info;
        s32 x;
        s32 y;

        switch (sf->pieces[0] >> 4) {
        case 0:
            info = &((const struct sprite_frame_3box_anchor *)sf)->anchor;
            break;
        case 1:
            info = &gEmptySpritePoint;
            break;
        case 2:
            info = &gEmptySpritePoint;
            break;
        case 3:
            info = &gEmptySpritePoint;
            break;
        case 4:
            info = &gEmptySpritePoint;
            break;
        case 5:
            info = &gEmptySpritePoint;
            break;
        case 6:
            info = &((const struct sprite_frame_1box_anchor *)sf)->anchor;
            break;
        default:
            info = &gEmptySpritePoint;
            break;
        }
        x = Q8_TO_INT(part->x);
        y = part->y;
        if ((s8)(part->mirror << 3) < 0)
            x += info->x;
        else
            x -= info->x;
        part->x = INT_TO_Q8(x);
        part->y = y;
    }
}

/* Stopping while hanging: A drops, B spins; once the animation is done,
 * hanging (animation 0x1F). */
void ActionCtrl::StateHangStop()
{
    u32 in = gKeys.all;
    s32 fire = INPUT_PRESSED(in) & 1;
    u16 alt;

    if (fire) {
        PlaySfx(gAudioContext, SFX_JUMP, 0x100);
        QueueNowX(0);
        ReleaseHang();
        return;
    }
    alt = INPUT_PRESSED(in) & 2;
    if (alt) {
        StartHangSpin();
        UpdateFacing();
        motionXKeepSpeed = fire;
        motionXPending = 1;
        motionX = fire;
        return;
    }
    if (part->animDone) {
        SetMode(ACTION_STATE_HANG);
        SetTargetAnim(part, 0x1F);
        frame = alt;
        frames = alt;
    }
}

/* gCollidableList's length. As an inline function (a block of its own),
 * the loop's exit test isn't copied in front of the loop, so the loop
 * body reloads the list's address as the ROM does. */
static inline s32 CollidableCount()
{
    return gCollidableList->count;
}

/* The super body slam's landing: breaks the crates within 0x40 px
 * (BreakCratesInArea), then sends event 0x16 (EVENT_ATTACK_SUPER_BODY_SLAM)
 * to each object of gCollidableList with a class id above 4, within 0x40
 * px (Manhattan distance) and 0x11 px vertically, that can be hit
 * (`flags` bit 6). */
void ActionCtrl::DoSuperBodySlamShockwave()
{
    Player *p = part;
    s32 range;
    s32 px;
    s32 py;
    s32 i;

    BreakCratesInArea(Q8_TO_INT(p->x), Q8_TO_INT(p->y), 0x40, 0x12);
    range = 0x40;
    p = part;
    px = Q8_TO_INT(p->x);
    py = Q8_TO_INT(p->y);
    i = 0;
    while (i < CollidableCount()) {
        MovingSprite *other = (MovingSprite *)gCollidableList->items[i];
        s32 dx;
        s32 dy;

        if (other->GetClassId() > 4) {
            dx = ABS_BRANCHLESS(Q8_TO_INT(other->x) - px);
            dy = ABS_BRANCHLESS(Q8_TO_INT(other->y) - py);
            if (dx + dy <= range && ((other->f.flags >> 6) & 1) != 0 && dy <= 0x11)
                other->HandleEvent(0, EVENT_ATTACK_SUPER_BODY_SLAM, 0);
        }
        i++;
    }
}

/* The tornado spin's next turn (from the ground, air and tornado spins).
 * Winding up (`tornadoUnwinding` clear): enters state `id` with the turn's
 * animation (0x17, 0x28, 0x27 for turns 0-2) for 0x14 frames and its
 * sound; once `tornadoTurn` reaches `charge`, the turns count back down.
 * Unwinding: the same, counting down, until the count wraps; then state
 * `param2` with the spin animation (0x10, 0x18 frames), and the spin
 * cooldown at 0x63. */
void ActionCtrl::StartTornadoSpin(s32 id, s32 param2)
{
    if (tornadoUnwinding == 0) {
        s32 idx = 0x17;
        s32 zero;
        s32 wait;

        tornadoVariant = 0;
        if (tornadoTurn == 1) {
            idx = 0x28;
            tornadoVariant = 1;
        } else if (tornadoTurn == 2) {
            idx = 0x27;
            tornadoVariant = 2;
        }
        zero = 0;
        wait = 0x14;
        SetMode(id);
        SetTargetAnim(part, idx);
        frame = zero;
        frames = wait;
        PlaySfx(gAudioContext, tornadoVariant + SFX_TORNADO_SPIN, 0x100);
        if (++tornadoTurn >= charge) {
            tornadoUnwinding = 1;
            if (tornadoTurn > 1)
                tornadoTurn = 1;
            else
                tornadoTurn = zero;
        }
    } else {
        if (tornadoTurn > 0xf0) {
            s32 idx;
            s32 zero;
            s32 wait;

            idx = 0x17;
            tornadoVariant = 0;
            if (tornadoTurn == 1) {
                idx = 0x28;
                tornadoVariant = 1;
            } else if (tornadoTurn == 2) {
                idx = 0x27;
                tornadoVariant = 2;
            }
            zero = 0;
            wait = 0x14;
            SetMode(id);
            SetTargetAnim(part, idx);
            frame = zero;
            frames = wait;
            PlaySfx(gAudioContext, tornadoVariant + SFX_TORNADO_SPIN, 0x100);
        } else {
            u8 *p21 = &tornadoVariant;
            s32 zero = 0;
            s32 wait;

            *p21 = zero;
            charge = zero;
            wait = 0x18;
            SetMode(param2);
            SetTargetAnim(part, 0x10);
            frame = zero;
            frames = wait;
            PlaySfx(gAudioContext, SFX_SPIN, 0x100);
            spinCooldown = 0x63;
        }
        tornadoTurn--;
    }
    tornadoFallQueued = 0;
}
