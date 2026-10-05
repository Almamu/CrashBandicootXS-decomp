#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0816C250-0x0816C2D8: two per-state member-function-pointer
 * tables of the player-controller objects, then an entry set. Linked in
 * ROM order between data/data.s sections by ldscript.txt - see
 * docs/data.md.
 */

extern void sub_8016B1C();
extern void sub_8016C08();
extern void sub_8016C94();
extern void sub_8016D5C();
extern void sub_8016DDC();
extern void sub_8017044();
extern void sub_80170EC();
extern void sub_8017184();
extern void sub_8017600();
extern void sub_80178EC();
extern void sub_801793C();
extern void sub_801796C();

/* Per-state handlers dispatched by PlayerCtrlKillPlayer (actor_part_16048.c,
 * its `struct pmf` view). */
const struct actor_pmf gStaticData_0816C250[8] = {
    ACTOR_PMF(sub_8016B1C),
    ACTOR_PMF(sub_8016C08),
    ACTOR_PMF(sub_8016C94),
    ACTOR_PMF(sub_8016D5C),
    ACTOR_PMF(sub_8016DDC),
    ACTOR_PMF(sub_80170EC),
    ACTOR_PMF(sub_8017044),
    ACTOR_PMF(sub_8017184),
};

/* Per-state handlers dispatched by UpdateInputCtrl (actor_part_17524.c). */
const struct actor_pmf gStaticData_0816C290[4] = {
    ACTOR_PMF(sub_8017600),
    ACTOR_PMF(sub_801796C),
    ACTOR_PMF(sub_801793C),
    ACTOR_PMF(sub_80178EC),
};

/* An entry set as gobj_1a794.h's `struct mover` points at it (`set`,
 * +0x04): the {a, b} entries (`struct vec_pair`) and a word the code
 * doesn't read, 0x100 in every set in the ROM. actor_part27b.c stores
 * gStaticData_0816C2D0 there. */
struct entry_set
{
    const u32 (*entries)[2];
    u32 unk_04;
};

/* gStaticData_0816C2D0's four entries. */
const u32 gStaticData_0816C2B0[4][2] = {
    { 0, 0 },
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
};

const struct entry_set gStaticData_0816C2D0 = {
    gStaticData_0816C2B0, 0x100,
};
