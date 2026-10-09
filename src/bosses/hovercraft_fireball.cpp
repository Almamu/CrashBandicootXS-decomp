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

/* The hovercraft's fireball (#664 part 11h, include/boss_actors.hpp), ROM
 * 0x08032910-0x08032AF8, between src/vehicle/jetpack/jetpack_collected_wumpa.cpp
 * and hovercraft.cpp. In hovercraft.cpp until #769; built with old_agbcp,
 * as that was. See docs/matching/archive/issue-60-61-gap-31a6c-part2.md. */

/* Takes `amount` off the hit points; at zero, the explosion palette, a
 * sound, and state 1 (exploding) with animation 1. */
void HovercraftFireball::Damage(s32 amount)
{
    s32 health = hp - amount;

    hp = health;
    if (health > 0)
        return;

    palette = 4;
    gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    SetState(1, 1);
}

/* The state method, then deletes itself once the explosion has played
 * through, or else the common update. */
void HovercraftFireball::Update()
{
    (this->*stateFuncs[state])();

    if (state == 1 && animDone != 0)
        delete this;
    else
        ActorSelf::Update();
}

/* 2 hit points, and the airship fireball's orbit set up (never used). */
HovercraftFireball::HovercraftFireball(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, y, z, 2)
{
    centerX = x;
    centerY = y;
    radius = 0;
    velZ = 0x95;
    exploding = 0;
}

void HovercraftFireball::StateExplode()
{
    exploding = 1;
}

/* Flies on at its Z speed, which decays by 5 down to 0x14; on touching
 * the player, hurts it (6) and explodes as in Damage. */
void HovercraftFireball::StateFly()
{
    s32 sum = z;
    s32 delta = velZ;

    sum += delta;
    z = sum;
    delta -= 5;
    velZ = delta;
    if (delta <= 0x13)
        velZ = 0x14;

    if ((u8)IsTouchingPlayer(this)) {
        ((HpActor *)gActorList)->Damage(6);
        palette = 4;
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        SetState(1, 1);
    }
}

/* Update's state dispatch, without the rest. */
void HovercraftFireball::RunState()
{
    (this->*stateFuncs[state])();
}

/* Exploding fireballs can't be shot. */
s32 HovercraftFireball::IsUnshootable()
{
    return exploding;
}
