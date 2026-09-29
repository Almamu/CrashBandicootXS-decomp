#include "core.h"

/*
 * ROM 0x0816B98C-0x0816BB6C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The 8-word tables the text popups of graphics_loading_1ef0c.c,
 * graphics_loading_1fdec.c and graphics_loading_1feec.c hand their
 * header (text_popup.h's `gfx`, +0x84): a small index 0-5 per slot, 8 in
 * the unused ones. gStaticData_0816B98C is the one every popup starts
 * with. */
const s32 gStaticData_0816B98C[8] = { 0, 1, 8, 8, 8, 8, 8, 8 };
const s32 gStaticData_0816B9AC[8] = { 0, 3, 8, 2, 1, 4, 8, 8 };
const s32 gStaticData_0816B9CC[8] = { 2, 8, 8, 0, 1, 3, 8, 8 };
const s32 gStaticData_0816B9EC[8] = { 3, 8, 0, 4, 1, 2, 8, 5 };
const s32 gStaticData_0816BA0C[8] = { 0, 8, 1, 8, 8, 8, 8, 8 };
const s32 gStaticData_0816BA2C[8] = { 0, 8, 1, 8, 8, 8, 8, 8 };
const s32 gStaticData_0816BA4C[8] = { 1, 0, 8, 8, 8, 8, 8, 8 };
const s32 gStaticData_0816BA6C[8] = { 2, 1, 8, 8, 8, 8, 0, 8 };
const s32 gStaticData_0816BA8C[8] = { 2, 0, 8, 8, 1, 8, 0, 8 };
const s32 gStaticData_0816BAAC[8] = { 1, 8, 8, 2, 3, 0, 8, 8 };
const s32 gStaticData_0816BACC[8] = { 1, 8, 8, 0, 0, 1, 8, 8 };
const s32 gStaticData_0816BAEC[8] = { 5, 2, 8, 1, 3, 4, 0, 8 };
const s32 gStaticData_0816BB0C[8] = { 1, 0, 8, 8, 8, 8, 8, 8 };
const s32 gStaticData_0816BB2C[8] = { 0, 8, 8, 1, 1, 0, 8, 8 };
const s32 gStaticData_0816BB4C[8] = { 2, 2, 8, 3, 3, 1, 3, 8 };
