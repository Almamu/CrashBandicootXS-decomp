#include "boss_actors.hpp"
#include "audio.hpp"

extern "C" {
#include "match.h"
#include "actor.h"
#include "globals.h"
}

/* The hovercraft's accessors and state changes (#664 part 11h). The
 * hovercraft is a bare AnimPart (gHovercraft, CreateHovercraft) and
 * globals (gHovercraftX, gHovercraftState, ...), like the airship; its
 * weapons (include/boss_actors.hpp) read it through the getters here.
 * The functions keep their C names. See
 * docs/matching/archive/issue-62-0x08033804-actor.md. */

/* Arms the hit flash (UpdateHovercraftHitFlash) unless it is running. */
void StartHovercraftHitFlash(void)
{
    if (gHovercraftHitFlashOn == 0 && gHovercraftHitFlashTimer == 0) {
        gHovercraftHitFlashTimer = 1;
        gHovercraftHitFlashOn = 1;
    }
}

/* Colour 15 of the hovercraft's BG and OBJ palettes (gFlashBgPalette,
 * gFlashObjPalette, iwram_data.cpp): white when `flag` is set, or else the
 * colour saved by the first call.
 *
 * Kept: the colour's r1 pin (the C had two more, on the pointers). The
 * ROM loads the white into r2 and copies it to r1; unpinned, the colour
 * gets other registers, the two BG stores are cross-jumped into one, with
 * every spelling tried (an inline for the two stores, a `u16` or an `s32`
 * colour). RunHovercraftState (hovercraft.cpp) has the same pin. */
void SetHovercraftFlashColor(u8 flag)
{
    MATCH_HOLD_REG(u16, val, r1);

    if (gHovercraftFlashColorSaved == 0) {
        gHovercraftFlashSavedColor = gFlashBgPalette[15];
        gHovercraftFlashColorSaved = 1;
    }

    if (flag != 0) {
        u16 *p = gFlashBgPalette;

        val = RGB_WHITE;
        p[15] = val;
    } else {
        u16 *p = gFlashBgPalette;

        val = gHovercraftFlashSavedColor;
        p[15] = val;
    }

    gFlashObjPalette[15] = val;
}

/* How many of its four weapons the hovercraft has left. */
s32 GetHovercraftPartsLeft(void)
{
    return gHovercraftPartsLeft;
}

/* One weapon less: the explosion sound, and once none is left, the
 * hovercraft falls (state 5). */
void LoseHovercraftPart(void)
{
    gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);

    gHovercraftPartsLeft -= 1;
    if (gHovercraftPartsLeft == 0) {
        gHovercraftGone = gHovercraftPartsLeft;
        SetHovercraftState(5, 0);
    }
}

/* The attack parameters (gHovercraftAttacks' record, SpawnHovercraft). */
const struct hovercraft_attack *GetHovercraftAttack(void)
{
    return gHovercraftAttack;
}

s32 GetHovercraftState(void)
{
    return gHovercraftState;
}

/* The level index CreateHovercraft caches (it picks the gHovercraftAttacks
 * record; the side guns test it against 0). */
s32 GetHovercraftLevel(void)
{
    return gHovercraftLevel;
}

s32 GetHovercraftZ(void)
{
    return gHovercraftZ;
}

s32 GetHovercraftY(void)
{
    return gHovercraftY;
}

s32 GetHovercraftX(void)
{
    return gHovercraftX;
}

/* EnterHovercraftState out of line. */
void SetHovercraftState(s32 a0, s32 a1)
{
    EnterHovercraftState(a0, a1);
}

/* gHovercraftStateFuncs[0]: the state CreateHovercraft sets before
 * SpawnHovercraft starts the fight (state 1), the twin of
 * AirshipStateInactive. Empty. */
void HovercraftStateInactive(void)
{
}

/* State 1: the hovercraft comes in at its Z speed; once close enough
 * (0x81FF), it stops sideways and closes in (state 2, animation 0). */
void HovercraftStateApproach(void)
{
    gHovercraftZ += gHovercraftVelZ;

    if (gHovercraftDistance <= 0x81FF) {
        s32 *velX = &gHovercraftVelX;
        s32 *velY = &gHovercraftVelY;

        *velY = 0;
        *velX = 0;
        EnterHovercraftState(2, 0);
    }
}

/* gHovercraftStateFuncs[4], the slot of gAirshipStateFuncs[4]'s
 * AirshipStateExplode. Empty and never entered: LoseHovercraftPart goes
 * straight to state 5 (HovercraftStateFall). */
void HovercraftStateExplodeStub(void)
{
}
