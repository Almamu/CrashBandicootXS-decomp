#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0816C250-0x0816C2D8: two per-state member-function-pointer
 * tables of the player-controller objects, then an entry set. Linked in
 * ROM order between data/data.s sections by ldscript.txt - see
 * docs/data.md.
 */

extern void PlayerCtrlStateIdle();
extern void PlayerCtrlStateSwim();
extern void PlayerCtrlStateStroke();
extern void PlayerCtrlStateSpin();
extern void PlayerCtrlStateTurn();
extern void PlayerCtrlStateSwimStart();
extern void PlayerCtrlStateStop();
extern void PlayerCtrlStateDead();
extern void InputCtrlStateStart();
extern void InputCtrlStateDead();
extern void sub_801793C();
extern void sub_801796C();

/* Per-state handlers dispatched by UpdatePlayerCtrl (actor_part_16048.c,
 * its `struct pmf` view); PlayerCtrlKillPlayer sets state 7. */
const struct actor_pmf gPlayerCtrlStateFuncs[8] = {
    ACTOR_PMF(PlayerCtrlStateIdle),
    ACTOR_PMF(PlayerCtrlStateSwim),
    ACTOR_PMF(PlayerCtrlStateStroke),
    ACTOR_PMF(PlayerCtrlStateSpin),
    ACTOR_PMF(PlayerCtrlStateTurn),
    ACTOR_PMF(PlayerCtrlStateStop),
    ACTOR_PMF(PlayerCtrlStateSwimStart),
    ACTOR_PMF(PlayerCtrlStateDead),
};

/* Per-state handlers dispatched by UpdateInputCtrl (actor_part_17524.c). */
const struct actor_pmf gInputCtrlStateFuncs[4] = {
    ACTOR_PMF(InputCtrlStateStart),
    ACTOR_PMF(sub_801796C),
    ACTOR_PMF(sub_801793C),
    ACTOR_PMF(InputCtrlStateDead),
};

/* An entry set as gobj_1a794.h's `struct mover` points at it (`set`,
 * +0x04): the {a, b} entries (`struct vec_pair`) and a word the code
 * doesn't read, 0x100 in every set in the ROM. actor_part27b.c stores
 * gMegaMixMotionSet there. */
struct entry_set
{
    const u32 (*entries)[2];
    u32 unk_04;
};

/* gMegaMixMotionSet's four entries. */
const u32 gMegaMixMotionEntries[4][2] = {
    { 0, 0 },
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
};

const struct entry_set gMegaMixMotionSet = {
    gMegaMixMotionEntries, 0x100,
};
