#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "util.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* The polar player's virtual methods, the hazards' hits and six of its
 * states (#664 part 11c, include/vehicle.hpp), ROM
 * 0x0802B364-0x0802BC68, between actor_factory.cpp and
 * polar_player_states.cpp. The player rides the polar bear: the d-pad
 * steers, A jumps and B dashes, and the bear's speed is the cell
 * animation's (SetCellAnimSpeed). Its state lives in the gPolar* globals.
 *
 * Built with old_agbcp, as the C was with old_agbcc. See
 * docs/matching/archive/issue-50-actor-bc68.md and
 * issue-51-54-naked-retry.md. */

/* Slot 2: dispenses the queued wumpas, runs the finish countdown (state
 * 10 when it runs out) and the invulnerability blink, the depth, the
 * animation, the current state's method from stateFuncs, the steering
 * while `gPolarSteerEnabled` is set, then moves Aku Aku with the player
 * (spawning him first). */
void PolarPlayer::Update()
{
    DispenseWumpa();
    if (gPolarFinishTimer != 0 && --gPolarFinishTimer == 0) {
        gPolarPlayerHalted = 1;
        SetCellAnimSpeed(0);
        SetState(10, 10);
    }
    if (gPolarInvulnTimer != 0 && --gPolarInvulnTimer != 0 && gPolarPlayerInactive == 0 &&
        gLevelState->maskLevel != MASK_LEVEL_INVINCIBLE)
        visible = ((u32)gPolarInvulnTimer >> 2) & 1;
    else
        visible = 1;
    if (gPolarPlayerHalted == 0) {
        depth = 0x2f00;
        z = INT_TO_Q8(GetCellAnimDistance()) - depth;
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
    if (gPolarSteerEnabled != 0) {
        struct held_pressed_pair keys = gKeys.half;

        if (keys.held & DPAD_LEFT) {
            if (gPolarSteerTime++ > 12)
                x += -0x380;
            else
                x += -0x2cd;
            LIMIT_MIN(x, -0x3200);
        } else {
            u16 right = keys.held & DPAD_RIGHT;

            if (right) {
                if (gPolarSteerTime++ > 12)
                    x += 0x380;
                else
                    x += 0x2cd;
                LIMIT_MAX(x, 0x3200);
            } else {
                gPolarSteerTime = right;
            }
        }
    }
    if (gPolarAkuAku != NULL) {
        gPolarAkuAku->Move(x, y, z);
    } else {
        s32 tier = gLevelState->maskLevel;

        gPolarAkuAku = SpawnPolarAkuAku(x, y, z, tier);
        if ((u8)IsActorMaskAssistDue())
            gPolarAkuAku->AddMask();
    }
}

/* AnimPart's frame accessors (anim_part.cpp), inlined. */
static inline s32 CurAttr(AnimPart *self)
{
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;

    return (s32)table[idx].attr << 16;
}

static inline u8 *CurFrame(AnimPart *self)
{
    s32 t = Q8_TO_INT(self->animTime);

    return (u8 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t];
}

/* Slot 3: projects the position by depth (scaled and double-sized when
 * drawn behind the reference depth), culls against the screen, uploads
 * the frame's tiles into the other of the two VRAM buffers when the frame
 * changed, and queues the OAM entry. JetpackPlayer::Draw with another
 * projection constant. */
void PolarPlayer::Draw()
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
        f = 0x2f00000 / d;
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
        if (frame != gPolarPlayerLastFrame) {
            gPolarPlayerTileBuffer ^= 1;
            gUnpackRleSpriteFrameFunc((u16 *)gPolarPlayerTiles[gPolarPlayerTileBuffer],
                                      (struct rle_frame *)frame);
            gPolarPlayerLastFrame = frame;
        }
        {
            /* computed first, into r0 */
            u32 tile = GET_TILE_NUM(gPolarPlayerTiles[gPolarPlayerTileBuffer]);

            QueueSpriteFrameOam(attr1, (palette << 12) | tile, scale);
        }
    }
}

