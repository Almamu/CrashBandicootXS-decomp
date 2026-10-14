#include "game/ui/frontend.hpp"
#include "graphics.hpp"
#include "game/ui/logo_fly_in.h"

extern "C" {
#include "core.h"
}

using Engine::Gfx::CreditsRedEyeStudiosLogo;
using Engine::Gfx::CreditsShinenLogo;
using Engine::Gfx::CreditsCosmigoLogo;
using Engine::Gfx::CreditsUniversalLogo;
using Engine::Gfx::CreditsVvLogo;

/*
 * ROM 0x0817CF40-0x0817CFF4. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const struct delta_record gTitleLogoPieceMotion0[];
extern const struct delta_record gTitleLogoPieceMotion1[];
extern const struct delta_record gTitleLogoPieceMotion2[];
extern const struct delta_record gTitleLogoPieceMotion3[];
extern const struct delta_record gTitleLogoPieceMotion4[];
extern const struct delta_record gTitleLogoPieceMotion5[];
extern const struct delta_record gTitleLogoPieceMotion6[];
extern const struct delta_record gTitleLogoPieceMotion7[];
extern const struct delta_record gTitleLogoPieceMotion8[];

/* Five popup glyph sources, each a {w, h, palette, tiles, 0}
 * package (LoadCreditsLogos, credits.cpp, reads them as its
 * `struct popup_glyph_src`). */
const struct bg_package gCreditsLogos[5] = {
    { 0x10, 0x4, (void *)CreditsCosmigoLogo::palette, (void *)CreditsCosmigoLogo::tiles, NULL },
    { 0x10, 0x9, (void *)CreditsRedEyeStudiosLogo::palette, (void *)CreditsRedEyeStudiosLogo::tiles, NULL },
    { 0x10, 0x4, (void *)CreditsShinenLogo::palette, (void *)CreditsShinenLogo::tiles, NULL },
    { 0x10, 0xc, (void *)CreditsUniversalLogo::palette, (void *)CreditsUniversalLogo::tiles, NULL },
    { 0x1e, 0x8, (void *)CreditsVvLogo::palette, (void *)CreditsVvLogo::tiles, NULL },
};

/* Nine {record, hold} seeds for the countdown slots of
 * title_screen.cpp (RunTitleScreen, ResetTitleLogoPieces): the motion
 * sequences in level_gfx_17cff4.cpp, NULL-terminated. Pieces 0 and 1
 * fly in, and start their motion that many frames earlier on a screen
 * bigger than the art (logo_fly_in.h): 0 on the GBA. */
const struct slot_seed gTitleLogoPieceSeeds[10] = {
    { gTitleLogoPieceMotion0, 0x54 - TITLE_LOGO_PIECE0_FLY_IN },
    { gTitleLogoPieceMotion1, 0x5f - TITLE_LOGO_PIECE1_FLY_IN },
    { gTitleLogoPieceMotion2, 0x3c },
    { gTitleLogoPieceMotion3, 0x30 },
    { gTitleLogoPieceMotion4, 0x24 },
    { gTitleLogoPieceMotion5, 0x18 },
    { gTitleLogoPieceMotion6, 0xc },
    { gTitleLogoPieceMotion7, 0 },
    { gTitleLogoPieceMotion8, 0x49 },
    { 0, 0 },
};
