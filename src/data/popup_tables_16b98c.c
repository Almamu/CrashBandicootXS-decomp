#include "core.h"

/*
 * ROM 0x0816B98C-0x0816BB6C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The 8-word tables the text popups of graphics_loading_1ef0c.c,
 * graphics_loading_1fdec.c and graphics_loading_1feec.c hand their
 * header (text_popup.h's `gfx`, +0x84): a small index 0-5 per slot, 8 in
 * the unused ones. gEnemyDefaultAnimMap is the one every popup starts
 * with. */
const s32 gEnemyDefaultAnimMap[8] = { 0, 1, 8, 8, 8, 8, 8, 8 };
const s32 gPenguinAnimMap[8] = { 0, 3, 8, 2, 1, 4, 8, 8 };
const s32 gPufferfishAnimMap[8] = { 2, 8, 8, 0, 1, 3, 8, 8 };
const s32 gBlowgunTribesmanAnimMap[8] = { 3, 8, 0, 4, 1, 2, 8, 5 };
const s32 gVenusFlytrapAnimMap[8] = { 0, 8, 1, 8, 8, 8, 8, 8 };
const s32 gVultureAnimMap[8] = { 0, 8, 1, 8, 8, 8, 8, 8 };
const s32 gSharkAnimMap[8] = { 1, 0, 8, 8, 8, 8, 8, 8 };
const s32 gSquidAnimMap[8] = { 2, 1, 8, 8, 8, 8, 0, 8 };
const s32 gElectricEelAnimMap[8] = { 2, 0, 8, 8, 1, 8, 0, 8 };
const s32 gFlamethrowerLabAssistantAnimMap[8] = { 1, 8, 8, 2, 3, 0, 8, 8 };
const s32 gStationarySpaceEnemyAnimMap[8] = { 1, 8, 8, 0, 0, 1, 8, 8 };
const s32 gPatrollingSpaceEnemyAnimMap[8] = { 5, 2, 8, 1, 3, 4, 0, 8 };
const s32 gPatrollingSewerEnemyAnimMap[8] = { 1, 0, 8, 8, 8, 8, 8, 8 };
const s32 gCrusherAnimMap[8] = { 0, 8, 8, 1, 1, 0, 8, 8 };
const s32 gSaucerLabAssistantAnimMap[8] = { 2, 2, 8, 3, 3, 1, 3, 8 };
