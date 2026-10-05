#include "core.h"

/*
 * ROM 0x0816BB6C-0x0816BF20. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

struct entry_set
{
    const u32 (*entries)[2];
    u32 unk_04;
};

/* ResetEnemyCtrl's (enemy_ctrl.c) entry set and its entries. */
extern const u32 gEnemyCtrlMotionEntries[4][2];

const struct entry_set gEnemyCtrlMotionSet = {
    gEnemyCtrlMotionEntries, 0x100,
};

const u32 gEnemyCtrlMotionEntries[4][2] = {
    { 0, 0 },
    { 1, 0 },
    { 40, 40 },
    { 37, 38 },
};

/* Timer values per direction, CreateCrate (crate_create.c) and
 * UpdateSlotCrate (crate_break.c). */
const u8 gSlotCrateTimers[4] = {
    40, 30, 20, 10,
};

/* Per-object-kind flags of the collision system (22 kinds), read by
 * crate_hit.c, crate_stack.c, crate_break.c and game_loop47-49.c. */
const u8 gCrateKindCounted[22] = {
    1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
const u8 gCrateKindBreakable[22] = {
    1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
const u8 gCrateKindExplosive[22] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1,
};
const u8 gCrateKindUnbreakable[22] = {
    0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* QueueCratePlayerCollision (crate_break.c): the kind of each of 42 ids. */
const s32 gActionCtrlStateAttackKinds[42] = {
    1, 1, 1, 1, 1, 2, 1, 2, 5, 2, 2, 2, 3, 4,
    4, 4, 1, 1, 1, 1, 1, 1, 5, 1, 5, 5, 2, 1,
    2, 0, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 0,
};

/* QueueCratePlayerCollision and ApplyCrateCollision (crate_break.c): the collision response
 * code of each pair of kinds, [22][7]. */
const s32 gCrateHitResponse[22][7] = {
    { 0, 1, 3, 3, 3, 3, 3 },
    { 0, 0, 5, 5, 5, 5, 5 },
    { 0, 1, 3, 3, 3, 3, 3 },
    { 1, 1, 1, 1, 1, 1, 1 },
    { 0, 1, 2, 3, 3, 3, 3 },
    { 0, 0, 0, 0, 0, 0, 3 },
    { 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 2, 2, 1, 2, 2 },
    { 0, 1, 3, 3, 3, 3, 3 },
    { 4, 4, 4, 4, 4, 4, 4 },
    { 0, 1, 3, 3, 3, 3, 3 },
    { 0, 1, 2, 3, 3, 3, 3 },
    { 1, 1, 2, 2, 2, 3, 3 },
    { 0, 1, 2, 4, 4, 4, 4 },
    { 0, 1, 3, 3, 3, 3, 3 },
    { 0, 1, 3, 3, 3, 3, 3 },
    { 0, 1, 3, 3, 3, 3, 3 },
    { 0, 1, 3, 3, 3, 3, 3 },
    { 1, 1, 1, 4, 4, 4, 4 },
    { 1, 1, 1, 4, 4, 4, 4 },
    { 1, 1, 1, 4, 4, 4, 4 },
};

/* QueueCratePlayerCollision: a flag per kind. */
const u8 gStaticData_0816BF00[8] = {
    0, 0, 1, 0, 0, 0, 0, 0,
};

/* The {x, y, z} scale triples of UpdateExtraLifeHop (extra_life.c) and
 * UpdateWumpaHop (wumpa_update.c). */
const s32 gExtraLifeHopWidths[3] = {
    0x300, 0x0, 0x200,
};
const s32 gWumpaHopWidths[3] = {
    0x300, 0x0, 0x200,
};
