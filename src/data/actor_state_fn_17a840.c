#include "core.h"
#include "vehicle.h"

/*
 * ROM 0x0817A840-0x0817A850. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* Per-state update functions of the gYeti gauge object,
 * called as `gYetiStateFuncs[gYetiState]()` by
 * UpdateYeti (yeti_update.cpp). */
void (*const gYetiStateFuncs[4])(void) = {
    YetiStateChase,
    YetiStateCharge,
    YetiStateCaught,
    YetiStateStop,
};
