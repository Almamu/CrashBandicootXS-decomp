#include "boss_actors.hpp"
#include "audio.hpp"
#include "actor_category.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "globals.h"
}

using Engine::Actor::ActorCategory;
using Game::Vehicles::Jetpack::AirshipFireball;

/* The airship's fireball (#664 parts 11f and 11i,
 * include/boss_actors.hpp), ROM 0x08030334-0x080306AC: an HpActor that
 * the airship fires in volleys (AirshipStateFireballs,
 * AirshipFireball::Spawn). It circles the point it was spawned at, then
 * spirals in on it (StateOrbit, StateSpiralIn), and explodes when shot
 * down. See docs/matching/archive/issue-57-0x0802fbf0-actor.md and
 * issue-58-0x08030334-actor.md. */

/* gAirshipFireballStateFuncs[0]: climbs with a decaying Z step, widens
 * its orbit up to 0x2A00, eases its centre a 32nd of the way to a point
 * off the player, and sits on the circle at the angle `stateTime` gives.
 * Once near it goes to state 1 (keeping its angle); on the player it
 * damages it (6) and explodes (state 2). The centre, the target and the
 * player's position are separate locals, as in the C: written as one
 * expression, the registers move (the C also pinned three of them, which
 * g++ does not need). */
void AirshipFireball::StateOrbit()
{
    s32 angle;
    s32 t;

    z += velZ;
    if ((velZ -= AIRSHIP_FIREBALL_DRAG_Z) <= AIRSHIP_FIREBALL_MIN_SPEED_Z - 1) {
        velZ = AIRSHIP_FIREBALL_MIN_SPEED_Z;
    }
    if ((radius += AIRSHIP_FIREBALL_RADIUS_STEP) > AIRSHIP_FIREBALL_ORBIT_RADIUS) {
        radius = AIRSHIP_FIREBALL_ORBIT_RADIUS;
    }

    ActorSelf *player = gActorList;
    s32 px, py, tx, ty, cx, cy;
    const s16 *sine;

    /* `px` is needed: inlined, the instructions are reordered */
    px = player->x;
    cx = centerX;
    /* `tx` is needed: inlined, the code is 1 instruction shorter */
    tx = cx + -INT_TO_Q8(6);
    cx += (px - tx) >> AIRSHIP_FIREBALL_ORBIT_EASE_SHIFT;
    centerX = cx;
    /* `py` is needed: inlined, the instructions are reordered */
    py = player->y;
    cy = centerY;
    /* `ty` is needed: inlined, the code is 1 instruction longer */
    ty = cy + INT_TO_Q8(8);
    cy += (py - ty) >> AIRSHIP_FIREBALL_ORBIT_EASE_SHIFT;
    centerY = cy;
    sine = gSineTable;
    t = stateTime;
    angle = ((t << 5) >> 4) & 0xff;
    x = cx + Q8_MUL(sine[(angle + 0x40) & 0xff], radius);
    y = cy + Q8_MUL(sine[angle], radius);
    if (depth <= INT_TO_Q8(44) - 1) {
        SetState(AIRSHIP_FIREBALL_STATE_SPIRAL_IN, 0);
        stateTime = t;
    }
    if ((u8)ActorCategory::IsTouchingPlayer(this)) {
        gActorList->AsHpActor()->Damage(6);
        palette = 4;
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        SetState(AIRSHIP_FIREBALL_STATE_EXPLODE, 1);
    }
}

/* gAirshipFireballStateFuncs[1]: StateOrbit spiralling in, the radius
 * shrinking by 0x100 down to 0 and the centre easing a 16th of the way,
 * with no depth check. */
void AirshipFireball::StateSpiralIn()
{
    s32 angle;

    z += velZ;
    if ((velZ -= AIRSHIP_FIREBALL_DRAG_Z) <= AIRSHIP_FIREBALL_MIN_SPEED_Z - 1) {
        velZ = AIRSHIP_FIREBALL_MIN_SPEED_Z;
    }
    if ((radius += -AIRSHIP_FIREBALL_RADIUS_STEP) < 0) {
        radius = 0;
    }

    ActorSelf *player = gActorList;
    s32 px, py, tx, ty, cx, cy;
    const s16 *sine;

    /* `px` is needed: inlined, the instructions are reordered */
    px = player->x;
    cx = centerX;
    /* `tx` is needed: inlined, the instructions differ */
    tx = cx + -INT_TO_Q8(6);
    cx += (px - tx) >> AIRSHIP_FIREBALL_SPIRAL_EASE_SHIFT;
    centerX = cx;
    /* `py` is needed: inlined, the instructions are reordered */
    py = player->y;
    cy = centerY;
    /* `ty` is needed: inlined, the code is 1 instruction longer */
    ty = cy + INT_TO_Q8(8);
    cy += (py - ty) >> AIRSHIP_FIREBALL_SPIRAL_EASE_SHIFT;
    centerY = cy;
    sine = gSineTable;
    angle = ((stateTime << 5) >> 4) & 0xff;
    x = cx + Q8_MUL(sine[(angle + 0x40) & 0xff], radius);
    y = cy + Q8_MUL(sine[angle], radius);
    if ((u8)ActorCategory::IsTouchingPlayer(this)) {
        gActorList->AsHpActor()->Damage(6);
        palette = 4;
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        SetState(AIRSHIP_FIREBALL_STATE_EXPLODE, 1);
    }
}

/* Takes `amount` off the hit points; at zero, the explosion palette, a
 * sound, and state 2 (exploding) with animation 1. */
void AirshipFireball::Damage(s32 amount)
{
    hp -= amount;
    if (hp <= 0) {
        palette = 4;
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        SetState(AIRSHIP_FIREBALL_STATE_EXPLODE, 1);
    }
}

/* The state method, then deletes itself once the explosion has played
 * through, or else the common update. */
void AirshipFireball::Update()
{
    (this->*stateFuncs[state])();

    if (state == AIRSHIP_FIREBALL_STATE_EXPLODE && animDone != 0)
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
    velZ = AIRSHIP_FIREBALL_LAUNCH_SPEED_Z;
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
u8 AirshipFireball::IsUnshootable()
{
    return exploding;
}
