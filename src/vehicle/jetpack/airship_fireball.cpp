#include "boss_actors.hpp"
#include "audio.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "globals.h"
}

/* The airship's fireball (#664 parts 11f and 11i,
 * include/boss_actors.hpp), ROM 0x08030334-0x080306AC: an HpActor that
 * the airship fires in volleys (AirshipStateFireballs,
 * SpawnAirshipFireball). It circles the point it was spawned at, then
 * spirals in on it (StateOrbit, StateSpiralIn), and explodes when shot
 * down. See docs/matching/archive/issue-57-0x0802fbf0-actor.md and
 * issue-58-0x08030334-actor.md. */

/* The jetpack player, the actor list's root. */
static inline HpActor *PlayerActor()
{
    return (HpActor *)gActorList;
}

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
    if ((velZ -= 5) <= 0x13) {
        velZ = 0x14;
    }
    if ((radius += 0x100) > 0x2a00) {
        radius = 0x2a00;
    }
    {
        ActorSelf *player = gActorList;
        s32 px, py, tx, ty, cx, cy;
        const s16 *sine;

        px = player->x;
        cx = centerX;
        tx = cx + -0x600;
        cx += (px - tx) >> 5;
        centerX = cx;
        py = player->y;
        cy = centerY;
        ty = cy + 0x800;
        cy += (py - ty) >> 5;
        centerY = cy;
        sine = gSineTable;
        t = stateTime;
        angle = ((t << 5) >> 4) & 0xff;
        x = cx + Q8_MUL(sine[(angle + 0x40) & 0xff], radius);
        y = cy + Q8_MUL(sine[angle], radius);
    }
    if (depth <= 0x2bff) {
        SetState(1, 0);
        stateTime = t;
    }
    if ((u8)IsTouchingPlayer(this)) {
        PlayerActor()->Damage(6);
        palette = 4;
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        SetState(2, 1);
    }
}

/* gAirshipFireballStateFuncs[1]: StateOrbit spiralling in, the radius
 * shrinking by 0x100 down to 0 and the centre easing a 16th of the way,
 * with no depth check. */
void AirshipFireball::StateSpiralIn()
{
    s32 angle;

    z += velZ;
    if ((velZ -= 5) <= 0x13) {
        velZ = 0x14;
    }
    if ((radius += -0x100) < 0) {
        radius = 0;
    }
    {
        ActorSelf *player = gActorList;
        s32 px, py, tx, ty, cx, cy;
        const s16 *sine;

        px = player->x;
        cx = centerX;
        tx = cx + -0x600;
        cx += (px - tx) >> 4;
        centerX = cx;
        py = player->y;
        cy = centerY;
        ty = cy + 0x800;
        cy += (py - ty) >> 4;
        centerY = cy;
        sine = gSineTable;
        angle = ((stateTime << 5) >> 4) & 0xff;
        x = cx + Q8_MUL(sine[(angle + 0x40) & 0xff], radius);
        y = cy + Q8_MUL(sine[angle], radius);
    }
    if ((u8)IsTouchingPlayer(this)) {
        PlayerActor()->Damage(6);
        palette = 4;
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        SetState(2, 1);
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
