#include "core.h"
#include "pickups.h"
#include "enemies.h"
#include "crates.h"
#include "player.h"
#include "objects.h"

/*
 * ROM 0x0816BB6C-0x0816BF20. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* ResetEnemyCtrl's (enemy_ctrl.cpp) entry set and its entries. */
extern const u32 gEnemyCtrlMotionEntries[4][2];

const struct entry_set gEnemyCtrlMotionSet = {
    gEnemyCtrlMotionEntries,
    0x100,
};

const u32 gEnemyCtrlMotionEntries[4][2] = {
    { 0, 0 },
    { 1, 0 },
    { 40, 40 },
    { 37, 38 },
};

/* Timer values per direction, CreateCrate (crate_create.cpp) and
 * UpdateSlotCrate (crate_states.cpp). */
const u8 gSlotCrateTimers[4] = {
    40,
    30,
    20,
    10,
};

/* Per-object-kind flags of the collision system (22 kinds), read by
 * crate_hit.cpp, crate_fields.cpp, crate_break.cpp, crate_switches.cpp and
 * crate_states.cpp. */
const u8 gCrateKindCounted[CRATE_KIND_COUNT] = {
    1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
const u8 gCrateKindBreakable[CRATE_KIND_COUNT] = {
    1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
const u8 gCrateKindExplosive[CRATE_KIND_COUNT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1,
};
const u8 gCrateKindUnbreakable[CRATE_KIND_COUNT] = {
    0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* QueueCratePlayerCollision (crate_break.cpp): the attack kind of each action
 * controller state. */
const s32 gActionCtrlStateAttackKinds[ACTION_STATE_COUNT] = {
    [ACTION_STATE_IDLE] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_UNUSED_IDLE] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_NOP2] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_RUN] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_TURBO_RUN] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_JUMP] = ATTACK_KIND_JUMP,
    [ACTION_STATE_NOP6] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_AIRBORNE_JUMP] = ATTACK_KIND_JUMP,
    [ACTION_STATE_BODY_SLAM_START] = ATTACK_KIND_BODY_SLAM,
    [ACTION_STATE_AIRBORNE_FLIP_JUMP] = ATTACK_KIND_JUMP,
    [ACTION_STATE_FLIP_BODY_SLAM_START] = ATTACK_KIND_JUMP,
    [ACTION_STATE_AIRBORNE_HIGH_JUMP] = ATTACK_KIND_JUMP,
    [ACTION_STATE_SLIDE] = ATTACK_KIND_SLIDE,
    [ACTION_STATE_SPIN] = ATTACK_KIND_SPIN,
    [ACTION_STATE_AIR_SPIN] = ATTACK_KIND_SPIN,
    [ACTION_STATE_TORNADO_SPIN] = ATTACK_KIND_SPIN,
    [ACTION_STATE_CROUCH_DOWN] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_CROUCH] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_STAND_UP] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_CRAWL_START] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_CRAWL] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_CRAWL_STAND_UP] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_BODY_SLAM_LAND] = ATTACK_KIND_BODY_SLAM,
    [ACTION_STATE_LAND] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_AIRBORNE_BODY_SLAM] = ATTACK_KIND_BODY_SLAM,
    [ACTION_STATE_AIRBORNE_SUPER_BODY_SLAM] = ATTACK_KIND_BODY_SLAM,
    [ACTION_STATE_AIRBORNE_FALL] = ATTACK_KIND_JUMP,
    [ACTION_STATE_CRAWL_STOP] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_LEFT_GROUND] = ATTACK_KIND_JUMP,
    [ACTION_STATE_DYING] = ATTACK_KIND_NONE,
    [ACTION_STATE_WARP_OUT] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_HANG_GRAB] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_HANG] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_HANG_SPIN] = ATTACK_KIND_SPIN,
    [ACTION_STATE_UNUSED_HANG] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_UNUSED_HANG_GRAB] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_RELEASE_HANG] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_HANG_MOVE_START] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_HANG_MOVE] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_UNUSED_HANG_RELEASE] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_HANG_STOP] = ATTACK_KIND_TOUCH,
    [ACTION_STATE_WARP_IN] = ATTACK_KIND_NONE,
};

/* QueueCratePlayerCollision and ApplyCrateCollision (crate_break.cpp): the collision response
 * code of each crate kind (row) against each attack kind (column,
 * ATTACK_KIND_*). */
const s32 gCrateHitResponse[CRATE_KIND_COUNT][7] = {
    [CRATE_KIND_BASIC] = { 0, 1, 3, 3, 3, 3, 3 },
    [CRATE_KIND_CHECKPOINT] = { 0, 0, 5, 5, 5, 5, 5 },
    [CRATE_KIND_AKU_AKU] = { 0, 1, 3, 3, 3, 3, 3 },
    [CRATE_KIND_IRON_SWITCH] = { 1, 1, 1, 1, 1, 1, 1 },
    [CRATE_KIND_ARROW] = { 0, 1, 2, 3, 3, 3, 3 },
    [CRATE_KIND_OUTLINE] = { 0, 0, 0, 0, 0, 0, 3 },
    [CRATE_KIND_NITRO_SWITCH] = { 1, 1, 1, 1, 1, 1, 1 },
    [CRATE_KIND_IRON] = { 1, 1, 1, 1, 1, 1, 1 },
    [CRATE_KIND_IRON_ARROW] = { 1, 1, 2, 2, 1, 2, 2 },
    [CRATE_KIND_LIFE] = { 0, 1, 3, 3, 3, 3, 3 },
    [CRATE_KIND_NITRO] = { 4, 4, 4, 4, 4, 4, 4 },
    [CRATE_KIND_MYSTERY] = { 0, 1, 3, 3, 3, 3, 3 },
    [CRATE_KIND_BOUNCY_WUMPA] = { 0, 1, 2, 3, 3, 3, 3 },
    [CRATE_KIND_REINFORCED] = { 1, 1, 2, 2, 2, 3, 3 },
    [CRATE_KIND_TNT] = { 0, 1, 2, 4, 4, 4, 4 },
    [CRATE_KIND_SLOT] = { 0, 1, 3, 3, 3, 3, 3 },
    [CRATE_KIND_TIME_1] = { 0, 1, 3, 3, 3, 3, 3 },
    [CRATE_KIND_TIME_2] = { 0, 1, 3, 3, 3, 3, 3 },
    [CRATE_KIND_TIME_3] = { 0, 1, 3, 3, 3, 3, 3 },
    [CRATE_KIND_TNT_LIT_1] = { 1, 1, 1, 4, 4, 4, 4 },
    [CRATE_KIND_TNT_LIT_2] = { 1, 1, 1, 4, 4, 4, 4 },
    [CRATE_KIND_TNT_LIT_3] = { 1, 1, 1, 4, 4, 4, 4 },
};

/* QueueCratePlayerCollision: per attack kind (gActionCtrlStateAttackKinds),
 * whether its crate breaks go through the player's `countdown` limiter
 * (ApplyCrateCollision -> BreakCrateInStack). Only ATTACK_KIND_JUMP. */
const u8 gAttackKindBreakLimited[8] = {
    [ATTACK_KIND_JUMP] = 1,
};

/* The {x, y, z} scale triples of UpdateExtraLifeHop (extra_life.cpp) and
 * UpdateWumpaHop (wumpa_update.cpp). */
const s32 gExtraLifeHopWidths[3] = {
    0x300,
    0x0,
    0x200,
};
const s32 gWumpaHopWidths[3] = {
    0x300,
    0x0,
    0x200,
};
