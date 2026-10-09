#define POLAR_WUMPA_CONSTRUCTOR_OUT_OF_LINE
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

/* The polar wumpas (#664 part 11d, include/vehicle.hpp), ROM
 * 0x0802C270-0x0802C4C8, between polar_player_dispatch.cpp and
 * polar_crate.cpp: the collected wumpa (PolarCollectedWumpa) and the
 * course's wumpa (PolarWumpa). */

/* Flies to the HUD's wumpa counter (0x1000, 0x1000), then is deleted with
 * SFX_HUD_COLLECT; on the way, its animation runs on (UpdateActor's
 * step, with no depth test or state timer). */
void PolarCollectedWumpa::Update()
{
    sortKey = 1;
    x += velX;
    y += velY;

    if (!(x > 0x1000 && y > 0x1000)) {
        gAudioContext->PlaySfx(SFX_HUD_COLLECT, 0x100);
        delete this;
        return;
    }

    animTime += (s16)animTimer;
    animDone = 0;
    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        animTime -= INT_TO_Q8(anims[animIndex].loopThreshold - anims[animIndex].loopBase);
        animDone = 1;
    }
}

/* DrawActor's (ActorSelf::Draw) sprite, with the screen position and the
 * scale given: double size below 0x100, affine unless 1:1; culled off
 * screen, OAM priority 2 when SORT_KEY_FLAG_BEHIND_BG. */
static inline void DrawScaledFrame(ActorSelf *self, s32 screenX, s32 screenY, s32 scale)
{
    u8 *frame = self->GetAnimFrameData();
    u32 flag;
    s32 halfW;
    s32 halfH;

    flag = 0;
    if (scale <= 0xff)
        flag = 0x200;

    if (flag != 0)
        halfW = frame[0] << 3;
    else
        halfW = frame[0] << 2;

    if (flag != 0)
        halfH = frame[1] << 3;
    else
        halfH = frame[1] << 2;

    screenX -= halfW;
    screenY -= halfH;

    if (screenY > 0x9f)
        return;
    if (screenY + halfH * 2 < 0)
        return;
    if (screenX > 0xef)
        return;
    if (screenX + halfW * 2 < 0)
        return;

    if (scale != 0x100)
        flag |= 0x100;

    {
        s32 attr = self->GetAnimFrameAttr();
        u32 packed = (screenY & 0xff) | (((u32)screenX & 0x1ff) << 16) | attr | flag;
        u32 pal = self->palette;
        u32 pre = pal << 0xc;
        u32 attr2;

        if (self->sortKey & SORT_KEY_FLAG_BEHIND_BG)
            attr2 = ((pre | 0x800) << 0x10) >> 0x10;
        else
            attr2 = (pal << 0x1c) >> 0x10;

        SetupSpriteFrameOam(frame, packed, attr2, scale);
    }
}

/* At its screen position (x and y are screen coordinates), at scale
 * 0x140. */
void PolarCollectedWumpa::Draw()
{
    DrawScaledFrame(this, Q8_TO_INT(x), Q8_TO_INT(y), 0x140);
}

/* Counts in its fruit. */
PolarCollectedWumpa::~PolarCollectedWumpa()
{
    s32 i;

    for (i = 0; i < count; i++)
        gLevelState->CollectWumpa();
}

/* At (x, y) on the screen, from the BG's centre, aimed at the wumpa
 * counter: the speed covers the Manhattan distance in 1/0x800 steps.
 * CreateJetpackCollectedWumpa (hovercraft.c) is its twin. */
PolarCollectedWumpa::PolarCollectedWumpa(const struct anim_table_record *rec, s32 x, s32 y,
                                         s32 count)
    : ActorSelf(rec, x, y, 1)
{
    s32 nx, ny;
    s32 dist;
    s32 steps;

    this->count = count;
    this->y += GetActorBgCenterY();
    nx = this->x + GetActorBgCenterX();
    this->x = nx;
    dist = ABS_BRANCHLESS(nx - 0x1000);
    ny = this->y;
    dist += ABS_BRANCHLESS(ny - 0x1000);
    steps = dist / 0x800;
    velX = (0x1000 - nx) / steps;
    velY = (0x1000 - ny) / steps;
}

/* Collected by the player: one wumpa. */
void PolarWumpa::Update()
{
    if ((u8)IsTouchingPlayer(this)) {
        static_cast<PolarPlayer *>(gActorList)->QueueWumpa(1);
        delete this;
    } else {
        ActorSelf::Update();
    }
}

/* The out-of-line copy (CreatePolarWumpa, no caller; vehicle.hpp has the
 * inline one). */
PolarWumpa::PolarWumpa(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
}
