#include "action_ctrl.hpp"
#include "spawners.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "match.h"
#include "system.h"
#include "level.h"
#include "globals.h"
}

/* ActionCtrl's run and jump states (include/action_ctrl.hpp; #664,
 * docs/cplusplus.md). Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS),
 * like the old_agbcc C it replaces. */

/* Running (states 3 and 4, the turbo run): unless the player left the
 * ground (CheckLeftGround), A jumps (animation 0x13, Y entry 7), B spins
 * and R slides (animation 0xF, X entry 0x1E, a dust effect part). Then
 * the D-pad: none goes idle, down crouches. L held in the plain run
 * starts the turbo run (HasTurboRun); releasing it in the turbo run goes
 * back to the plain one. */
void ActionCtrl::StateRun()
{
    void **pad = &gInput;
    u32 in = gKeys.all;
    u8 busy = CheckLeftGround();

    if (busy)
        return;
    if (INPUT_PRESSED(in) & 1) {
        gAudioContext->PlaySfx(SFX_JUMP, 0x100);
        SetMode(ACTION_STATE_JUMP);
        SetTargetAnim(part, 0x13);
        frame = busy;
        QueueNowY(7);
        return;
    }
    {
        u16 alt = INPUT_PRESSED(in) & 2;

        if (alt) {
            StartSpin();
            return;
        }
        if (INPUT_PRESSED(in) & R_BUTTON) {
            s32 frames;
            MovingSprite *obj;

            gAudioContext->PlaySfx(SFX_SLIDE, 0x100);
            frames = 0x10;
            SetMode(ACTION_STATE_SLIDE);
            SetTargetAnim(part, 0xF);
            frame = alt;
            this->frames = frames;
            QueueX(alt, 1, 0x1E);
            gPlayer->listCount = alt;
            gPlayer->listCount = alt;
            obj = gEntitySpawner->LaunchEffectPart(0x29, 1, 0, 0xA, alt, gPlayer);
            obj->f.b.visible = 0;
            obj->mirrorBits.gfxMode = 1;
        }
    }
    {
        u8 dir = GetDpadDirection(*pad);

        switch (dir) {
        case 0:
            SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, dir);
            QueueX(dir, 1, dir);
            QueueY(dir, 1, dir);
            QueueX(dir, 1, 0x1D);
            break;
        case 2:
        case 7:
        case 8:
            {
                s32 zero = 0;

                SetMode(ACTION_STATE_CROUCH_DOWN);
                SetTargetAnim(part, 3);
                frames = zero;
                QueuePendingX(zero, 0x1D);
            }
            break;
        }
    }
    {
        s32 held = (u16)(INPUT_HELD(in) & L_BUTTON);

        if (held) {
            if (state == ACTION_STATE_RUN && (u8)gLevelState->HasTurboRun()) {
                turboRun = 1;
                SetMode(ACTION_STATE_TURBO_RUN);
                SetTargetAnim(part, 0x18);
                QueueX(0, 1, 0x1B);
            }
        } else if (state == ACTION_STATE_TURBO_RUN) {
            turboRun = held;
            StartRun();
        } else if (frame != 0) {
            frame = held;
        }
    }
    UpdateFacing();
}

/* Jumping (state 5, the take-off): the player's `flags2` bits 0 and 1
 * cleared. Hitting a ceiling (`hitAxes` bit 2) falls (animation 0x15 on
 * frame 2, the Y speed cleared); B spins in the air unless the spin
 * cooldown runs. Once the take-off animation is done, A with the D-pad
 * sideways is the flip jump (animation 6), otherwise the jump's rise
 * (animation 0xC); a queued Y entry 7 becomes 0xA, 9 (A still held) or
 * 8. Then the D-pad steers: X entries 0, 0x1C on ice (after a turbo run),
 * 0xD (once `frame` counts a double jump) or 7. */
void ActionCtrl::StateJump()
{
    ActAndFlags0D(part, -2);
    ActAndFlags0D(part, -3);
    if (part->hitAxes & 4) {
        Player *p;
        s32 frame;
        s32 count;

        SetMode(ACTION_STATE_AIRBORNE_FALL);
        SetTargetAnim(part, 0x15);
        p = part;
        frame = 2;
        count = p->bank->anims[p->tag].frameCount;
        CLAMP_INDEX(frame, count);
        p->frame = frame;
        p->ClearSpeedY();
        part->StoreHitAxes(0);
        return;
    }
    {
        u32 in = gKeys.all;
        u8 busy = spinCooldown;

        if (busy == 0 && (INPUT_PRESSED(in) & 2)) {
            s32 frames;

            gAudioContext->PlaySfx(SFX_SPIN, 0x100);
            frames = 0x18;
            SetMode(ACTION_STATE_AIR_SPIN);
            SetTargetAnim(part, 0x10);
            frame = busy;
            this->frames = frames;
            tornadoVariant = busy;
            charge = busy;
            tornadoTurn = busy;
            tornadoFallQueued = busy;
            tornadoUnwinding = busy;
            gPlayer->bounce = busy;
            return;
        }
    }
    {
        Player *p = part;

        if (p->animDone) {
            u32 cur = gKeys.all;

            if ((cur & A_BUTTON) && (cur & DPAD_SIDEWAYS)) {
                if (p->tag == 6) {
                    SetMode(ACTION_STATE_AIRBORNE_FLIP_JUMP);
                } else {
                    SetMode(ACTION_STATE_AIRBORNE_FLIP_JUMP);
                    SetTargetAnim(part, 6);
                }
                if (motionY == 7)
                    QueueNowY(0xA);
            } else {
                SetMode(ACTION_STATE_AIRBORNE_JUMP);
                SetTargetAnim(part, 0xC);
                {
                    u8 *slot = &motionY;

                    if (*slot == 7) {
                        s32 one = 1;

                        /* Kept from the C: the ROM loads this 1 (r6) apart
                         * from the A test's own 1, which plain C++ shares. */
                        MATCH_KEEP(one);
                        if (cur & 1)
                            QueueYAt(slot, one, 9);
                        else
                            QueueYAt(slot, one, 8);
                    }
                }
            }
        }
    }
    if (GetDpadDirection(gInput) <= 2) {
        if (gPlayer->slippery == 0)
            QueueNowX(0);
    } else {
        if (motionX == 0x1B || motionX == 0x1C) {
            QueueNowXKeepSpeed(0x1C);
        } else if (frame != 0) {
            if (motionX != 0xD)
                QueueNowX(0xD);
        } else if (gPlayer->slippery == 0) {
            QueueNowX(7);
        }
    }
    UpdateFacing();
}
