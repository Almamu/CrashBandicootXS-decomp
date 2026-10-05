#include "core.h"

/*
 * ROM 0x0817A840-0x0817A850. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void YetiStateCaught();
extern void sub_802DB2C();
extern void sub_802DCC0();
extern void YetiStateStop();

/* Per-state update functions of the gYeti gauge object,
 * called as `gYetiStateFuncs[gYetiState]()` by
 * UpdateYeti (actor_part74.c). */
void (*const gYetiStateFuncs[4])() = {
    sub_802DB2C,
    sub_802DCC0,
    YetiStateCaught,
    YetiStateStop,
};
