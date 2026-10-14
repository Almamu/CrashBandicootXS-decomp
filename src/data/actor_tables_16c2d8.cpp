extern "C" {
#include "core.h"
#include "game/bosses/bosses.h"
#include "engine/util/math_util.h"
}

/*
 * ROM 0x0816C2D8-0x0816C418. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The 12-byte vector records SetMegaMixMotionYFromSet and its siblings
 * (mega_mix.cpp) look up by type id. */
const struct speed_ramp gMegaMixMotionRecords[4] = {
    { 0, 0, 0 },
    { 0, Q8(0.125), MEGA_MIX_RUN_SPEED },
    { 0, Q8(0.125), MEGA_MIX_CATCH_UP_SPEED },
    { MEGA_MIX_RUN_SPEED, Q8(0.125), MEGA_MIX_TURN_BACK_SPEED },
};

/* UpdateTiny / SetTinyState (tiny.cpp): a value per round. */
const u8 gTinyRoundAnchors[3] = {
    4, 1, 0,
};

/* PickTinyHopTarget (tiny.cpp): a sequence of 0-4 values. */
const u8 gTinyHopTargets[77] = {
    1, 1, 2, 1, 1, 0, 2, 0, 3, 3, 0, 0, 0, 3, 3, 1,
    1, 2, 4, 4, 3, 3, 3, 3, 3, 1, 1, 2, 1, 1, 0, 2,
    0, 3, 3, 0, 0, 0, 3, 3, 1, 1, 2, 2, 3, 3, 3, 3,
    3, 3, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 0, 0,
};

/* The Neo Cortex fight's crosshair (gCortexTargetVtable). SetCortexTargetDest
 * (cortex.cpp) glides it to a new point in this many steps,
 * indexed by the level config's index. */
const u8 gCortexTargetHopSteps[4] = {
    0x10, 0xE, 0xA, 0x20,
};

/* UpdateCortexTarget (cortex.cpp), per config index, while the
 * crosshair chases the player (state 5): the glide step count, and the
 * countdown values at which it starts blinking (with SFX_CORTEX_TARGET_BLINK) and stops
 * blinking on frame 1, before it fires. */
const u8 gCortexTargetChaseSteps[3] = {
    24, 22, 18,
};
const u8 gCortexTargetBlinkStartTimes[3] = {
    4, 4, 4,
};
const u8 gCortexTargetBlinkStopTimes[3] = {
    2, 2, 2,
};

/* UpdateDingodile (dingodile.cpp): the X positions (Q8) at which
 * Dingodile, walking left (`facing`) or right, stops (state 5, motion 0)
 * if the player is less than 32 px ahead; the Hurt tables replace the
 * others once he has been hit. */
const s32 gDingodileStopXLeft[4] = {
    INT_TO_Q8(240), INT_TO_Q8(160), INT_TO_Q8(75), 0,
};
const s32 gDingodileStopXLeftHurt[6] = {
    INT_TO_Q8(260), INT_TO_Q8(210), INT_TO_Q8(160), INT_TO_Q8(110), INT_TO_Q8(75), 0,
};
const s32 gDingodileStopXRight[4] = {
    INT_TO_Q8(75), INT_TO_Q8(160), INT_TO_Q8(240), INT_TO_Q8(1024),
};
const s32 gDingodileStopXRightHurt[6] = {
    INT_TO_Q8(75), INT_TO_Q8(110), INT_TO_Q8(160), INT_TO_Q8(210), INT_TO_Q8(250), INT_TO_Q8(1024),
};

/* Motion records (`struct speed_ramp`, objects.h): UpdateDingodileShark
 * (dingodile.cpp), and StartDingodileMotion (dingodile_create.cpp) through the
 * entries of entry_set_16c418.cpp. */
const struct speed_ramp gDingodileMotionRecords[4] = {
    { 0, 0, 0 },
    { 0, DINGODILE_WALK_ACCEL, DINGODILE_WALK_SPEED },
    { -DINGODILE_HIT_RECOIL_SPEED, DINGODILE_HIT_RECOIL_DECEL, 0 },
    { 0, Q8(1.125), Q8(1.125) },
};

/* The Y motion records (StartCtrlTargetMotionY, {speed, accel, limit})
 * UpdateDingodileProjectile (dingodile.cpp) starts: the rocket's rise
 * (state 0, until it reaches the top and drops the stalactite) and the
 * stalactite's fall (state 5). Only the first three words of the second
 * table are read. */
const struct speed_ramp gDingodileRocketRiseMotion = {
    0, DINGODILE_ROCKET_RISE_ACCEL, -INT_TO_Q8(4),
};
const struct speed_ramp gDingodileStalactiteFallMotion[3] = {
    { 0, DINGODILE_STALACTITE_FALL_ACCEL, Q8(2.875) },
    { -INT_TO_Q8(4), Q8(0.25), INT_TO_Q8(4) },
    { -DINGODILE_STALACTITE_UNK_START, DINGODILE_STALACTITE_UNK_STEP, 0 },
};
