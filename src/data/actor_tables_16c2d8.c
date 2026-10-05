#include "core.h"

/*
 * ROM 0x0816C2D8-0x0816C418. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The 12-byte vector records SetMegaMixMotionYFromSet and its siblings
 * (actor_part27b.c) look up by type id. */
const s32 gMegaMixMotionRecords[4][3] = {
    { 0, 0, 0 },
    { 0, 32, 450 },
    { 0, 32, 620 },
    { 450, 32, 750 },
};

/* UpdateTiny / SetTinyState (actor_part_18008.c): a value per round. */
const u8 gTinyRoundAnchors[3] = {
    4, 1, 0,
};

/* PickTinyHopTarget (actor_part_18008.c): a sequence of 0-4 values. */
const u8 gTinyHopTargets[77] = {
    1, 1, 2, 1, 1, 0, 2, 0, 3, 3, 0, 0, 0, 3, 3, 1,
    1, 2, 4, 4, 3, 3, 3, 3, 3, 1, 1, 2, 1, 1, 0, 2,
    0, 3, 3, 0, 0, 0, 3, 3, 1, 1, 2, 2, 3, 3, 3, 3,
    3, 3, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 0, 0,
};

/* The Neo Cortex fight's crosshair (gCortexTargetVtable). SetCortexTargetDest
 * (actor_part_1967c.c) glides it to a new point in this many steps,
 * indexed by the level config's index. */
const u8 gCortexTargetHopSteps[4] = {
    0x10, 0xE, 0xA, 0x20,
};

/* UpdateCortexTarget (actor_part_188d0.c), per config index, while the
 * crosshair chases the player (state 5): the glide step count, and the
 * countdown values at which it starts blinking (with sfx 0x5C) and stops
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

/* UpdateDingodile (actor_part_1967c.c): the X positions (Q8) at which
 * Dingodile, walking left (`facing`) or right, stops (state 5, motion 0)
 * if the player is within 0x1FFF ahead; the Hurt tables replace the
 * others once he has been hit. */
const s32 gDingodileStopXLeft[4] = {
    0xF000, 0xA000, 0x4B00, 0x0,
};
const s32 gDingodileStopXLeftHurt[6] = {
    0x10400, 0xD200, 0xA000, 0x6E00, 0x4B00, 0x0,
};
const s32 gDingodileStopXRight[4] = {
    0x4B00, 0xA000, 0xF000, 0x40000,
};
const s32 gDingodileStopXRightHurt[6] = {
    0x4B00, 0x6E00, 0xA000, 0xD200, 0xFA00, 0x40000,
};

/* {x, y, z} vectors (gobj_1a794.h's `struct vec3`): UpdateDingodileShark
 * (actor_part_1967c.c), and StartDingodileMotion (actor_part_1a794.c) through the
 * entries of entry_set_16c418.c. */
const s32 gDingodileMotionRecords[4][3] = {
    { 0, 0, 0 },
    { 0, 180, 300 },
    { -400, 30, 0 },
    { 0, 288, 288 },
};

/* The Y motion records (StartCtrlTargetMotionY, {speed, accel, limit})
 * UpdateDingodileProjectile (actor_part_1967c.c) starts: the rocket's rise
 * (state 0, until it reaches the top and drops the stalactite) and the
 * stalactite's fall (state 5). Only the first three words of the second
 * table are read. */
const s32 gDingodileRocketRiseMotion[3] = {
    0, 7, -1024,
};
const s32 gDingodileStalactiteFallMotion[9] = {
    0, 4, 736, -1024, 64, 1024, -600, 10, 0,
};
