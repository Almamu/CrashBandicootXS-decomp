#include "boss_actors.hpp"

extern "C" {
#include "audio.h"
#include "actor.h"
#include "globals.h"
}

/* The airship's fireball (#664 part 11i, include/boss_actors.hpp): an
 * HpActor that the airship fires in volleys (AirshipStateFireballs,
 * SpawnAirshipFireball). It circles the point it was spawned at, then
 * spirals in on it (StateOrbit, StateSpiralIn, jetpack_plane.c), and
 * explodes when shot down. See
 * docs/matching/archive/issue-58-0x08030334-actor.md. */

/* Takes `amount` off the hit points; at zero, the explosion palette, a
 * sound, and state 2 (exploding) with animation 1. */
void AirshipFireball::Damage(s32 amount)
{
    hp -= amount;
    if (hp <= 0) {
        palette = 4;
        PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
        SetState(2, 1);
    }
}

/* The state method, then deletes itself once the explosion has played
 * through, or else the common update. */
void AirshipFireball::Update()
{
    (this->*stateFuncs[state])();

    if (state == 2 && animDone != 0)
        delete this;
    else
        ActorSelf::Update();
}

/* 2 hit points, centred on the spawn point, with no radius yet. */
AirshipFireball::AirshipFireball(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, y, z, 2)
{
    centerX = x;
    centerY = y;
    radius = 0;
    velZ = 0x95;
    exploding = 0;
}

void AirshipFireball::StateExplode()
{
    exploding = 1;
}

/* Update's state dispatch, without the rest. */
void AirshipFireball::RunState()
{
    (this->*stateFuncs[state])();
}

/* Exploding fireballs can't be shot. */
s32 AirshipFireball::IsUnshootable()
{
    return exploding;
}
