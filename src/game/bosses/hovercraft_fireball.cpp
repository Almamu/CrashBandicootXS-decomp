#include "game/bosses/boss_actors.hpp"
#include "game/vehicles/vehicle.hpp"
#include "engine/audio/audio.hpp"
#include "game/level/level_state.hpp"
#include "engine/actor/actor_category.hpp"

extern "C" {
#include "match.h"
#include <libgcc.h>
#include "engine/sys/system.h"
#include "engine/actor/actor.h"
#include "engine/gfx/gfx.h"
#include "game/level/level.h"
#include "globals.h"
#include "engine/util/math_util.h"
}

using Engine::Actor::ActorCategory;
using Game::Bosses::HovercraftFireball;
using Game::Entities::ActorSelf;
using Game::Entities::HpActor;

/* The hovercraft's fireball (#664 part 11h, include/game/bosses/boss_actors.hpp), ROM
 * 0x08032910-0x08032AF8, between src/game/vehicles/jetpack/jetpack_collected_wumpa.cpp
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
    SetState(HOVERCRAFT_FIREBALL_STATE_EXPLODE, 1);
}

/* The state method, then deletes itself once the explosion has played
 * through, or else the common update. */
void HovercraftFireball::Update()
{
    (this->*stateFuncs[state])();

    if (state == HOVERCRAFT_FIREBALL_STATE_EXPLODE && animDone != 0)
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
    velZ = HOVERCRAFT_FIREBALL_LAUNCH_SPEED_Z;
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
    delta -= HOVERCRAFT_FIREBALL_DRAG_Z;
    velZ = delta;
    if (delta <= HOVERCRAFT_FIREBALL_MIN_SPEED_Z - 1)
        velZ = HOVERCRAFT_FIREBALL_MIN_SPEED_Z;

    if ((u8)ActorCategory::IsTouchingPlayer(this)) {
        gActorList->AsHpActor()->Damage(6);
        palette = 4;
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        SetState(HOVERCRAFT_FIREBALL_STATE_EXPLODE, 1);
    }
}

/* Update's state dispatch, without the rest. */
void HovercraftFireball::RunState()
{
    (this->*stateFuncs[state])();
}

/* Exploding fireballs can't be shot. */
u8 HovercraftFireball::IsUnshootable()
{
    return exploding;
}
