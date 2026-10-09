#include "vehicle.hpp"
#include "boss_actors.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include <libgcc.h>
#include "system.h"
#include "actor.h"
#include "bosses.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* JetpackPlayer's constructor, virtual methods, steering, first three
 * states, course end, ring pass and VRAM tile buffers (#664 part 11e,
 * include/vehicle.hpp), ROM 0x0802E740-0x0802F3BC, between
 * jetpack_spawn.cpp and jetpack_player.cpp. JetpackPlayer is
 * gJetpackPlayerVtable; its state lives in the gJetpack* globals.
 *
 * Built with old_agbcp (as the C was with old_agbcc): AllocTiles's
 * products (`adds r2, r3, #0; muls r2, r1; adds r0, r2, #0`) are
 * old_agbcc's, as in AllocPolarPlayerTiles (polar_player.cpp); the C wrote
 * them in asm. See docs/matching/archive/issue-56-0x0802f0dc-actor.md. */

/* Clamps a steering speed to +-0x240, keeping its sign. */
#define CLAMP_SPEED(v)                                                         \
    if (ABS_BRANCHLESS(v) > 0x240)                                             \
        (v) = (v) < 0 ? -0x240 : ((v) != 0 ? 0x240 : 0);                      \
    else (void)0

/* InitJetpackPlayer: 100 hit points (0x78 when `IsActorMaskAssistDue`
 * says so), and a reset of all its global state. A nonzero start depth
 * starts it in state 7. */
JetpackPlayer::JetpackPlayer(const struct anim_table_record *rec, s32 z)
    : HpActor(rec, 0, z == 0 ? -0x9600 : 0, z, 100)
{
    AllocTiles();
    gJetpackPlayerVelX = 0;
    if (this->z != 0) {
        gJetpackPlayerVelY = 0;
        SetState(7, 0);
        SetCellAnimSpeed(0x28);
    } else {
        SetCellAnimSpeed(0x1e);
        gJetpackPlayerVelY = 0x180;
    }
    gJetpackInputEnabled = 0;
    gJetpackPlayerInactive = 1;
    gJetpackShotCooldown = 0;
    gJetpackFadeStarted = 0;
    gJetpackPlayerHalted = 0;
    gJetpackQueuedWumpa = 0;
    gJetpackWumpaDispenseTimer = 0;
    gJetpackFlashTimer = 0;
    gJetpackRingLastFrame = -0xbe;
    gJetpackRingChain = 0;
    gJetpackPauseLocked = 0;
    if ((u8)IsActorMaskAssistDue())
        hp = 0x78;
    gJetpackPlayerMaxHp = hp;
    gJetpackBomberCount = 0;
    gJetpackBomberSfxTimer = 0;
}

/* Slot 2: the engine sound's throttle, the fire cooldown, the movement by
 * the steering speeds (clamped to the play area unless
 * `gJetpackPlayerInactive` is set), the depth, the animation, then the
 * current state's method from stateFuncs. */
void JetpackPlayer::Update()
{
    s32 nx, ny;

    if (gJetpackBomberCount != 0) {
        if (gJetpackBomberSfxTimer-- <= 0) {
            s32 vol;

            gJetpackBomberSfxTimer = 0x16;
            vol = gJetpackBomberCount * 48;
            LIMIT_MAX(vol, 0x100);
            gAudioContext->PlaySfx(SFX_JETPACK_BOMBER, vol);
        }
        gJetpackBomberCount = 0;
    }
    if (gJetpackShotCooldown != 0)
        gJetpackShotCooldown--;
    AnimatePalette();
    DispenseWumpa();
    nx = x += gJetpackPlayerVelX;
    ny = y += gJetpackPlayerVelY;
    if (gJetpackPlayerInactive == 0) {
        x = CLAMP_MIN(nx, -0x8000);
        x = CLAMP_MAX(x, 0x8000);
        y = CLAMP_MIN(ny, -0x4b00);
        y = CLAMP_MAX(y, 0x4b00);
    }
    if (gJetpackPlayerHalted == 0) {
        depth = 0x1c00;
        z = INT_TO_Q8(GetCellAnimDistance()) + depth;
    }
    {
        s32 d = (depth >> 1) & 0x7f80;

        sortKey = d | (((ABS_BRANCHLESS(y) + ABS_BRANCHLESS(x)) >> 11) & 0x7f);
    }
    stateTime++;
    animTime += (s16)animTimer;
    animDone = 0;
    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        ANIM_REWIND(animTime, anims[animIndex]);
        animDone = 1;
    }
    UpdateActorBgScroll(x, y);
    (this->*stateFuncs[state])();
}

