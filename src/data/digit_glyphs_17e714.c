#include "core.h"

/*
 * ROM 0x0817E714-0x0817E72C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gStaticData_0817D7A4[];

/* The six digit glyphs the counter selector draws (sub_80372BC,
 * counter_selector_icons.c), inside the still-raw gStaticData_0817D7A4. */
const u8 *const gStaticData_0817E714[6] = {
    gStaticData_0817D7A4 + 0xf34,
    gStaticData_0817D7A4 + 0xf3c,
    gStaticData_0817D7A4 + 0xf48,
    gStaticData_0817D7A4 + 0xf50,
    gStaticData_0817D7A4 + 0xf58,
    gStaticData_0817D7A4 + 0xf64,
};
