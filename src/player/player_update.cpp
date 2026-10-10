#include "player.hpp"
#include "spawners.hpp"
#include "crate_list.hpp"
#include "hud.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "util.h"
#include "system.h"
#include "crates.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
#include "objects.h"
}

/* The player's (Player, include/player.hpp) object pass, event handler and
 * draw method. Built by old_agbcp (Makefile OLD_AGBCC_OBJS). */

/* The pass over the objects the player touches, once a frame (the
 * `cleared` latch skips it after the first). With the attack bit, the
 * attack box goes to the collidable list (g++ copies it into the
 * by-value argument with MemCopy32); while the player collides, the
 * collision queue is emptied, the crates and the touchables are tested
 * and the queued crate collisions resolved. */
void Player::TouchPlayer()
{
    u32 latch = cleared;

    if (latch != 0)
        return;

    if ((f.flags >> 1) & 1) {
        struct aabb box = GetAttackBox();

        CollidableList()->Collide(box, dir, this);
    }

    if (f.flags >> 7) {
        CollisionQueue *q = &collisionQueue;

        q->count = latch;
        q->posCommitted = latch;
        Crates()->CollidePlayer(3);
        TouchableList()->CollideClass(4);
        ResolvePlayerCollisions();
    }
}

static inline s32 IsBlinking(Player *p)
{
    s32 armed = 0;

    if (p->deadline > gRoomFrameCount)
        armed = 1;
    return armed;
}

/* The player's events (`event`, EVENT_*): the pickups' effects (the gems,
 * the crystal, the stopwatch, Aku Aku's mask), the room exits and warps,
 * the hits and the terrain's events. A hit with Aku Aku costs a mask and
 * starts a 90-frame invulnerability; without, it is a death. Most events
 * also go to the controller (`mover`), some after the Y ramp is cleared.
 *
 * The case bodies are in the ROM's block order. After the controller
 * call of a masked hit, the ROM reloads the mask level into r0 and
 * overwrites it straight away. That is the leftover of a test whose body
 * is a dead store to `m` (the mirror flag, set for real a few lines
 * later), kept as `if (maskLevel == MASK_LEVEL_NONE) m = 0;` (owner
 * decision, #662). gcc 2.9 removes it in stages: jump1 can't delete the
 * test because its body still does something, flow1 then deletes the
 * dead `m = 0` but not the branch or its compare, and jump2 (after
 * reload) deletes the now-empty branch and compare but not the load that
 * fed them, and no flow pass runs after it. An empty body (`if (c) {}`,
 * any release-mode assert or log macro, `(void)(c)`, `c ? (void)0 :
 * (void)0`) is deleted by jump1 with its load, and a store to a local
 * with no test (`m = maskLevel;`) is deleted by flow1 with its load
 * (#662: rounds 3-9, plus 52 macro and reused-local variants). The only
 * other form that leaves the load is an if/else with identical arms.  */
