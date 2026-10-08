#include "action_ctrl.hpp"
#include "spawners.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "system.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
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
    void *pad = gInput;
    u32 in = gKeys.all;
    u8 contact = part->hitAxes;
    u8 dir = GetDpadDirection(pad);
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
            u8 dir = GetDpadDirection(gInput);

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
        void *pad = gInput;

        in = gKeys.all;
        dir = GetDpadDirection(pad);
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
        void *pad = gInput;

        in = gKeys.all;
        dir = GetDpadDirection(pad);
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
        void *pad = gInput;

        in = gKeys.all;
        dir = GetDpadDirection(pad);
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
             * ROM (a plain pointer lands in a fresh register) */
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
        switch (GetDpadDirection(gInput)) {
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
    dir = GetDpadDirection(gInput);
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
