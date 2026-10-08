#include "vehicle.hpp"

extern "C" {
#include "actor.h"
#include "bosses.h"
}

/* The jetpack player's shot, JetpackShot (#664 part 11e,
 * include/vehicle.hpp). See
 * docs/matching/archive/issue-56-0x0802f0dc-actor.md. */

/* Slot 2: flies by its speed (falling a little) into the screen, and is
 * gone once it hits a shootable actor (2 damage), the airship (2 damage)
 * or the far depth 0x8200; until then ActorSelf's update. */
void JetpackShot::Update()
{
    x += velX;
    y += -0xc0 + velY;
    z += 0x400;

    HpActor *hit = FindShotTarget(this);

    if (hit != 0) {
        hit->Damage(2);
        delete this;
    } else if (IsTouchingAirship(this)) {
        DamageAirship(2);
        delete this;
    } else if (depth > 0x8200) {
        delete this;
    } else {
        ActorSelf::Update();
    }
}

/* CreateJetpackShot: 1 hit point, flying at (velX, velY). */
JetpackShot::JetpackShot(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 velX,
                         s32 velY)
    : HpActor(rec, x, y, z, 1)
{
    this->velX = velX;
    this->velY = velY;
}

/* Slot 5: its own shots can't be shot. */
s32 JetpackShot::IsUnshootable()
{
    return 1;
}
