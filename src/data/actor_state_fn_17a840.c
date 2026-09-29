#include "core.h"

/*
 * ROM 0x0817A840-0x0817A850. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void nullsub_27();
extern void sub_802DB2C();
extern void sub_802DCC0();
extern void sub_802E0A4();

/* Per-state update functions of the gUnknown_030014BC gauge object,
 * called as `gStaticData_0817A840[gUnknown_030014D0]()` by
 * sub_802D7B0 (actor_part74.c). */
void (*const gStaticData_0817A840[4])() = {
    sub_802DB2C,
    sub_802DCC0,
    nullsub_27,
    sub_802E0A4,
};
