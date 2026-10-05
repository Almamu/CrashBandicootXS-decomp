#include "core.h"

/*
 * ROM 0x0816B92C-0x0816B93C: two entry sets. Linked in ROM order between
 * data/data.s sections by ldscript.txt - see docs/data.md.
 */

/* The {entries, 0x100} shape of the entry sets in entry_set_16c418.c
 * (gobj_1a794.h's `struct mover` `set`, +0x04); these two are stored at
 * the same +0x04 of their objects, but what their entries hold hasn't
 * been traced. */
struct entry_set
{
    const void *entries;
    u32 unk_04;
};

/* Their entries, in motion_records_16b304.c. */
extern const u32 gStaticData_0816B514[][2];
extern const u32 gStaticData_0816B790[][2];

/* The sets PlayRoom (game_loop39.c) gives the two HUD widgets it
 * builds, through SetCtrlAnimSet (which stores them at +0x04). */
const struct entry_set gStaticData_0816B92C = {
    gStaticData_0816B514, 0x100,
};

const struct entry_set gStaticData_0816B934 = {
    gStaticData_0816B790, 0x100,
};
