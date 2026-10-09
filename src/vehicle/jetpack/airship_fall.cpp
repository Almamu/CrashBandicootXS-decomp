#include "boss_actors.hpp"

/* The airship (#664 part 11i, include/boss_actors.hpp). See
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * gAirshipStateFuncs[5], after the explosion: the airship falls (its Y
 * speed grows by 7 a frame up to 0x140) until it is past 0xBB80, then
 * goes back to state 0 (inactive) with animation 0 and BG2 off. */
void AirshipStateFall(void)
{
    s32 total;
    s32 delta;

    gAirshipX += gAirshipVelX;

    total = gAirshipY + gAirshipVelY;
    gAirshipY = total;

    gAirshipZ += gAirshipVelZ;

    delta = gAirshipVelY + 7;
    gAirshipVelY = delta;
    if (delta > 0x140) {
        gAirshipVelY = 0x140;
    }

    if (total > 0xbb80) {
        SetAirshipState(0, 0);
        REG_DISPCNT &= 0xfbff;
    }
}
