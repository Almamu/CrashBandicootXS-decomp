#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0817CF40-0x0817CFF4. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gTitleLogoPieceMotion0[];
extern const u8 gTitleLogoPieceMotion1[];
extern const u8 gTitleLogoPieceMotion2[];
extern const u8 gTitleLogoPieceMotion3[];
extern const u8 gTitleLogoPieceMotion4[];
extern const u8 gTitleLogoPieceMotion5[];
extern const u8 gTitleLogoPieceMotion6[];
extern const u8 gTitleLogoPieceMotion7[];
extern const u8 gTitleLogoPieceMotion8[];
extern const u8 gCreditsRedEyeStudiosLogoPalette[];
extern const u8 gCreditsShinenLogoPalette[];
extern const u8 gCreditsCosmigoLogoPalette[];
extern const u8 gCreditsUniversalLogoPalette[];
extern const u8 gCreditsVvLogoPalette[];
extern const u8 gCreditsRedEyeStudiosLogoTiles[];
extern const u8 gCreditsShinenLogoTiles[];
extern const u8 gCreditsCosmigoLogoTiles[];
extern const u8 gCreditsUniversalLogoTiles[];
extern const u8 gCreditsVvLogoTiles[];

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
    { 0x10, 0x4, (void *)gCreditsCosmigoLogoPalette, (void *)gCreditsCosmigoLogoTiles, NULL },
    { 0x10, 0x9, (void *)gCreditsRedEyeStudiosLogoPalette, (void *)gCreditsRedEyeStudiosLogoTiles, NULL },
    { 0x10, 0x4, (void *)gCreditsShinenLogoPalette, (void *)gCreditsShinenLogoTiles, NULL },
    { 0x10, 0xc, (void *)gCreditsUniversalLogoPalette, (void *)gCreditsUniversalLogoTiles, NULL },
    { 0x1e, 0x8, (void *)gCreditsVvLogoPalette, (void *)gCreditsVvLogoTiles, NULL },
};

/* Nine {record, hold} seeds for the countdown slots of
 * graphics_loading_35d1c.c (RunTitleScreen, ResetTitleLogoPieces): the motion
 * sequences in level_gfx_17cff4.c, NULL-terminated. */
const struct slot_seed gTitleLogoPieceSeeds[10] = {
    { gTitleLogoPieceMotion0, 0x54 },
    { gTitleLogoPieceMotion1, 0x5f },
    { gTitleLogoPieceMotion2, 0x3c },
    { gTitleLogoPieceMotion3, 0x30 },
    { gTitleLogoPieceMotion4, 0x24 },
    { gTitleLogoPieceMotion5, 0x18 },
    { gTitleLogoPieceMotion6, 0xc },
    { gTitleLogoPieceMotion7, 0 },
    { gTitleLogoPieceMotion8, 0x49 },
    { NULL, 0 },
};
