#include "player.hpp"
#include "crate_list.hpp"

extern "C" {
#include "hud.h"
#include "util.h"
#include "system.h"
#include "audio.h"
#include "crates.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
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
 * The case bodies are in the ROM's block order. The ROM reloads the mask
 * level after the controller call of a masked hit and never uses it; only
 * a volatile read reproduces that load. */
void Player::HandleEvent(s32 from, s32 event, s32 arg)
{
    switch (event) {
    case EVENT_CRYSTAL:
        *GetCurrentLevelFlags(gLevelState) |= LEVEL_FLAG_CRYSTAL;
        PlaySfx(gAudioContext, SFX_CRYSTAL, 0x100);
        break;
    case EVENT_ROOM_EXIT:
        RequestRoomExit();
        ShowHudCounters(gHud);
        break;
    case EVENT_WARP_EXIT:
        {
            struct level_state *game = gLevelState;

            if (game->timeTrial)
                FreezeLevelClock(game, 100);
        }
        mover->HandleEvent((MovingSprite *)from, event, arg);
        ShowHudCounters(gHud);
        break;
    case EVENT_WARP_BONUS_ROUND:
        RequestBonusRound(gLevelState);
        mover->HandleEvent((MovingSprite *)from, event, arg);
        break;
    case EVENT_WARP_GEM_PATH:
        RequestGemPath(gLevelState);
        mover->HandleEvent((MovingSprite *)from, event, arg);
        break;
    case EVENT_STOPWATCH:
        if (gLevelState->maskLevel == MASK_LEVEL_INVINCIBLE)
            deadline = 0;
        PlaySfx(gAudioContext, SFX_CLOCK, 0x100);
        StartTimeTrial(gLevelState);
        break;
    case EVENT_CRATE_GEM:
        PlaySfx(gAudioContext, SFX_GEM, 0x100);
        *GetCurrentLevelFlags(gLevelState) |= LEVEL_FLAG_CRATE_GEM;
        break;
    case EVENT_GEM_PATH_GEM:
        PlaySfx(gAudioContext, SFX_GEM, 0x100);
        *GetCurrentLevelFlags(gLevelState) |= LEVEL_FLAG_GEM_PATH_GEM;
        break;
    case EVENT_YELLOW_GEM:
        PlaySfx(gAudioContext, SFX_GEM, 0x100);
        gLevelState->flags |= 2;
        break;
    case EVENT_GREEN_GEM:
        PlaySfx(gAudioContext, SFX_GEM, 0x100);
        gLevelState->flags |= 4;
        break;
    case EVENT_RED_GEM:
        PlaySfx(gAudioContext, SFX_GEM, 0x100);
        gLevelState->flags |= 1;
        break;
    case EVENT_BLUE_GEM:
        PlaySfx(gAudioContext, SFX_GEM, 0x100);
        gLevelState->flags |= 8;
        break;
    case EVENT_POWER_DOUBLE_JUMP:
    case EVENT_POWER_TORNADO_SPIN:
    case EVENT_POWER_BODY_SLAM:
    case EVENT_POWER_TURBO_RUN:
        RequestRoomExit();
        break;
    case EVENT_MASK_GAIN:
        if (gLevelState->maskLevel == MASK_LEVEL_NONE) {
            struct gfx_vec *h = maskTrail;
            s32 i;

            for (i = 7; i >= 0; i--)
                *h++ = Pos();
        }
        {
            s32 mode = gLevelState->maskLevel;

            if ((mode <= MASK_LEVEL_TWO && gPlayer->ctrlMode != 1) || mode <= MASK_LEVEL_ONE)
                RaiseMaskLevel(gLevelState);
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
    case 8:
    case EVENT_HIT_CORTEX_SHOT:
    case EVENT_HIT_CRUSH:
        if ((f.flags >> 6) & 1) {
            if (!IsBlinking(this)) {
                struct level_state *game = gLevelState;

                if (game->maskLevel != MASK_LEVEL_NONE) {
                    if (game->maskLevel <= MASK_LEVEL_TWO) {
                        Sprite *c;
                        s32 cx, cy, m;

                        deadline = gRoomFrameCount + 90;
                        SetMaskLevel(game, game->maskLevel - 1);
                        PlaySfx(gAudioContext, SFX_AKU_AKU_LOSE, 0x100);
                        PlaySfx(gAudioContext, SFX_PLAYER_HURT, 0x100);
                        mover->HandleEvent((MovingSprite *)from, EVENT_MASK_HIT, arg);
                        /* The ROM reloads the mode here and never uses it. */
                        (void)*(volatile s32 *)&gLevelState->maskLevel;
                        c = child;
                        cx = Q8_TO_INT(c->x);
                        cy = Q8_TO_INT(c->y);
                        m = c->mirrorFlags.mirrorX;
                        SpawnEffectPart(gEntitySpawner, 0x22, 3, cx, cy, m);
                    }
                } else {
                    AddDeath(game);
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

static inline void SetChildAnim(Sprite *c, u8 anim)
{
    c->tag = anim;
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
            SetChildAnim(child, 1);
        else
            SetChildAnim(child, 2);
        child->Draw();
    }
    {
        /* Through a local, the mask level is loaded again here: tested
         * straight from gLevelState, a not-invincible first test jumps past
         * this one (jump threading). */
        struct level_state *game = gLevelState;

        if (game->maskLevel == MASK_LEVEL_INVINCIBLE || !IsBlinking(this) || (gRoomFrameCount & 4))
            ((SpriteRenderer *)gSpriteRenderer)->Draw(this);
    }
    {
        struct level_state *game = gLevelState;

        if (game->maskLevel == MASK_LEVEL_INVINCIBLE && !IsBlinking(this))
            SetMaskLevel(game, MASK_LEVEL_TWO);
    }
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

        if ((u32)(mode - 1) <= 1) {
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
