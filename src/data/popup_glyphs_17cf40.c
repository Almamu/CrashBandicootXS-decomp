#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0817CF40-0x0817CFF4. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gStaticData_0817D0E4[];
extern const u8 gStaticData_086319A0[];
extern const u8 gStaticData_086319C8[];
extern const u8 gStaticData_086319F0[];
extern const u8 gStaticData_08631A18[];
extern const u8 gStaticData_08631A40[];
extern const u8 gStaticData_08631ACC[];
extern const u8 gStaticData_086324B4[];
extern const u8 gStaticData_08632820[];
extern const u8 gStaticData_08632BC4[];
extern const u8 gStaticData_086334C4[];

/* graphics_loading_35d1c.c's view of one countdown-slot seed. */
struct slot_seed
{
    const void *record;
    s32 hold;
};

/* Five popup glyph sources, each a {w, h, palette, tiles, 0}
 * package (sub_80352AC, actor_part131.c, reads them as its
 * `struct popup_glyph_src`). */
const struct bg_package gStaticData_0817CF40[5] = {
    { 0x10, 0x4, (void *)gStaticData_086319F0, (void *)gStaticData_08632820, NULL },
    { 0x10, 0x9, (void *)gStaticData_086319A0, (void *)gStaticData_08631ACC, NULL },
    { 0x10, 0x4, (void *)gStaticData_086319C8, (void *)gStaticData_086324B4, NULL },
    { 0x10, 0xc, (void *)gStaticData_08631A18, (void *)gStaticData_08632BC4, NULL },
    { 0x1e, 0x8, (void *)gStaticData_08631A40, (void *)gStaticData_086334C4, NULL },
};

/* Nine {record, hold} seeds for the countdown slots of
 * graphics_loading_35d1c.c (sub_8035E14, sub_80360DC): records in
 * the still-raw gStaticData_0817D0E4, NULL-terminated. */
const struct slot_seed gStaticData_0817CFA4[10] = {
    { gStaticData_0817D0E4 + 0x14, 0x54 },
    { gStaticData_0817D0E4 + 0x94, 0x5f },
    { gStaticData_0817D0E4 + 0x114, 0x3c },
    { gStaticData_0817D0E4 + 0x1d4, 0x30 },
    { gStaticData_0817D0E4 + 0x274, 0x24 },
    { gStaticData_0817D0E4 + 0x314, 0x18 },
    { gStaticData_0817D0E4 + 0x3b4, 0xc },
    { gStaticData_0817D0E4 + 0x454, 0 },
    { gStaticData_0817D0E4 + 0x4f4, 0x49 },
    { NULL, 0 },
};