/* AnimPart's frame accessors (anim_part.cpp), inlined. */
static inline u8 *CurFrame(AnimPart *self)
{
    s32 base = Q8_TO_INT(self->animTime);
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;
    s32 val = table[idx].frameIndex;

    val += base;
    return (u8 *)self->frameOffsets[val];
}

static inline s32 CurAttr(AnimPart *self)
{
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;

    return (s32)table[idx].attr << 16;
}

/* Slot 3: projects the position by depth (scaled and double-sized when
 * drawn behind the reference depth), culls against the screen, uploads
 * the frame's tiles into the other of the two VRAM buffers when the
 * frame changed, and queues the OAM entry. */
void JetpackPlayer::Draw()
{
    s32 scale;
    s32 attr1 = 0;
    u8 *frame;
    u32 w, h;
    s32 halfW, halfH;
    s32 sx, sy;

    frame = CurFrame(this);
    w = frame[0];
    halfW = w * 4;
    h = frame[1];
    halfH = h * 4;
    if (depth == record->baseDepth) {
        scale = 0x100;
        sy = Q8_TO_INT(y + GetActorBgCenterY());
        sx = Q8_TO_INT(x + GetActorBgCenterX());
    } else {
        s32 d = depth;
        s32 f;

        scale = Q8_DIV(d, record->baseDepth);
        f = 0x1c00000 / d;
        sy = Q8_TO_INT(Q12_MUL(y, f) + GetActorBgCenterY());
        sx = Q8_TO_INT(Q12_MUL(x, f) + GetActorBgCenterX());
        attr1 = 0x100;
        if (scale <= 0xff) {
            attr1 |= 0x200;
            halfW = w * 8;
            halfH = h * 8;
        }
    }
    sx -= halfW;
    sy -= halfH;
    if (sy <= 0x9f && sy + halfH * 2 >= 0 && sx <= 0xef && sx + halfW * 2 >= 0) {
        u32 attr = CurAttr(this);

        attr1 |= (sy & 0xff) | ((sx & 0x1ff) << 16) | attr | GetSpriteShapeSizeBits(frame);
        if (frame != gJetpackPlayerLastFrame) {
            gJetpackPlayerTileBuffer ^= 1;
            gUnpackRleSpriteFrameFunc((u16 *)gJetpackPlayerTiles[gJetpackPlayerTileBuffer],
                                      (struct rle_frame *)frame);
            gJetpackPlayerLastFrame = frame;
        }
        {
            /* computed first, into r0 */
            u32 tile = GET_TILE_NUM(gJetpackPlayerTiles[gJetpackPlayerTileBuffer]);

            QueueSpriteFrameOam(attr1, (palette << 12) | tile, scale);
        }
    }
}

/* Slot 4: ignored during the first 16 frames of states 2/3. Out of hit
 * points, the player enters state 4 (anim 3), input is locked and the
 * steering speeds are cut; otherwise SFX_UNKNOWN_42 plays. */
void JetpackPlayer::Damage(s32 dmg)
{
    if ((u32)(state - 2) <= 1 && stateTime <= 0x10)
        return;
    hp -= dmg;
    gJetpackFlashTimer = 0x12;
    if (hp <= 0) {
        hp = 0;
        gAudioContext->PlaySfx(SFX_JETPACK_PLAYER_DOWN, 0x100);
        SetState(4, 3);
        if (gLevelState->timeTrial == 0)
            gLevelState->LoseLife();
        gJetpackInputEnabled = 0;
        gJetpackPauseLocked = 1;
        gJetpackPlayerInactive = 1;
        SetCellAnimSpeed(0x1e);
        gJetpackPlayerVelY = 0;
        CLAMP_SPEED(gJetpackPlayerVelX);
        gJetpackPlayerVelX /= 2;
    } else {
        gAudioContext->PlaySfx(SFX_UNKNOWN_42, 0x100);
    }
}

/* The key word read as a whole (the ROM does a 32-bit load). */
static inline struct held_pressed_pair ReadKeys(void)
{
    return gKeys.half;
}

/* Moves a steering speed 0x40 toward zero. */
static inline void DecaySpeed(s32 *p)
{
    s32 v = *p;

    if (v >= 0) {
        if (v != 0)
            v -= 0x40;
    } else {
        v += 0x40;
    }
    *p = v;
}

/* Vertical steering: up/down change `gJetpackPlayerVelY` by 0x40 while
 * input is enabled, otherwise it decays to zero; clamped to +-0x240. */
void JetpackPlayer::SteerY()
{
    if (gJetpackInputEnabled && (ReadKeys().held & DPAD_UP))
        gJetpackPlayerVelY -= 0x40;
    else if (gJetpackInputEnabled && (ReadKeys().held & DPAD_DOWN))
        gJetpackPlayerVelY += 0x40;
    else {
        DecaySpeed(&gJetpackPlayerVelY);
        if (ABS_BRANCHLESS(gJetpackPlayerVelY) <= 0x40)
            gJetpackPlayerVelY = 0;
    }
    CLAMP_SPEED(gJetpackPlayerVelY);
}