void Player::HandleEvent(s32 from, s32 event, s32 arg)
{
    switch (event) {
    case EVENT_CRYSTAL:
        *gLevelState->GetCurrentLevelFlags() |= LEVEL_FLAG_CRYSTAL;
        gAudioContext->PlaySfx(SFX_CRYSTAL, 0x100);
        break;
    case EVENT_ROOM_EXIT:
        RequestRoomExit();
        gHud->ShowCounters();
        break;
    case EVENT_WARP_EXIT:
        if (gLevelState->timeTrial)
            gLevelState->FreezeLevelClock(100);
        mover->HandleEvent((MovingSprite *)from, event, arg);
        gHud->ShowCounters();
        break;
    case EVENT_WARP_BONUS_ROUND:
        gLevelState->RequestBonusRound();
        mover->HandleEvent((MovingSprite *)from, event, arg);
        break;
    case EVENT_WARP_GEM_PATH:
        gLevelState->RequestGemPath();
        mover->HandleEvent((MovingSprite *)from, event, arg);
        break;
    case EVENT_STOPWATCH:
        if (gLevelState->maskLevel == MASK_LEVEL_INVINCIBLE)
            deadline = 0;
        gAudioContext->PlaySfx(SFX_CLOCK, 0x100);
        gLevelState->StartTimeTrial();
        break;
    case EVENT_CRATE_GEM:
        gAudioContext->PlaySfx(SFX_GEM, 0x100);
        *gLevelState->GetCurrentLevelFlags() |= LEVEL_FLAG_CRATE_GEM;
        break;
    case EVENT_GEM_PATH_GEM:
        gAudioContext->PlaySfx(SFX_GEM, 0x100);
        *gLevelState->GetCurrentLevelFlags() |= LEVEL_FLAG_GEM_PATH_GEM;
        break;
    case EVENT_YELLOW_GEM:
        gAudioContext->PlaySfx(SFX_GEM, 0x100);
        gLevelState->progress.flags |= 2;
        break;
    case EVENT_GREEN_GEM:
        gAudioContext->PlaySfx(SFX_GEM, 0x100);
        gLevelState->progress.flags |= 4;
        break;
    case EVENT_RED_GEM:
        gAudioContext->PlaySfx(SFX_GEM, 0x100);
        gLevelState->progress.flags |= 1;
        break;
    case EVENT_BLUE_GEM:
        gAudioContext->PlaySfx(SFX_GEM, 0x100);
        gLevelState->progress.flags |= 8;
        break;
    case EVENT_POWER_DOUBLE_JUMP:
    case EVENT_POWER_TORNADO_SPIN:
    case EVENT_POWER_BODY_SLAM:
    case EVENT_POWER_TURBO_RUN:
        RequestRoomExit();
        break;
    case EVENT_MASK_GAIN:
        if (gLevelState->maskLevel == MASK_LEVEL_NONE) {
            struct vec2 *h = maskTrail;
            s32 i;

            for (i = 7; i >= 0; i--)
                *h++ = Pos();
        }
        {
            s32 mode = gLevelState->maskLevel;

            if ((mode <= MASK_LEVEL_TWO && gPlayer->ctrlMode != 1) || mode <= MASK_LEVEL_ONE)
                gLevelState->RaiseMaskLevel();
        }
        if (gLevelState->maskLevel == MASK_LEVEL_INVINCIBLE)
            deadline = gRoomFrameCount + 1200;
        break;
    case EVENT_HIT:
    case EVENT_HIT_FIRE:
    case EVENT_HIT_ELECTRIC:
    case EVENT_HIT_EXPLOSION:
    case 5:
    case EVENT_HIT_BITE:
    case 7:
    case EVENT_HIT_DART:
    case EVENT_HIT_CORTEX_SHOT:
    case EVENT_HIT_CRUSH:
        if ((f.flags >> 6) & 1) {
            if (!IsBlinking(this)) {
                if (gLevelState->maskLevel != MASK_LEVEL_NONE) {
                    if (gLevelState->maskLevel <= MASK_LEVEL_TWO) {
                        Sprite *c;
                        s32 cx, cy, m;

                        deadline = gRoomFrameCount + 90;
                        gLevelState->SetMaskLevel(gLevelState->maskLevel - 1);
                        gAudioContext->PlaySfx(SFX_AKU_AKU_LOSE, 0x100);
                        gAudioContext->PlaySfx(SFX_PLAYER_HURT, 0x100);
                        mover->HandleEvent((MovingSprite *)from, EVENT_MASK_HIT, arg);
                        /* Dead on purpose: the ROM reloads the mask level
                         * here and never uses it. This test is what leaves
                         * that load; its `m = 0` is dead (m is set from the
                         * mirror flag below), so the compiler drops the
                         * store, then the empty test, but not the load.
                         * See the comment above the function. */
                        if (gLevelState->maskLevel == MASK_LEVEL_NONE)
                            m = 0;
                        c = child;
                        cx = Q8_TO_INT(c->x);
                        cy = Q8_TO_INT(c->y);
                        m = c->mirrorFlags.mirrorX;
                        gEntitySpawner->SpawnEffectPart(0x22, 3, cx, cy, m);
                    }
                } else {
                    gLevelState->AddDeath();
                    mover->HandleEvent((MovingSprite *)from, event, arg);
                }
            }
        }
        break;
    case EVENT_HANG_GRAB:
    case EVENT_HANG_RELEASE:
        rampY.start = 0;
        rampY.step = 0;
        rampY.target = 0;
        mover->HandleEvent((MovingSprite *)from, event, arg);
        break;
    case EVENT_BUMP:
        mover->HandleEvent((MovingSprite *)from, event, arg);
        break;
    case EVENT_BOUNCE:
    case EVENT_BOUNCE_HIGH:
    case EVENT_LAUNCH_PAD:
        rampY.start = 0;
        rampY.step = 0;
        rampY.target = 0;
        mover->HandleEvent((MovingSprite *)from, event, arg);
        break;
    }
}

