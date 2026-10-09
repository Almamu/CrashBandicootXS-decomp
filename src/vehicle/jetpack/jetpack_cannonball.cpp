#include "vehicle.hpp"
#include "boss_actors.hpp"
#include "audio.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "globals.h"
}

/* The jetpack cannonball (#664 part 11f, include/vehicle.hpp), ROM
 * 0x08030298-0x08030334, between jetpack_bomber.cpp and
 * airship_fireball.cpp: JetpackCannonball (gJetpackCannonballVtable), a
 * straight-line projectile that damages the player on contact. See
 * docs/matching/archive/issue-57-0x0802fbf0-actor.md. */

/* The jetpack player, the actor list's root. */
static inline HpActor *PlayerActor()
{
    return (HpActor *)gActorList;
}

/* Slot 2: flies by its velocity, falling; on the player it damages it
 * (2) and is gone, else ActorSelf's update. */
void JetpackCannonball::Update()
{
    x += velX;
    y += velY;
    z += -0x100;
    if ((u8)IsTouchingPlayer(this)) {
        PlayerActor()->Damage(2);
        delete this;
    } else {
        ActorSelf::Update();
    }
}

/* CreateJetpackCannonball: 1 hit point, flying at (velX, velY). */
JetpackCannonball::JetpackCannonball(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                                     s32 velX, s32 velY)
    : HpActor(rec, x, y, z, 1)
{
    this->velX = velX;
    this->velY = velY;
}

/* Slot 5: cannonballs can't be shot. */
s32 JetpackCannonball::IsUnshootable()
{
    return 1;
}
