#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include <libgcc.h>
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* JetpackPlayer's course end, ring pass and VRAM tile buffers (#664 part
 * 11e, include/vehicle.hpp). Built with old_agbcp: AllocTiles's products
 * (`adds r2, r3, #0; muls r2, r1; adds r0, r2, #0`) are old_agbcc's, as in
 * AllocPolarPlayerTiles (polar_player.cpp); the C wrote them in asm. See
 * docs/matching/archive/issue-56-0x0802f0dc-actor.md. */

/* AnimPart::GetAnimFrameData (anim_part.cpp), inlined. */
static inline u8 *CurFrame(AnimPart *self)
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

    f = CurFrame(this);
    gJetpackPlayerTiles[0] = AllocVramTileBlock(f[1] * f[0] * 32);
    f = CurFrame(this);
    gJetpackPlayerTiles[1] = AllocVramTileBlock(f[1] * f[0] * 32);
    gJetpackPlayerTileBuffer = 1;
    gJetpackPlayerLastFrame = 0;
}