/* Aku Aku's step `v` of its animation, clamped to the animation's last. */
static inline void ClampStep(Sprite *c, s32 v)
{
    s32 n = c->bank->anims[c->tag].frameCount;

    CLAMP_INDEX(v, n);
    c->frame = v;
}

static inline void PlaceAt(s32 px, s32 py, Sprite *c, s32 dx, s32 dy)
{
    px += dx;
    py += dy;
    c->x = px;
    c->y = py;
}

/* The orbit tail passes its two sums straight in as arguments. gcc 2.x
 * expands all of an inline call's arguments (a sum is left unforced)
 * before it copies them into the parameters. So the trail addresses and
 * table reads come first, in argument order, and the shifts, trail
 * loads and adds come after the `child` load, as in the ROM. */
static inline void PlaceChild(Sprite *c, s32 px, s32 py)
{
    c->x = px;
    c->y = py;
}

/* Draws Aku Aku and the player. While invincible, Aku Aku sits at the
 * player's head, facing the other way, flickering between its animations 1
 * and 2; the player blinks while the invulnerability lasts, and when it
 * runs out the mask level drops back to two. The player's position goes
 * into the trail, and with one or two masks Aku Aku follows the trail's
 * oldest entry on an orbit, with a random step every 8 frames. */
void Player::Draw()
{
    if (gLevelState->maskLevel == MASK_LEVEL_INVINCIBLE) {
        if (!(gRoomFrameCount & 7))
            /* One expression, so the store address is loaded before the
             * call; the locals keep `+ 2` from being folded into the
             * mirror term and load the mirror bit before the u16 mask, as
             * in the ROM. */
            gAkuAkuInvincibleFrame = ({
                s32 r = RandRange(2);
                s32 m = mirrorFlags.mirrorX;
                s32 v = (u16)r + 2;

                v - m * 2;
            });
        ClampStep(child, gAkuAkuInvincibleFrame);
        if (mirrorBits.flipX < 0)
            PlaceAt(x, y, child, -0x600, -0x1300);
        else
            PlaceAt(x, y, child, 0x600, -0x1300);
        if (gRoomFrameCount & 4)
            child->SetTag(1);
        else
            child->SetTag(2);
        child->Draw();
    }
    {
        /* Through a local, the mask level is loaded again here: tested
         * straight from gLevelState, a not-invincible first test jumps past
         * this one (jump threading). */
        LevelState *game = gLevelState;

        if (game->maskLevel == MASK_LEVEL_INVINCIBLE || !IsBlinking(this) || (gRoomFrameCount & 4))
            gSpriteRenderer->Draw(this);
    }
    if (gLevelState->maskLevel == MASK_LEVEL_INVINCIBLE && !IsBlinking(this))
        gLevelState->SetMaskLevel(MASK_LEVEL_TWO);
    {
        s32 px = x;

        maskTrail[maskTrailIdx].x = px;
    }
    {
        s32 py = y;

        maskTrail[maskTrailIdx].y = py;
    }
    maskTrailIdx = (maskTrailIdx + 1) % 8;
    {
        s32 mode = gLevelState->maskLevel;

        if ((u32)(mode - MASK_LEVEL_ONE) <= 1) {
            if (!(gRoomFrameCount & 7)) {
                s32 v;

                gAkuAkuFollowFrame = gAkuAkuFollowFrame + (u16)RandRange(3) - 1;
                v = gAkuAkuFollowFrame;
                LIMIT_MAX(v, 3);
                LIMIT_MIN(v, 0);
                gAkuAkuFollowFrame = v;
            }
            ClampStep(child, gAkuAkuFollowFrame);
            {
                s32 idx = maskTrailIdx;

                // clang-format off
                PlaceChild(child,
                           maskTrail[idx].x + SIN_Q8(gRoomFrameCount) * 16,
                           maskTrail[idx].y + SIN_Q8(gRoomFrameCount >> 1) * 8 - 0x1800);
                // clang-format on
            }
            child->tag = mode - 1;
            child->Draw();
        }
    }
    if (animDone)
        f.b.bit3 = 0;
}

