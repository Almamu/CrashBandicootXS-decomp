#include "hovercraft.hpp"
#include "audio.hpp"

extern "C" {
#include "actor.h"
#include "globals.h"
}

/* The hovercraft's accessors and state changes (#664 part 11h). The
 * hovercraft is a bare AnimPart (anim, Create) and variables (x, state,
 * ...), like the airship, all static members of Hovercraft
 * (include/hovercraft.hpp, #772); its weapons (include/boss_actors.hpp)
 * read it through the getters here. The members keep their C names
 * (cxx_symbols.txt). See
 * docs/matching/archive/issue-62-0x08033804-actor.md. */

/* Arms the hit flash (UpdateHovercraftHitFlash) unless it is running. */
void Hovercraft::StartHitFlash(void)
{
    if (hitFlashOn == 0 && hitFlashTimer == 0) {
        hitFlashTimer = 1;
        hitFlashOn = 1;
    }
}

/* Colour 15 of the hovercraft's palettes, white or the saved colour
 * (ApplyHovercraftFlashColor, include/boss_actors.hpp). */
void Hovercraft::SetFlashColor(u8 flag)
{
    ApplyFlashColor(flag);
}

/* How many of its four weapons the hovercraft has left. */
s32 Hovercraft::GetPartsLeft(void)
{
    return partsLeft;
}

/* One weapon less: the explosion sound, and once none is left, the
 * hovercraft falls (state 5). */
void Hovercraft::LosePart(void)
{
    gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);

    partsLeft -= 1;
    if (partsLeft == 0) {
        gone = partsLeft;
        SetState(5, 0);
    }
}

/* The attack parameters (gHovercraftAttacks' record, SpawnHovercraft). */
const struct hovercraft_attack *Hovercraft::GetAttack(void)
{
    return attack;
}

s32 Hovercraft::GetState(void)
{
    return state;
}

/* The level index CreateHovercraft caches (it picks the gHovercraftAttacks
 * record; the side guns test it against 0). */
s32 Hovercraft::GetLevel(void)
{
    return level;
}

s32 Hovercraft::GetZ(void)
{
    return z;
}

s32 Hovercraft::GetY(void)
{
    return y;
}

s32 Hovercraft::GetX(void)
{
    return x;
}

/* EnterHovercraftState out of line. */
void Hovercraft::SetState(s32 a0, s32 a1)
{
    EnterState(a0, a1);
}

/* gHovercraftStateFuncs[0]: the state CreateHovercraft sets before
 * SpawnHovercraft starts the fight (state 1), the twin of
 * AirshipStateInactive. Empty. */
void Hovercraft::StateInactive(void)
{
}

/* State 1: the hovercraft comes in at its Z speed; once close enough
 * (0x81FF), it stops sideways and closes in (state 2, animation 0). */
void Hovercraft::StateApproach(void)
{
    z += velZ;

    if (distance <= 0x81FF) {
        s32 *vx = &velX;
        s32 *vy = &velY;

        *vy = 0;
        *vx = 0;
        EnterState(2, 0);
    }
}

/* gHovercraftStateFuncs[4], the slot of gAirshipStateFuncs[4]'s
 * AirshipStateExplode. Empty and never entered: LoseHovercraftPart goes
 * straight to state 5 (HovercraftStateFall). */
void Hovercraft::StateExplodeStub(void)
{
}
