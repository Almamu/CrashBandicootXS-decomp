#include "boss_actors.hpp"
#include "audio.hpp"

extern "C" {
#include "globals.h"
}

/* The airship (#664 part 11i, include/boss_actors.hpp), hit by the
 * player's shots. See docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * Takes `delta` off the airship's hit points and starts the hit flash
 * (gAirshipHitFlashTimer). While it has hit points left, a hit sound;
 * at zero, it stops and enters state 4 (exploding) with animation 1. */
void DamageAirship(s32 delta)
{
    s32 remaining = gAirshipHp - delta;

    gAirshipHp = remaining;
    gAirshipHitFlashTimer = 0x12;

    if (remaining <= 0) {
        gAirshipHp = 0;
        gAirshipVelX = 0;
        gAirshipVelY = 0;
        gAirshipVelZ = 0xaa;
        SetAirshipState(4, 1);
    } else {
        gAudioContext->PlaySfx(SFX_AIRSHIP_HIT, 0x100);
    }
}
