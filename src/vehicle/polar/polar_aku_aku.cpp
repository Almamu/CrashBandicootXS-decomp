#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* Aku Aku (#664 part 11d, include/vehicle.hpp), ROM 0x0802D204-0x0802D59C,
 * between polar_objects.cpp and polar_course_objects.cpp: PolarAkuAku's
 * Refresh, Update and Move, its masks and its constructor, and
 * GetPolarMaskLevel. The mask level is the level's
 * (gLevelState->maskLevel, SetMaskLevel); Refresh shows it. */

/* Switches to animation `idx`, keeping the frame unless it is past the
 * new animation's end (as PolarPlayer::Boost). */
static inline void SwitchAnim(ActorSelf *a, s32 idx)
{
    a->animIndex = idx;
    a->animTimer = a->anims[idx].duration;
    a->animDone = 0;
    if (a->GetAnimFrameBaseOffset() >= a->anims[a->animIndex].loopThreshold)
        a->animTime = 0;
}

/* Aku Aku's look for the mask level (gLevelState->maskLevel): hidden with
 * none (unless a mask was just `lost`), else the level's palette
 * (gPolarAkuAkuPalette1 on) and animation 0. At the third level,
 * invincible for 500 frames (state 1); a mask just lost with none left,
 * state 2 (animation 1, Update refreshes again once it is done); else
 * back to state 0. */
void PolarAkuAku::Refresh(u8 lost)
{
    s32 level = gLevelState->maskLevel;

    if (level == MASK_LEVEL_NONE && lost == 0) {
        visible = level;
    } else {
        QueueVramDmaTransfer((u8 *)gPolarAkuAkuPalette1 + (level - 1) * PALETTE_SIZE_16,
                             (void *)(OBJ_PLTT + 14 * PALETTE_SIZE_16), PALETTE_SIZE_16, 0x10);
        visible = 1;
        SwitchAnim(this, 0);
    }

    if (level == MASK_LEVEL_INVINCIBLE) {
        gPolarAkuAkuInvincibleTimer = 0x1F4;
        SetState(1, 0);
    } else if (level == MASK_LEVEL_NONE && lost != 0) {
        gPolarAkuAkuInvincibleTimer = level;
        SetState(2, 1);
    } else {
        gPolarAkuAkuInvincibleTimer = 0;
        if (state != 0)
            SetState(0, 0);
    }
}

/* Invincible, it blinks (gPolarAkuAkuPalette3/2 every 4 frames) until the
 * timer runs out, then is back to two masks. Its animation runs on
 * (ActorSelf::Update's step, with no clipping). */
void PolarAkuAku::Update()
{
    if (gPolarAkuAkuInvincibleTimer != 0) {
        if (gPolarAkuAkuInvincibleTimer & 4)
            QueueVramDmaTransfer((void *)gPolarAkuAkuPalette3,
                                 (void *)(OBJ_PLTT + 14 * PALETTE_SIZE_16), PALETTE_SIZE_16, 0x10);
        else
            QueueVramDmaTransfer((void *)gPolarAkuAkuPalette2,
                                 (void *)(OBJ_PLTT + 14 * PALETTE_SIZE_16), PALETTE_SIZE_16, 0x10);

        gPolarAkuAkuInvincibleTimer -= 1;
        if (gPolarAkuAkuInvincibleTimer == 0) {
            gLevelState->SetMaskLevel(MASK_LEVEL_TWO);
            Refresh(0);
        }
    }

    if (state == 2 && animDone != 0)
        Refresh(0);

    UpdateDepth();
    stateTime += 1;
    animTime += (s16)animTimer;
    animDone = 0;
    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        animTime -= INT_TO_Q8(anims[animIndex].loopThreshold - anims[animIndex].loopBase);
        animDone = 1;
    }
}

/* Follows the player at (posX, posY, posZ), easing in (1/16 in X and Y,
 * 1/4 in Z): state 0 hovers round behind it (a sine path), state 1
 * (invincible) sits on it, in front; otherwise it eases to it, in front.
 * "Easing" is a round-toward-zero divide of the remaining delta. The
 * `goto` is the ROM's one copy of the Y and Z easing for state 0 and the
 * default case: with the targets set in each branch and one easing after
 * them, the registers differ. */
void PolarAkuAku::Move(s32 posX, s32 posY, s32 posZ)
{
    s32 tx, ty, tz;
    s32 cur, d;

    if (state == 0) {
        s32 ox = SIN_Q8(stateTime * 4) * 24 - 0x1000;
        s32 oy;

        tx = posX + ox;
        oy = SIN_Q8(stateTime * 2) * 10 - 0x1e00;
        ty = posY + oy;
        tz = posZ - 0x200;
        x += (tx - x) / 16;
        cur = y;
        d = ty - cur;
        goto ease_y;
    } else if (state == 1) {
        s32 oy;

        x = posX;
        oy = SIN_Q8(stateTime * 9) * 4 - 0xa00;
        y = oy + posY;
        z = posZ + 0x200;
        return;
    }
    tz = posZ + 0x200;
    x += (posX - x) / 16;
    cur = y;
    d = posY - cur;
ease_y:
    y = cur + d / 16;
    z += (tz - z) / 4;
}

/* All masks gone (PolarPlayer::Catch). */
void PolarAkuAku::ClearMask()
{
    gLevelState->SetMaskLevel(MASK_LEVEL_NONE);
    Refresh(0);
}

/* A mask lost (PolarPlayer::Hurt and Shock): the level, one down
 * unless none is left, and a lost mask shown. */
s32 PolarAkuAku::RemoveMask()
{
    s32 level;

    gAudioContext->PlaySfx(SFX_AKU_AKU_LOSE, 0x100);
    level = gLevelState->maskLevel;
    if (level != MASK_LEVEL_NONE) {
        level -= 1;
        gLevelState->SetMaskLevel(level);
    }
    Refresh(1);
    return level;
}

/* A mask gained (PolarPlayer::GiveMask): the level, one up to
 * invincible. */
s32 PolarAkuAku::AddMask()
{
    s32 level;

    gAudioContext->PlaySfx(SFX_AKU_AKU_GAIN, 0x100);
    level = gLevelState->maskLevel;
    if (level != MASK_LEVEL_INVINCIBLE) {
        level += 1;
        gLevelState->SetMaskLevel(level);
    }
    Refresh(0);
    return level;
}

/* Behind the player (SpawnPolarAkuAku, actor_factory.cpp: at its
 * position), with the mask level `level`. */
PolarAkuAku::PolarAkuAku(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 level)
    : ActorSelf(rec, x - 0x1000, y - 0x1E00, z - 0x200)
{
    gLevelState->SetMaskLevel(level);
    Refresh(0);
}

/* The mask level (no caller). */
void PolarAkuAku::SetMask(s32 level)
{
    gLevelState->SetMaskLevel(level);
}

/* The mask level (no caller). */
s32 GetPolarMaskLevel(void)
{
    return gLevelState->maskLevel;
}