/* Horizontal steering, same shape with left/right and
 * `gJetpackPlayerVelX`. */
void JetpackPlayer::SteerX()
{
    if (gJetpackInputEnabled && (ReadKeys().held & DPAD_LEFT))
        gJetpackPlayerVelX -= 0x40;
    else if (gJetpackInputEnabled && (ReadKeys().held & DPAD_RIGHT))
        gJetpackPlayerVelX += 0x40;
    else {
        DecaySpeed(&gJetpackPlayerVelX);
        if (ABS_BRANCHLESS(gJetpackPlayerVelX) <= 0x40)
            gJetpackPlayerVelX = 0;
    }
    CLAMP_SPEED(gJetpackPlayerVelX);
}

/* State 1, flying: steering, then R/L enter the roll states 2/3 and A
 * fires a shot (sfx 0x24, `SpawnJetpackShot`) when the cooldown allows. */
void JetpackPlayer::StateFly()
{
    SteerY();
    SteerX();
    if (gJetpackInputEnabled) {
        struct held_pressed_pair keys = gKeys.half;

        if (keys.held & L_BUTTON) {
            gJetpackFlashTimer = 0x12;
            gAudioContext->PlaySfx(SFX_SPIN, 0x100);
            SetState(2, 1);
        } else if (keys.held & R_BUTTON) {
            gJetpackFlashTimer = 0x12;
            gAudioContext->PlaySfx(SFX_SPIN, 0x100);
            SetState(3, 2);
        } else if (gJetpackShotCooldown == 0 && (keys.held & 1)) {
            s32 sx, sy;

            gJetpackShotCooldown = 0x12;
            gAudioContext->PlayAmbientSfx(SFX_JETPACK_SHOOT, 1000, 0xa0, true);
            sx = x + 0x1200;
            sy = y - 0x1800;
            SpawnJetpackShot(sx, sy, z + 10, Q12_MUL(sx, 0x199), Q12_MUL(sy, 0x199));
        }
    }
}

/* State 2, rolling left: a quick leftward burst for 5 frames, then the
 * horizontal speed recovers; after 0x21 frames R/L may chain another
 * roll, and the animation's end returns to state 1. */
void JetpackPlayer::StateRollLeft()
{
    SteerY();
    if (stateTime <= 5) {
        gJetpackPlayerVelX += -0x100;
        LIMIT_MIN(gJetpackPlayerVelX, -0x500);
    } else {
        gJetpackPlayerVelX += 0x2d;
        if (ABS_BRANCHLESS(gJetpackPlayerVelX) <= 0x2d)
            gJetpackPlayerVelX = 0;
    }
    if (stateTime > 0x21) {
        SteerX();
        struct held_pressed_pair keys = gKeys.half;

        if (keys.held & L_BUTTON) {
            gJetpackFlashTimer = 0x12;
            gAudioContext->PlaySfx(SFX_SPIN, 0x100);
            SetState(2, 1);
        } else if (keys.held & R_BUTTON) {
            gJetpackFlashTimer = 0x12;
            gAudioContext->PlaySfx(SFX_SPIN, 0x100);
            SetState(3, 2);
        }
    }
    if (animDone)
        SetState(1, 0);
}

/* State 3, rolling right: the mirror of StateRollLeft. */
void JetpackPlayer::StateRollRight()
{
    SteerY();
    if (stateTime <= 5) {
        gJetpackPlayerVelX += 0x100;
        LIMIT_MAX(gJetpackPlayerVelX, 0x500);
    } else {
        gJetpackPlayerVelX -= 0x2d;
        if (ABS_BRANCHLESS(gJetpackPlayerVelX) <= 0x2d)
            gJetpackPlayerVelX = 0;
    }
    if (stateTime > 0x21) {
        SteerX();
        struct held_pressed_pair keys = gKeys.half;

        if (keys.held & L_BUTTON) {
            gJetpackFlashTimer = 0x12;
            gAudioContext->PlaySfx(SFX_SPIN, 0x100);
            SetState(2, 1);
        } else if (keys.held & R_BUTTON) {
            gJetpackFlashTimer = 0x12;
            gAudioContext->PlaySfx(SFX_SPIN, 0x100);
            SetState(3, 2);
        }
    }
    if (animDone)
        SetState(1, 0);
}


/* AnimPart::GetAnimFrameData (anim_part.cpp), inlined; written
 * differently from Draw's CurFrame above. */
static inline u8 *CurFrameData(AnimPart *self)
{
    s32 t = Q8_TO_INT(self->animTime);

    return (u8 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t];
}