/* The player's (Player, include/player.hpp) motion, update, body box test
 * and destructor. */

/* Steps each speed toward its ramp's target by the ramp's step, stopping
 * at the target; sets `dir` from the signs of the speeds; saves the
 * previous position and moves. MovingSprite::ApplyVelocity's code, with
 * the player's own global for its redundant store (gLastPlayerVelY, the
 * word after gLastSpriteVelY). Returns whether the player moves. */
s32 Player::ApplyVelocity()
{
    if (speedX < rampX.target) {
        speedX += rampX.step;
        if (speedX > rampX.target)
            speedX = rampX.target;
    } else if (speedX > rampX.target) {
        speedX -= rampX.step;
        if (speedX < rampX.target)
            speedX = rampX.target;
    }
    if (speedY < rampY.target) {
        speedY += rampY.step;
        if (speedY > rampY.target)
            speedY = rampY.target;
    } else if (speedY > rampY.target) {
        speedY -= rampY.step;
        if (speedY < rampY.target)
            speedY = rampY.target;
    }

    dir = 0;
    if (speedX > 0)
        dir = 1;
    else if (speedX < 0)
        dir = 2;
    if (speedY > 0)
        dir |= 8;
    else if (speedY < 0)
        dir |= 4;

    PrevPos() = Pos();
    x += speedX;
    y += speedY;
    if (gLastPlayerVelY != 0 && speedY == 0)
        gLastPlayerVelY = speedY;
    gLastPlayerVelY = speedY;
    return speedX != 0 || speedY != 0;
}

u8 Player::HasRampYTarget()
{
    if (rampY.target != 0) {
        return 1;
    } else {
        return 0;
    }
}

void Player::ClearSpeedY()
{
    speedY = 0;
}

/* Clamps the Y speed and its ramp's start and step to 0 or below.
 * UNUSED: nothing calls it. */
void Player::StopFalling()
{
    LIMIT_MAX(speedY, 0);
    LIMIT_MAX(rampY.start, 0);
    LIMIT_MAX(rampY.step, 0);
}

/* Counts the crate-break limiter down, then the ground sprite's update. */
void Player::Update()
{
    if (countdown != 0) {
        countdown -= 1;
    }
    GroundSprite::Update();
}

/* Whether the player's body box (the frame's) overlaps `box`; an empty
 * body box touches nothing. */
u8 Player::TouchesBox(struct aabb *box)
{
    u8 result = 0;
    struct aabb body = GetBodyBox();

    if (body.w > 0) {
        result = AabbOverlaps(&body, box);
    }
    return result;
}

/* Deletes Aku Aku; g++ then destroys the collision queue (flags 2) and
 * the ground sprite. */
Player::~Player()
{
    delete child;
}
