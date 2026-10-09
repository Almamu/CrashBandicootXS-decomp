#include "boss_actors.hpp"
#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "match.h"
#include <libgcc.h>
#include "system.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
}

/* The jetpack ring's constructor and IsUnshootable and the collected
 * wumpa (#664 part 11h, include/vehicle.hpp), ROM 0x080326E4-0x08032910,
 * between jetpack_rocket.cpp and src/bosses/hovercraft_fireball.cpp. The
 * collected wumpa's destructor is its key method, so
 * gJetpackCollectedWumpaVtable is emitted here. In hovercraft.cpp until
 * #769; built with old_agbcp, as that was. See
 * docs/matching/archive/issue-60-61-gap-31a6c-part2.md. */

/* 1 hit point, not cued yet. */
JetpackRing::JetpackRing(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, y, z, 1)
{
    cued = 0;
}

/* Rings can't be shot. */
s32 JetpackRing::IsUnshootable()
{
    return 1;
}

/* Flies on towards the wumpa counter; once there (out of the positive
 * quadrant past 0x1000), the collect sound and deletes itself, otherwise
 * the animation step. */
void JetpackCollectedWumpa::Update()
{
    s32 one = 1;
    s32 nx, ny;

    sortKey = one;
    nx = x += velX;
    ny = y += velY;
    if (nx <= 0x1000 || ny <= 0x1000) {
        gAudioContext->PlaySfx(SFX_HUD_COLLECT, 0x100);
        delete this;
        return;
    }
    animTime += (s16)animTimer;
    animDone = 0;
    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        ANIM_REWIND(animTime, anims[animIndex]);
        animDone = one;
    }
}

/* Draws the current frame at the position, centred on the frame's width
 * and height, unless it is entirely off screen. Same shape as
 * ActorSelf::Draw with the scale doubling fixed off: `scaled` starts at
 * 0 (halving nothing) and becomes the 0x100 OBJ-affine bit once the
 * sprite is known to be visible. The third OAM word takes the palette as
 * its priority nibble, plus 0x800 when the sort key is behind the BG. */
void JetpackCollectedWumpa::Draw()
{
    s32 sx;
    s32 sy;
    u8 *frame;
    u32 scaled;
    s32 halfW;
    s32 halfH;
    u32 attr;
    u32 attr2;
    u16 attr2Out;
    s32 oamPriority = 0x180;
    u32 highBit = 0x800;

    {
        s32 qx = x, qy = y;

        sx = Q8_TO_INT(qx);
        sy = Q8_TO_INT(qy);
    }
    frame = GetAnimFrameData();
    scaled = 0;
    halfW = scaled ? frame[0] << 3 : frame[0] << 2;
    halfH = scaled ? frame[1] << 3 : frame[1] << 2;
    sx -= halfW;
    sy -= halfH;
    if (sy > 0x9f)
        return;
    if (sy + halfH * 2 < 0)
        return;
    if (sx > 0xef)
        return;
    if (sx + halfW * 2 < 0)
        return;

    scaled |= 0x100;
    attr = (sy & 0xff) | ((sx & 0x1ff) << 16) | GetAnimFrameAttr() | scaled;
    sx = palette;
    attr2 = sx << 12;
    if (sortKey & SORT_KEY_FLAG_BEHIND_BG)
        attr2Out = attr2 | highBit;
    else
        attr2Out = attr2;
    SetupSpriteFrameOam(frame, attr, attr2Out, oamPriority);
}

/* Adds the fruit it carries. */
JetpackCollectedWumpa::~JetpackCollectedWumpa()
{
    s32 i;

    for (i = 0; i < reward; i++)
        gLevelState->CollectWumpa();
}

/* PolarCollectedWumpa's constructor with 1 hit point: from the BG's
 * centre, the velocity that reaches the wumpa counter (0x1000, 0x1000) in
 * a number of frames proportional to the distance. */
JetpackCollectedWumpa::JetpackCollectedWumpa(const struct anim_table_record *rec, s32 x, s32 y,
                                             s32 reward)
    : HpActor(rec, x, y, 1, 1)
{
    s32 sum;
    s32 q;
    s32 nx;

    this->reward = reward;
    this->y += GetActorBgCenterY();
    nx = this->x + GetActorBgCenterX();
    this->x = nx;

    {
        s32 a1 = nx - 0x1000;
        s32 a2 = ABS_BRANCHLESS(a1);
        s32 ny = this->y;
        s32 b1 = ny - 0x1000;
        s32 b2 = ABS_BRANCHLESS(b1);

        sum = a2 + b2;
        if (sum < 0)
            sum += 0x7ff;
        q = sum >> 0xb;

        velX = __divsi3(0x1000 - nx, q);
        velY = __divsi3(0x1000 - ny, q);
    }
}

/* Collected wumpas can't be shot. */
s32 JetpackCollectedWumpa::IsUnshootable()
{
    return 1;
}