/* The course's end (the category's hook, JetpackReachCourseEnd): unless
 * the player is already inactive, it stops, loses the input and enters
 * state 5 (animation 4), with a cue; in a time trial the clock freezes. */
void JetpackPlayer::FinishRun()
{
    s32 zero = gJetpackPlayerInactive;

    if (zero == 0) {
        gJetpackInputEnabled = zero;
        gJetpackPlayerHalted = 1;
        gJetpackPlayerInactive = 1;
        SetCellAnimSpeed(0x3c);
        gJetpackPlayerVelY = zero;
        gJetpackPlayerVelX = zero;
        SetState(5, 4);
        gAudioContext->PlaySfx(SFX_JETPACK_RUN_FINISH, 0x100);
        if (gLevelState->timeTrial != 0)
            gLevelState->FreezeLevelClock(0x2710);
    }
}

/* Flying through a ring at (x, y) (UpdateJetpackRing): in states 1, 6, 2
 * and 3, the player snaps to the ring's center, plays animation 5 and
 * enters state 6 (the ring's boost, StateBoost), stopping its steering.
 * Outside time trials, rings passed less than 0xbe frames apart (and
 * more than 0x14) build a chain of five rewards: 1, 5 and 0x14 wumpas,
 * a fifth of the hit points, and a life. */
void JetpackPlayer::PassRing(s32 x, s32 y)
{
    s32 state = this->state;
    u8 paused;

    if (state != 1 && state != 6 && state != 2 && state != 3)
        return;

    if (animIndex != 5) {
        animIndex = 5;
        animTimer = anims[5].duration;
        animDone = 0;
        animTime = 0;
    }

    this->x = x;
    this->y = y;

    if (this->state != 6)
        SetCellAnimSpeed(0x50);
    this->state = 6;

    /* Both addresses first, as the ROM loads them (stored in place,
     * each address is loaded just before its store). */
    {
        s32 *velY = &gJetpackPlayerVelY;
        s32 *velX = &gJetpackPlayerVelX;

        *velX = 0;
        *velY = 0;
    }
    stateTime = 0;

    paused = gLevelState->timeTrial;
    if (paused != 0)
        return;

    if (GetActorCategoryFrameCount() - gJetpackRingLastFrame <= 0x14)
        return;

    if (GetActorCategoryFrameCount() - gJetpackRingLastFrame > 0xbe)
        gJetpackRingChain = paused;

    switch (gJetpackRingChain) {
    case 0:
        if (gLevelState->timeTrial == 0) {
            if (gJetpackQueuedWumpa == 0)
                gJetpackWumpaDispenseTimer = 0xf;
            gJetpackQueuedWumpa += 1;
        }
        break;
    case 1:
        if (gLevelState->timeTrial == 0) {
            if (gJetpackQueuedWumpa == 0)
                gJetpackWumpaDispenseTimer = 0xf;
            gJetpackQueuedWumpa += 5;
        }
        break;
    case 2:
        if (gLevelState->timeTrial == 0) {
            if (gJetpackQueuedWumpa == 0)
                gJetpackWumpaDispenseTimer = 0xf;
            gJetpackQueuedWumpa += 0x14;
        }
        break;
    case 3:
        if (gJetpackPlayerInactive == 0) {
            /* `pct` a variable: the ROM multiplies (`muls`), where a
             * literal 0x14 is strength-reduced to shifts. */
            s32 max = gJetpackPlayerMaxHp;
            s32 pct = 0x14;
            s32 v = hp + __divsi3(pct * max, 0x64);

            hp = v;
            if (v > gJetpackPlayerMaxHp)
                hp = gJetpackPlayerMaxHp;
        }
        break;
    case 4:
        if (gLevelState->timeTrial == 0) {
            gLevelState->AddLife();
            gAudioContext->PlaySfx(SFX_EXTRA_LIFE, 0x100);
        }
        break;
    }

    gJetpackRingLastFrame = GetActorCategoryFrameCount();
    gJetpackRingChain++;
    if (gJetpackRingChain == 5)
        gJetpackRingChain = 0;
}

/* The two VRAM tile buffers Draw unpacks the frames into, each the size
 * of the current frame (w * h tiles); buffer 1 first, and no frame in
 * either. */
void JetpackPlayer::AllocTiles()
{
    u8 *f;

    f = CurFrameData(this);
    gJetpackPlayerTiles[0] = AllocVramTileBlock(f[1] * f[0] * 32);
    f = CurFrameData(this);
    gJetpackPlayerTiles[1] = AllocVramTileBlock(f[1] * f[0] * 32);
    gJetpackPlayerTileBuffer = 1;
    gJetpackPlayerLastFrame = 0;
}