/* A hazard's hit (the penguin, the icicle, ...; polar_objects.cpp): nothing
 * while invulnerable (returns 1). With no mask, the player is knocked off
 * (state 6, anim 5, the shock palette, a life lost) and the yeti stops;
 * otherwise Aku Aku loses a mask and the player is invulnerable for 0x4b
 * frames. */
s32 PolarPlayer::Hurt()
{
    /* Both globals through pointers taken first: the ROM loads their
     * addresses before the tests and keeps them for the else branch. */
    s32 *timer = &gPolarInvulnTimer;

    if (*timer != 0)
        return 1;

    PolarAkuAku **aku = &gPolarAkuAku;
    s32 tier = gLevelState->maskLevel;

    if (tier == MASK_LEVEL_NONE) {
        gAudioContext->PlaySfx(SFX_PLAYER_HURT, 0x100);
        QueueVramDmaTransfer((void *)gPolarPlayerShockPalette, (void *)OBJ_PLTT, 0x20, 0x10);
        SetState(6, 5);
        gPolarPauseLocked = 1;
        if (gLevelState->timeTrial == 0)
            gLevelState->LoseLife();
        gPolarSteerEnabled = 0;
        gPolarPlayerInactive = 1;
        SetCellAnimSpeed(0);
        StopYeti();
    } else {
        *timer = 0x4b;
        (*aku)->RemoveMask();
    }
    return 0;
}

/* The electric fence's shock (polar_objects.cpp): as Hurt, but the shock
 * (state 12, anim 11) with no life lost yet (StateShocked). */
s32 PolarPlayer::Shock()
{
    /* The globals through pointers, as in Hurt. */
    s32 *timer = &gPolarInvulnTimer;

    if (*timer != 0)
        return 1;

    PolarAkuAku **aku = &gPolarAkuAku;
    s32 tier = gLevelState->maskLevel;

    if (tier == MASK_LEVEL_NONE) {
        SetState(0xc, 0xb);
        gAudioContext->PlaySfx(SFX_ELECTRIC_SHOCK, 0x100);
        gPolarSteerEnabled = 0;
        gPolarPlayerInactive = 1;
        SetCellAnimSpeed(0);
        StopYeti();
    } else {
        *timer = 0x4b;
        (*aku)->RemoveMask();
    }
    return 0;
}

/* The two VRAM tile buffers Draw uploads the frames into, each the size
 * of the current frame (PolarReloadPlayerTiles, actor_category_hooks.cpp, also calls
 * it). The ROM's product copies are old_agbcp's code for `h * w * 32`. */
void PolarPlayer::AllocTiles()
{
    u8 *f;

    f = CurFrame(this);
    gPolarPlayerTiles[0] = AllocVramTileBlock(f[1] * f[0] * 32);
    f = CurFrame(this);
    gPolarPlayerTiles[1] = AllocVramTileBlock(f[1] * f[0] * 32);
    gPolarPlayerTileBuffer = 1;
    gPolarPlayerLastFrame = 0;
}

/* State 0, mounting the bear: a riderless bear (gRiderlessPolar) runs
 * below while the player falls by `gPolarPlayerVelY` (gravity 0x2d); on
 * landing (y 0x2800) the bear goes, and the player rides it (state 9,
 * anim 9). */
void PolarPlayer::StateMount()
{
    if (gRiderlessPolar == 0) {
        ActorSelf *o = CreateActor(2, x, 0x2800, z, 0);

        gRiderlessPolar = o;
        o->RestartAnim(1);
    }
    s32 ny = y + gPolarPlayerVelY;

    y = ny;
    gPolarPlayerVelY += 0x2d;
    if (ny > 0x2800) {
        gAudioContext->PlaySfx(SFX_POLAR_MOUNT, 0x100);
        y = 0x2800;
        SetState(9, 9);
        gPolarPlayerInactive = 0;
        delete gRiderlessPolar;
        gRiderlessPolar = 0;
        SetCellAnimSpeed(0x19);
    }
}

