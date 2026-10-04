#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0817CF40-0x0817CFF4. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gStaticData_0817D0F8[];
extern const u8 gStaticData_0817D178[];
extern const u8 gStaticData_0817D1F8[];
extern const u8 gStaticData_0817D2B8[];
extern const u8 gStaticData_0817D358[];
extern const u8 gStaticData_0817D3F8[];
extern const u8 gStaticData_0817D498[];
extern const u8 gStaticData_0817D538[];
extern const u8 gStaticData_0817D5D8[];
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
 * package (LoadCreditsLogos, actor_part131.c, reads them as its
 * `struct popup_glyph_src`). */
const struct bg_package gCreditsLogos[5] = {
    { 0x10, 0x4, (void *)gStaticData_086319F0, (void *)gStaticData_08632820, NULL },
    { 0x10, 0x9, (void *)gStaticData_086319A0, (void *)gStaticData_08631ACC, NULL },
    { 0x10, 0x4, (void *)gStaticData_086319C8, (void *)gStaticData_086324B4, NULL },
    { 0x10, 0xc, (void *)gStaticData_08631A18, (void *)gStaticData_08632BC4, NULL },
    { 0x1e, 0x8, (void *)gStaticData_08631A40, (void *)gStaticData_086334C4, NULL },
};

/* Nine {record, hold} seeds for the countdown slots of
 * graphics_loading_35d1c.c (RunTitleScreen, ResetTitleLogoPieces): the motion
 * sequences in level_gfx_17cff4.c, NULL-terminated. */
const struct slot_seed gTitleLogoPieceSeeds[10] = {
    { gStaticData_0817D0F8, 0x54 },
    { gStaticData_0817D178, 0x5f },
    { gStaticData_0817D1F8, 0x3c },
    { gStaticData_0817D2B8, 0x30 },
    { gStaticData_0817D358, 0x24 },
    { gStaticData_0817D3F8, 0x18 },
    { gStaticData_0817D498, 0xc },
    { gStaticData_0817D538, 0 },
    { gStaticData_0817D5D8, 0x49 },
    { NULL, 0 },
};
