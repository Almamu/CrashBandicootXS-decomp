#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "bosses.h"

/*
 * ROM 0x0817C2D0-0x0817C3FC. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* Airship::attacks (include/airship.hpp): the airship's attack
 * parameters (struct airship_attack, bosses.h).
 * SpawnAirship picks one by gAirshipLevel. */
const struct airship_attack gAirshipAttacks[6] = {
    { 30, 120, 1, 120, 15, 7, 90 },
    { 45, 90, 3, 120, 15, 8, 80 },
    { 60, 60, 5, 120, 15, 9, 70 },
    { 30, 120, 1, 150, 15, 5, 110 },
    { 40, 90, 3, 150, 15, 6, 100 },
    { 50, 60, 5, 150, 15, 7, 90 },
};

/* Airship::hitFlashPalettes: a 3-frame palette strip for BG palette 1.
 * SpawnAirship (airship.cpp) loads frame 0, AnimateAirshipPalette
 * (airship_graphics.cpp) ping-pongs through all three while its counter
 * runs. */
const u16 gAirshipHitFlashPalettes[3][16] = {
    {
        0x03E0, 0x30E7, 0x3549, 0x41AC, 0x46C5, 0x3222, 0x1DA0, 0x033F,
        0x02BF, 0x3AB9, 0x05F7, 0x0194, 0x5B3B, 0x29B0, 0x14BF, 0x7FFF,
    },
    {
        0x17E5, 0x458C, 0x49EE, 0x5651, 0x5B6A, 0x46C7, 0x3245, 0x17DF,
        0x175F, 0x4F5E, 0x1A9C, 0x1639, 0x6FDF, 0x3E55, 0x295F, 0x7FFF,
    },
    {
        0x2BEA, 0x5A31, 0x5E93, 0x6AF6, 0x6FEF, 0x5B6C, 0x46EA, 0x2BFF,
        0x2BFF, 0x63FF, 0x2F3F, 0x2ADE, 0x7FFF, 0x52FA, 0x3DFF, 0x7FFF,
    },
};

/* Airship::box: the boss's box (struct anim_box), read by AirshipStateExplode
 * (airship_states.cpp), SteerAirship (airship.cpp) and IsTouchingAirship
 * (airship_touch.cpp). */
const struct anim_box gAirshipBox = { -102, -12, -2, 51, 68, 4 };

/* Airship::keyframes: the two keyframes CreateAirship (airship.cpp) gives its tracker part. */
const struct anim_frame_record gAirshipKeyframes[2] = {
    { 64, 0, 4, 0, 0x0, { 0, 0 } },
    { 64, 0, 1, 0, 0x0, { 0, 0 } },
};