/* State 1, running: at each turn of the run animation the bear's speed
 * is set, and the player looks around (anim 1) once in three. A jumps
 * (state 4, anim 3) and B dashes (state 2, anim 2) while the steering is
 * on. */
void PolarPlayer::StateRun()
{
    if (animDone != 0) {
        SetCellAnimSpeed(0x24);
        if (animIndex != 0)
            RestartAnim(0);
        else if ((u16)RandRange(3) == 0)
            RestartAnim(1);
        else
            RestartAnim(0);
    }
    if (gPolarSteerEnabled != 0) {
        if (gKeys.half.pressed & 1) {
            SetState(4, 3);
            gAudioContext->PlaySfx(SFX_JUMP, 0x100);
            gPolarPlayerVelY = 0xFFFFF880;
        }
        /* The mask as in StateBoost (polar_player_states.cpp). */
        u32 keys = gKeys.all;
        u32 bit = B_BUTTON;

        bit = keys &= bit;
        if (bit) {
            SetState(2, 2);
            SetCellAnimSpeed(0x38);
        }
    }
}

/* State 4, jumping: rises and falls by `gPolarPlayerVelY` (gravity 0x60,
 * to 0x780; releasing A in the first 10 frames cuts the jump short) and
 * runs again (state 1, anim 4) back on the ground. */
void PolarPlayer::StateJump()
{
    y += gPolarPlayerVelY;
    gPolarPlayerVelY += 0x60;
    LIMIT_MAX(gPolarPlayerVelY, 0x780);
    if (stateTime <= 10 && (gKeys.all & 1) == 0 && gPolarPlayerVelY < -0x400)
        gPolarPlayerVelY = -0x400;
    if (y > 0x2800) {
        y = 0x2800;
        SetState(1, 4);
        SetCellAnimSpeed(0x24);
    }
}

/* State 2, dashing: running again (state 1) when B is released; A jumps
 * as in StateRun. */
void PolarPlayer::StateDash()
{
    if ((u16)(gKeys.all & 2) == 0) {
        SetState(1, 0);
        SetCellAnimSpeed(0x24);
    }

    if (gKeys.half.pressed & 1) {
        SetState(4, 3);
        gAudioContext->PlaySfx(SFX_JUMP, 0x100);
        gPolarPlayerVelY = 0xFFFFF880;
    }
}

/* State 12, shocked by the fence: the palette blinks every 4 frames; after
 * 0x2c frames the player is knocked off (state 6, anim 5) and loses a
 * life. */
void PolarPlayer::StateShocked()
{
    s32 counter = stateTime;

    if (counter > 0x2c) {
        QueueVramDmaTransfer((void *)gPolarPlayerShockPalette, (void *)OBJ_PLTT, 0x20, 0x10);
        gPolarPauseLocked = 1;
        SetState(6, 5);
        if (gLevelState->timeTrial == 0)
            gLevelState->LoseLife();
    } else if (counter & 4) {
        QueueVramDmaTransfer((void *)gPolarPlayerShockPalette, (void *)OBJ_PLTT, 0x20, 0x10);
    } else {
        QueueVramDmaTransfer((void *)gPolarPlayerShockBlinkPalette, (void *)OBJ_PLTT, 0x20, 0x10);
    }
}

/* State 7, caught by the yeti: once the animation is done, carried off
 * (state 8, anim 7) and a life lost; the bear runs on riderless when the
 * player is high enough. */
void PolarPlayer::StateCaught()
{
    if (animDone) {
        gPolarPlayerVelY = 0xFFFFF980;
        if (y > 0x2000)
            gRiderlessPolar = CreateActor(2, x, 0x2800, z, 0);
        SetState(8, 7);
        gPolarPauseLocked = 1;
        if (gLevelState->timeTrial == 0)
            gLevelState->LoseLife();
    }
}
