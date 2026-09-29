#include "core.h"

/*
 * ROM 0x0817E714-0x0817E72C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gStaticData_0817E6D8[];
extern const u8 gStaticData_0817E6E0[];
extern const u8 gStaticData_0817E6EC[];
extern const u8 gStaticData_0817E6F4[];
extern const u8 gStaticData_0817E6FC[];
extern const u8 gStaticData_0817E708[];

/* The six language names the counter selector draws (sub_80372BC,
 * counter_selector_icons.c), in countdown_17d7a4.c. */
const u8 *const gStaticData_0817E714[6] = {
    gStaticData_0817E6D8,
    gStaticData_0817E6E0,
    gStaticData_0817E6EC,
    gStaticData_0817E6F4,
    gStaticData_0817E6FC,
    gStaticData_0817E708,
};
