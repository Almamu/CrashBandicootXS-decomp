#include "core.h"

/*
 * ROM 0x0817A840-0x0817A850. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void YetiStateCaught();
extern void YetiStateChase();
extern void YetiStateCharge();
extern void YetiStateStop();

/* Per-state update functions of the gYeti gauge object,
 * called as `gYetiStateFuncs[gYetiState]()` by
 * UpdateYeti (actor_part74.c). */
void (*const gYetiStateFuncs[4])() = {
    YetiStateChase,
    YetiStateCharge,
    YetiStateCaught,
    YetiStateStop,
};
