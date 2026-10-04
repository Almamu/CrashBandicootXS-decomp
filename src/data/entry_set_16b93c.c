#include "core.h"

/*
 * ROM 0x0816B93C-0x0816B98C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The {entries, 0x100} entry set of entry_set_16b92c.c, followed by its
 * entries: the third HUD widget's, which PlayRoom (game_loop39.c)
 * builds through sub_800B69C. */
struct entry_set
{
    const u32 (*entries)[2];
    u32 unk_04;
};

extern const u32 gStaticData_0816B944[9][2];

const struct entry_set gStaticData_0816B93C = {
    gStaticData_0816B944, 0x100,
};

const u32 gStaticData_0816B944[9][2] = {
    { 0, 0 },
    { 1, 0 },
    { 2, 0 },
    { 1, 5 },
    { 2, 6 },
    { 1, 3 },
    { 2, 4 },
    { 7, 0 },
    { 8, 0 },
};
