#include "action_ctrl.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "spawners.hpp"

extern "C" {
#include "util.h"
#include "system.h"
#include "level.h"
#include "globals.h"
#include "match.h"
#include "math_util.h"
}

/* ActionCtrl's CheckLeftGround (include/action_ctrl.hpp; #664,
 * docs/cplusplus.md). Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS):
 * the C, built by agbcc, pinned 3 registers to old_agbcc's code. */

/* Whether the player has left the ground (`hitAxes` bit 3, the floor,
 * clear): then the fall (state 0x1A, animation 0x1B) after more than two
 * probe tries, or state 0x1C (just left the ground) before, with the
 * falling Y motion (entry 4) queued. */
u8 ActionCtrl::CheckLeftGround()
{
    if ((part->hitAxes & 8) == 0) {
        if (part->probeTries > 2) {
            SetMode(ACTION_STATE_AIRBORNE_FALL);
            SetTargetAnim(part, 0x1B);
        } else {
            SetMode(ACTION_STATE_LEFT_GROUND);
        }
        QueueNowY(4);
        return 1;
    }
    return 0;
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
    Player *p;

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
        gPlayer->SetPrevPos(gPlayer->x, gPlayer->y);
    }
skip:
    if (state == ACTION_STATE_IDLE) {
        Player *p = gPlayer;

        if (p->speedX == 0 && p->bank->animCount != 0x12 && idleFidget == 0) {
            gAudioContext->StopSfx(SFX_SKID);
            SetTargetAnim(gPlayer, 0x12);
        }
    }
    if (state == ACTION_STATE_IDLE || state == ACTION_STATE_CROUCH) {
        Player *p = gPlayer;

        if (p->speedX != 0 && p->slippery == 0)
            QueueNowX(0);
    }
    {
        u8 pending = motionXPending;

        if (pending == 1) {
            Player *q;

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
                SetTargetMotionX(part, &rec.start);
            else
                StartTargetMotionX(part, &rec.start);
            motionXPending = 0;
        }
    }
    if (motionYPending == 1) {
        rec = gCtrlMotionRecords[animSet->entries[motionY][1]];
        if (motionYKeepSpeed)
            SetTargetMotionY(part, &rec);
        else
            StartTargetMotionY(part, &rec);
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
    KeyInput *pad = gInput;
    u32 in = gKeys.all;
    u8 dir = pad->GetDpadDirection();
    s32 count;
    Player *p;

    if (dpadLockTimer != 0) {
        dpadLockTimer--;
        if (dir == 0)
            dpadLockTimer = dir;
    }
    p = part;
    if (p->animDone) {
        SetTargetAnim(p, 0x12);
        idleFidget = 0;
    }
    count = ++frames;
    p = part;
    if (p->tag == 0x12 && p->frame == 0) {
        if (count > 0x708) {
            SetTargetAnim(p, 0x1A);
            frames = 0;
        } else if (count >= 0x49D && count <= 0x4C3) {
            SetTargetAnim(p, 5);
            frames = 0x4C4;
        } else if (count >= 0x1E1 && count <= 0x207) {
            SetTargetAnim(p, 0xE);
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
            gAudioContext->PlaySfx(SFX_JUMP, 0x100);
            SetMode(ACTION_STATE_JUMP);
            SetTargetAnim(part, 0x13);
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
                SetTargetAnim(part, 3);
                frames = alt;
            }
        }
        UpdateFacing();
        return;
    other:
        turboRun = held;
        if (dir == 0) {
            Player *q = part;

            if (IsSlippery(q) && motionX != 0x1F && q->speedX != 0)
                QueueNowXKeepSpeed(0x1F);
        } else {
            u8 wait = dpadLockTimer;

            if (wait == 0) {
                switch (dir) {
                case 3 ... 8:
                    if ((INPUT_HELD(in) & L_BUTTON) && (u8)gLevelState->HasTurboRun()) {
                        turboRun = 1;
                        SetMode(ACTION_STATE_TURBO_RUN);
                        SetTargetAnim(part, 0x18);
                        QueueX(wait, 1, 0x1B);
                    } else {
                        StartRun();
                    }
                    break;
                case 2:
                    SetMode(ACTION_STATE_CROUCH_DOWN);
                    SetTargetAnim(part, 3);
                    frames = wait;
                    break;
                }
            }
        }
        UpdateFacing();
    }
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
    KeyInput **pad = &gInput;
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
        u8 dir = (*pad)->GetDpadDirection();

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
                         * from the A test's own 1, which plain C++ shares.
                         * A byte test (`(u8)cur & 1` or a `u8` flag) keeps
                         * the two 1s apart, as in HandleEvent's launch
                         * pad, but its AND lands in r0 instead of r2
                         * (the else's 0 store then shifts registers);
                         * the permuter only found a shared hoisted 1.
                         * The mechanism is ActionCtrl::HandleEvent's (see
                         * its bounce cases): cse1 gives the arms' 1 the
                         * test's pseudo, and regmove copies it (#662
                         * round 3). Round 4 traced the condition the ROM
                         * implies to cse's quantities and local-alloc's
                         * update_equiv_regs (see HandleEvent's bounce).
                         * Round 5: the test through an inline (`Held`,
                         * `HeldB(cur, A_BUTTON)`, the bool one keeping
                         * `cur` in r4) or the whole queueing as an
                         * inline with the A flag or keys as a parameter
                         * still share the 1, and a cse that doesn't
                         * link equal constants changes other states. */
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
    if (gInput->GetDpadDirection() <= 2) {
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

/* ActionCtrl's state methods in the air, spinning, sliding, crouching and
 * crawling (include/action_ctrl.hpp; #664, docs/cplusplus.md). Built by
 * old_agbcp (the Makefile's OLD_AGBCC_OBJS), like the old_agbcc C it
 * replaces. */

/* Byte masks with the mask as an `s32` parameter: the AND stays in SImode
 * (a plain `*p & -0x11` is narrowed to 0xEF), so the -0x11 the ROM derives
 * from the 1 already in r7 (`subs r7, #0x12`) can be shared by both
 * sparks. */
static inline void AndByte(u8 *p, s32 clear)
{
    *p &= clear;
}

static inline void OrMaskByte(u8 *p, s32 clear, s32 set)
{
    *p = (*p & clear) | set;
}

/* A spark (an effect part). The coordinates are inline parameters, so they
 * are computed before the pool pointer is loaded, as in the ROM. */
static inline MovingSprite *SpawnSpark(s32 x, s32 y, s32 mirror)
{
    return gEntitySpawner->SpawnEffectPart(0x29, 1, x, y, mirror);
}

/* The airborne states (7, 9, 0xB, 0x18, 0x19 and 0x1A share it). B
 * starts the air spin (as in StateJump) unless the spin cooldown runs or
 * the player is body slamming. Out of contact: the facing, then the
 * D-pad lock countdown or the air input (HandleAirInput), and in the fall
 * Y entry 4. Landing on a ceiling (`hitAxes` bit 2): the fall (animation
 * 0x15 on frame 2, the Y speed cleared). On the ground (bit 3) while
 * falling: a body slam lands with two sparks at the player (+-0x14 px,
 * the first mirrored), the super body slam's shockwave and animation
 * 0x11; the air spin goes on as the ground or tornado spin; anything
 * else lands (animation 0x16). */
void ActionCtrl::StateAirborne()
{
    KeyInput *pad = gInput;
    u32 in = gKeys.all;
    u8 contact = part->hitAxes;
    u8 dir = pad->GetDpadDirection();
    s32 state = this->state;

    if (state != ACTION_STATE_AIR_SPIN) {
        u8 busy = spinCooldown;

        if (busy == 0 && (INPUT_PRESSED(in) & 2) &&
            (u32)(state - ACTION_STATE_AIRBORNE_BODY_SLAM) > 1) {
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
        }
    }
    if (contact == 0) {
        u8 *timer;

        UpdateFacing();
        timer = &dpadLockTimer;
        if (*timer != 0) {
            (*timer)--;
            if (dir == 0)
                *timer = contact;
            return;
        }
        HandleAirInput();
        if (this->state == ACTION_STATE_AIRBORNE_FALL) {
            u8 *slot = &motionY;

            if (*slot == 0)
                QueueNowYAt(slot, 4);
        }
        return;
    }
    {
        u8 bit4 = contact & 4;

        if (bit4) {
            if (this->state != ACTION_STATE_AIRBORNE_FALL) {
                ActOrFlags0D(part, 1);
                slamBlocked = 0;
                if (this->state != ACTION_STATE_AIR_SPIN) {
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
                }
                part->ClearSpeedY();
            }
            UpdateFacing();
            HandleAirInput();
            part->StoreHitAxes(0);
            return;
        }
        if (contact == 1 || contact == 2) {
            UpdateFacing();
            HandleAirInput();
            part->hitAxes = bit4;
            return;
        }
        if ((contact & 8) && part->speedY >= 0) {
            s32 st;

            part->f.bytes.flags2 |= 1;
            slamBlocked = bit4;
            st = this->state;
            if ((u32)(st - ACTION_STATE_AIRBORNE_BODY_SLAM) <= 1) {
                MovingSprite *obj;
                s32 x;
                s32 y;
                s32 frame;
                s32 count;

                x = gPlayer->x;
                Q8_TO_INT_INPLACE(x);
                x += 0x14;
                y = gPlayer->y;
                Q8_TO_INT_INPLACE(y);
                y += 0xC;
                obj = SpawnSpark(x, y, 1);
                obj->mirrorBits.gfxMode = 1;
                obj->f.b.visible = 0;
                obj->f.b.visible = 0;
                OrMaskByte(&obj->mirror, -0x11, 0x10);
                frame = 3;
                count = obj->bank->anims[obj->tag].frameCount;
                CLAMP_INDEX(frame, count);
                obj->frame = frame;

                x = gPlayer->x;
                Q8_TO_INT_INPLACE(x);
                x -= 0x14;
                y = gPlayer->y;
                Q8_TO_INT_INPLACE(y);
                y += 0xC;
                obj = SpawnSpark(x, y, bit4);
                obj->mirrorBits.gfxMode = 1;
                obj->f.b.visible = 0;
                AndByte(&obj->mirror, -0x11);
                frame = 3;
                count = obj->bank->anims[obj->tag].frameCount;
                CLAMP_INDEX(frame, count);
                obj->frame = frame;

                if (this->state == ACTION_STATE_AIRBORNE_SUPER_BODY_SLAM)
                    DoSuperBodySlamShockwave();
                if (this->state != ACTION_STATE_DYING) {
                    gAudioContext->PlaySfx(SFX_BODY_SLAM_LAND, 0x100);
                    SetMode(ACTION_STATE_BODY_SLAM_LAND);
                    SetTargetAnim(part, 0x11);
                    motionYKeepSpeed = bit4;
                    motionYPending = 1;
                    motionY = bit4;
                }
                return;
            }
            if (st == ACTION_STATE_AIR_SPIN) {
                u16 held = INPUT_HELD(in) & DPAD_SIDEWAYS;

                if (held) {
                    QueueX(bit4, 1, 1);
                } else {
                    motionXKeepSpeed = held;
                    motionXPending = 1;
                    motionX = held;
                }
                QueueNowY(0);
                if (tornadoTurn)
                    SetMode(ACTION_STATE_TORNADO_SPIN);
                else
                    SetMode(ACTION_STATE_SPIN);
                return;
            }
            if (gPlayer->slippery) {
                SetMode(ACTION_STATE_LAND);
                SetTargetAnim(part, 0x16);
            } else {
                SetMode(ACTION_STATE_LAND);
                SetTargetAnim(part, 0x16);
            }
            QueuePendingX(0, 0);
            QueueNowY(0);
            return;
        }
    }
    QueuePendingX(0, 0);
}

/* The flip body slam's start: on the flip jump's animation (6), animation
 * 9 on its frame 3, 8 past it (or once done); then, once the animation is
 * done, the super body slam (animation 7) if HasSuperBodySlam allows it,
 * else the body slam. */
void ActionCtrl::StateFlipBodySlamStart()
{
    Player *p = part;

    if (p->tag == 6) {
        s32 frame = p->frame;

        if (frame == 3)
            SetTargetAnim(p, 9);
        else if (frame > 3 || p->animDone)
            SetTargetAnim(p, 8);
    } else if (p->animDone) {
        if ((u8)gLevelState->HasSuperBodySlam()) {
            SetMode(ACTION_STATE_AIRBORNE_SUPER_BODY_SLAM);
            SetTargetAnim(part, 7);
        } else {
            SetMode(ACTION_STATE_AIRBORNE_BODY_SLAM);
        }
    }
}

/* Sliding: off the ground queues Y entry 5; A held jumps high and B
 * spins, when there is room for the animation (PlayerHasRoomForAnim).
 * For `frames` frames the slide holds the animation on frame 3; then,
 * once it is done: off the ground the fall (animation 0x1B, Y entry 4);
 * R held crawls; the D-pad sideways runs (the turbo run with L held and
 * HasTurboRun), another direction stands up; with the D-pad idle, stands
 * up if there is room, else crouches. */
void ActionCtrl::StateSlide()
{
    u32 in = gKeys.all;

    {
        Player *p = part;

        if (p->hitAxes == 0) {
            QueueNowY(5);
        } else if (INPUT_HELD(in) & 1) {
            if (p->HasRoomForAnim(0xB) == 1) {
                gAudioContext->PlaySfx(SFX_HIGH_JUMP, 0x100);
                ActAndFlags0D(part, -2);
                ActAndFlags0D(part, -3);
                StartHighJump();
                return;
            }
        } else if (INPUT_PRESSED(in) & 2) {
            if (p->HasRoomForAnim(0x10) == 1) {
                StartSpin();
                QueueX(0, 1, 1);
                return;
            }
        }
    }

    if (++frame < frames) {
        Player *p = part;
        s32 frame;
        s32 count;

        p->stepTimer = 0;
        frame = 3;
        count = p->bank->anims[p->tag].frameCount;
        CLAMP_INDEX(frame, count);
        p->frame = frame;
        return;
    }
    {
        Player *p = part;
        u8 contact;

        if (!p->animDone)
            return;
        contact = p->hitAxes;
        if (contact == 0) {
            SetMode(ACTION_STATE_AIRBORNE_FALL);
            SetTargetAnim(part, 0x1B);
            QueueNowY(4);
            return;
        }
    }
    {
        u16 held = INPUT_HELD(in) & R_BUTTON;

        if (held) {
            s32 zero = 0;

            SetMode(ACTION_STATE_CRAWL);
            SetTargetAnim(part, 0);
            frames = zero;
            QueuePendingX(zero, 3);
            StateCrawl();
            return;
        }
        {
            u8 dir = gInput->GetDpadDirection();

            if (dir != 0 && part->HasRoomForAnim(2)) {
                switch (dir) {
                case 3 ... 4:
                    if ((INPUT_HELD(in) & L_BUTTON) && (u8)gLevelState->HasTurboRun()) {
                        turboRun = 1;
                        SetMode(ACTION_STATE_TURBO_RUN);
                        SetTargetAnim(part, 0x18);
                        QueueX(held, 1, 0x1B);
                        return;
                    }
                    StartRun();
                    return;
                }
                SetMode(ACTION_STATE_STAND_UP);
                SetTargetAnim(part, 2);
                QueuePendingX(0, 0);
                return;
            } else {
                u8 hit = part->HasRoomForAnim(2);

                if (hit == 1) {
                    SetMode(ACTION_STATE_STAND_UP);
                    SetTargetAnim(part, 2);
                    QueueX(0, hit, 0);
                    return;
                }
                SetMode(ACTION_STATE_CROUCH);
                SetTargetAnim(part, 4);
                QueuePendingX(0, 0);
            }
        }
    }
}

/* The ground spin: off the ground queues Y entry 5; A on the ground jumps
 * into the air spin (Y entry 7). With HasTornadoSpin, B adds a tornado
 * turn (`charge`, up to 3) unless the spin cooldown runs. The D-pad
 * steers (SteerSpin); at the end of the spin, the tornado spin if turns
 * were charged, else EndSpin. */
void ActionCtrl::StateSpin()
{
    u32 in;
    u8 dir;

    {
        KeyInput *pad = gInput;

        in = gKeys.all;
        dir = pad->GetDpadDirection();
    }
    if (part->hitAxes == 0)
        QueueNowY(5);
    if ((INPUT_PRESSED(in) & 1) && (part->hitAxes & 8)) {
        ActAndFlags0D(part, -2);
        ActAndFlags0D(part, -3);
        SetMode(ACTION_STATE_AIR_SPIN);
        QueueNowY(7);
        tornadoTurn = 0;
        tornadoFallQueued = 0;
        part->hitAxes = 0;
        return;
    }
    if ((u8)gLevelState->HasTornadoSpin() && (INPUT_PRESSED(in) & 2) && spinCooldown == 0) {
        if (++charge > 3)
            charge = 3;
    }
    SteerSpin(dir);
    if (++frame >= frames || part->animDone) {
        if (charge)
            StartTornadoSpin(ACTION_STATE_TORNADO_SPIN, ACTION_STATE_SPIN);
        else
            EndSpin(dir, in);
    }
}

/* The air spin: landing (`hitAxes` bit 3, falling) goes on as the ground
 * spin. B charges tornado turns as in StateSpin. At the end of the spin
 * the tornado spin if turns were charged, else the fall (animation 0x15)
 * with the spin cooldown started. Then the airborne state's method. */
void ActionCtrl::StateAirSpin()
{
    u32 in;
    Player *p;

    in = gKeys.all;
    p = part;

    if ((p->hitAxes & 8) && p->speedY > 0) {
        /* flags2 through a pointer to it: ActOrFlags0D's 1 is
         * reused for motionYPending after the call, and `p->flags2 |= 1`
         * schedules slamBlocked's 0 into the `ldrb`'s slot. */
        {
            u8 *flags2 = &p->f.bytes.flags2;

            *flags2 |= 1;
        }
        slamBlocked = 0;
        SetMode(ACTION_STATE_SPIN);
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = 0;
        StateSpin();
        return;
    }
    if ((u8)gLevelState->HasTornadoSpin() && (INPUT_PRESSED(in) & 2) && spinCooldown == 0) {
        if (++charge > 3)
            charge = 3;
    }
    {
        s32 frame = ++this->frame;
        s32 frames = this->frames;

        p = part;
        if (frame >= frames || p->animDone) {
            u8 charge;

            ActOrFlags0D(p, 1);
            charge = this->charge;
            if (charge) {
                StartTornadoSpin(ACTION_STATE_AIR_SPIN, ACTION_STATE_AIR_SPIN);
            } else {
                spinCooldown = 0xC;
                SetMode(ACTION_STATE_AIRBORNE_FALL);
                SetTargetAnim(part, 0x15);
                this->frame = charge;
                this->frames = charge;
            }
        }
    }
    StateAirborne();
}

/* The tornado spin: as StateSpin, but off the ground the tornado fall
 * (StartTornadoFall) once a turn was played, and every spin's end plays
 * the next turn (StartTornadoSpin). */
void ActionCtrl::StateTornadoSpin()
{
    u32 in;
    u8 dir;

    {
        KeyInput *pad = gInput;

        in = gKeys.all;
        dir = pad->GetDpadDirection();
    }
    if (part->hitAxes == 0) {
        if (tornadoTurn)
            StartTornadoFall();
        else
            QueueNowY(5);
    }
    if ((INPUT_PRESSED(in) & 1) && (part->hitAxes & 8)) {
        ActAndFlags0D(part, -2);
        ActAndFlags0D(part, -3);
        SetMode(ACTION_STATE_AIR_SPIN);
        QueueNowY(7);
        tornadoFallQueued = 0;
        part->hitAxes = 0;
        return;
    }
    if ((u8)gLevelState->HasTornadoSpin() && (INPUT_PRESSED(in) & 2) && spinCooldown == 0) {
        if (++charge > 3)
            charge = 3;
    }
    SteerSpin(dir);
    if (++frame >= frames || part->animDone)
        StartTornadoSpin(ACTION_STATE_TORNADO_SPIN, ACTION_STATE_SPIN);
}

/* Crouching down: A jumps high. On the ground, a queued Y entry 4 or 5
 * is cancelled. Once the animation is done, crouching (animation 4). */
void ActionCtrl::StateCrouchDown()
{
    u32 in;
    s32 fire;

    in = gKeys.all;
    fire = INPUT_PRESSED(in) & 1;

    if (fire) {
        gAudioContext->PlaySfx(SFX_HIGH_JUMP, 0x100);
        ActAndFlags0D(part, -2);
        ActAndFlags0D(part, -3);
        StartHighJump();
        return;
    }
    if (part->hitAxes == 8 && (u8)(motionY - 4) <= 1) {
        motionYKeepSpeed = fire;
        motionYPending = 1;
        motionY = fire;
    }
    if (part->animDone) {
        SetMode(ACTION_STATE_CROUCH);
        SetTargetAnim(part, 4);
    }
}

/* Crouching: A jumps high if there is room. Unless the player left the
 * ground (CheckLeftGround): the D-pad turns the player to face it; if he
 * didn't turn, the D-pad sideways starts the crawl (animation 0x14, X
 * entry 3). With neither down nor R held, stands up if there is room,
 * or else (not crawling) stays crouched with both motions cleared. */
void ActionCtrl::StateCrouch()
{
    u32 in;
    u8 dir;
    s32 turned;
    s32 moved;

    {
        KeyInput *pad = gInput;

        in = gKeys.all;
        dir = pad->GetDpadDirection();
    }
    if ((INPUT_PRESSED(in) & 1) && part->HasRoomForAnim(0xB) == 1) {
        gAudioContext->PlaySfx(SFX_HIGH_JUMP, 0x100);
        ActAndFlags0D(part, -2);
        ActAndFlags0D(part, -3);
        StartHighJump();
        return;
    }
    if (CheckLeftGround())
        return;

    turned = 0;
    if ((s32)(part->mirror << 27) < 0 && (dir == 4 || dir == 6 || dir == 8)) {
        u8 *p = &part->mirror;
        s32 m = -0x11;

        m &= *p;
        *p = m;
        motionXPending = 1;
        turned = 1;
        goto turn_done;
    }
    if ((s8)(part->mirror << 3) >= 0 && (dir == 3 || dir == 5 || dir == 7)) {
        s32 m;

        turned = 1;
        {
            /* volatile: keeps the `+0x28` address in the part copy's
             * register and computed ahead of the -0x11 mask, as in the
             * ROM (a plain pointer lands in a fresh register: the test
             * above already computed `part + 0x28`, and the pointer
             * becomes a copy of it). Set after the mask, the address is
             * the ROM's register but comes after the `movs; negs`; the
             * bitfield store `mirrorBits.flipX = 1` is a halfword longer
             * and `*p = (*p & -0x11) | 0x10` loads an 0xEF mask (#662
             * round 2). Round 3: the plain pointer is a block-local
             * pseudo, so local-alloc gives it r0, the first free
             * register; the ROM's `adds r2, #0x28` is the address tied
             * to the dying part copy (r2), as global-alloc's copy
             * preference would place it. Through `part->mirror` with no
             * pointer, the registers are the ROM's but reload forms the
             * address (ldrb's offset is 0-31) next to the `ldrb`, after
             * the mask. Round 4 (local-alloc.c, with an instrumented
             * copy printing each block's quantity order): written plainly
             * (or as `part->SetFlipX(1)`, which gives the ROM's insns),
             * the flip's block has exactly three local quantities: the
             * address (born first, q0), the mask (q1) and the 0x10 (q2);
             * combine folds the byte load into the AND as a subreg of the
             * MEM. block_alloc sorts three quantities with a hand-written
             * exchange on quantity numbers (qty_compare (0, 1), (1, 2),
             * (0, 1)), and with these priorities (0.30, 1.5, 1.0) the two
             * swaps cancel: the address goes first and takes r0. The ROM
             * order (mask r0, 0x10 r1, address r2) needs a fourth
             * quantity (qsort then sorts by priority; the volatile byte
             * load is one), or the address out of local-alloc, i.e. live
             * in two blocks, which code confined to this block can't be.
             * With the address born first (its `adds` comes first in the
             * ROM too), no priorities make the exchange put the mask
             * first: it yields address-first or 0x10-first. Round 5:
             * the flip as UpdateFacing's FaceRight-style inline (Player
             * or u8 * parameter, with or without the pending store), a
             * byte local for the load (`u8`/`s32`), the load first, an
             * `s32 bit = 0x10` and SetFlipX(1) all leave the same three
             * quantities; the whole turn as one inline taking `part`
             * makes the flip share the test's address instead. */
            volatile u8 *p = &part->mirror;

            m = -0x11;
            m &= *p;
            m |= 0x10;
            *p = m;
        }
        motionXPending = turned;
    }
turn_done:

    moved = 0;
    if (!turned) {
        switch (gInput->GetDpadDirection()) {
        case 3:
        case 4:
        case 7:
        case 8:
            SetMode(ACTION_STATE_CRAWL_START);
            SetTargetAnim(part, 0x14);
            QueuePendingX(moved, 3);
            moved = 1;
            break;
        }
    }

    {
        s32 held = INPUT_HELD(in) & (DPAD_DOWN | R_BUTTON);

        if (held == 0) {
            u8 hit = part->HasRoomForAnim(2);

            if (hit == 1) {
                SetMode(ACTION_STATE_STAND_UP);
                SetTargetAnim(part, 2);
                motionXKeepSpeed = held;
                motionXPending = hit;
                motionX = held;
            } else if (!moved) {
                SetMode(ACTION_STATE_CROUCH);
                SetTargetAnim(part, 4);
                motionXKeepSpeed = moved;
                motionXPending = 1;
                motionX = moved;
                motionYKeepSpeed = moved;
                motionYPending = 1;
                motionY = moved;
            }
        }
    }
}

/* Standing up: once the animation is done, idle. */
void ActionCtrl::StateStandUp()
{
    Player *p = part;

    if (p->animDone != 0) {
        SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, 0);
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = 0;
    }
}

/* Starting to crawl: A jumps high if there is room; once the animation
 * is done, crawling (animation 0), and the crawl state's method. */
void ActionCtrl::StateCrawlStart()
{
    u32 in = gKeys.all;

    if ((INPUT_PRESSED(in) & 1) != 0 && part->HasRoomForAnim(0xB) == 1) {
        gAudioContext->PlaySfx(SFX_HIGH_JUMP, 0x100);
        ActAndFlags0D(part, -2);
        ActAndFlags0D(part, -3);
        StartHighJump();
        return;
    }
    if (part->animDone != 0) {
        SetMode(ACTION_STATE_CRAWL);
        SetTargetAnim(part, 0);
        StateCrawl();
    }
}

/* Crawling: A jumps high if there is room. Unless the player left the
 * ground: the D-pad idle or down stops (animation 1); up stands up if
 * there is room, else crouches. With neither down nor R held, stands up
 * if there is room. Then the facing. */
void ActionCtrl::StateCrawl()
{
    u32 in = gKeys.all;
    u8 busy;
    u8 dir;
    u8 hit;
    u32 held;

    if ((INPUT_PRESSED(in) & 1) && part->HasRoomForAnim(0xB) == 1) {
        gAudioContext->PlaySfx(SFX_HIGH_JUMP, 0x100);
        ActAndFlags0D(part, -2);
        ActAndFlags0D(part, -3);
        StartHighJump();
        return;
    }
    busy = CheckLeftGround();
    if (busy != 0)
        return;
    dir = gInput->GetDpadDirection();
    switch (dir) {
    case 0:
    case 2:
        SetMode(ACTION_STATE_CRAWL_STOP);
        SetTargetAnim(part, 1);
        QueuePendingX(0, 0);
        break;
    case 1:
        if (part->HasRoomForAnim(2) == 1) {
            SetMode(ACTION_STATE_CRAWL_STAND_UP);
            SetTargetAnim(part, 2);
            QueueX(busy, dir, busy);
            break;
        }
        /* The do/while (no code) is a loop to gcc: it stops CSE from
         * following the jump into this block and giving its stores a copy
         * of `busy` and `dir` in other registers. With the same registers
         * in both branches, the ROM's cross-jump of the `bl` and the
         * stores follows. (The C's ACT_VCALL macros had the same loop.) */
        do {
            SetMode(ACTION_STATE_CROUCH);
        } while (0);
        SetTargetAnim(part, 4);
        QueueX(busy, dir, busy);
        break;
    }
    held = INPUT_HELD(in) & (DPAD_DOWN | R_BUTTON);
    if (held == 0 && (hit = part->HasRoomForAnim(2)) == 1) {
        SetMode(ACTION_STATE_STAND_UP);
        SetTargetAnim(part, 2);
        QueueX(held, hit, held);
    }
    UpdateFacing();
}
