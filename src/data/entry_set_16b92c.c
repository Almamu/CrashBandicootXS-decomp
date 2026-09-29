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

/* Entries inside the still-raw gStaticData_0816B304/gStaticData_0816B61C. */
extern const u8 gStaticData_0816B304[];
extern const u8 gStaticData_0816B61C[];

/* The sets sub_802375C (game_loop39.c) gives the two HUD widgets it
 * builds, through sub_800B69C (which stores them at +0x04). */
const struct entry_set gStaticData_0816B92C = {
    gStaticData_0816B304 + 0x210, 0x100,
};

const struct entry_set gStaticData_0816B934 = {
    gStaticData_0816B61C + 0x174, 0x100,
};
