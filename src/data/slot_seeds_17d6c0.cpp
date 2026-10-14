#include "game/ui/frontend.hpp"
#include "graphics.hpp"
#include "game/ui/logo_fly_in.h"

extern "C" {
#include "core.h"
}

using Engine::Gfx::VvLogoEmblem;
using Engine::Gfx::VvLogoLetters;
using Engine::Gfx::VvLogoUrl;

/*
 * ROM 0x0817D6C0-0x0817D7A4. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const struct delta_record gVvLogoPieceMotion00[];
extern const struct delta_record gVvLogoPieceMotion01[];
extern const struct delta_record gVvLogoPieceMotion02[];
extern const struct delta_record gVvLogoPieceMotion03[];
extern const struct delta_record gVvLogoPieceMotion04[];
extern const struct delta_record gVvLogoPieceMotion05[];
extern const struct delta_record gVvLogoPieceMotion06[];
extern const struct delta_record gVvLogoPieceMotion07[];
extern const struct delta_record gVvLogoPieceMotion08[];
extern const struct delta_record gVvLogoPieceMotion09[];
extern const struct delta_record gVvLogoPieceMotion10[];
extern const struct delta_record gVvLogoPieceMotion11[];
extern const struct delta_record gVvLogoPieceMotion12[];
extern const struct delta_record gVvLogoPieceMotion13[];
extern const struct delta_record gVvLogoPieceMotion14[];
extern const struct delta_record gVvLogoPieceMotion15[];
extern const struct delta_record gVvLogoPieceMotion16[];
extern const struct delta_record gVvLogoPieceMotion17[];
extern const struct delta_record gVvLogoPieceMotion18[];
extern const struct delta_record gVvLogoPieceMotion19[];

/* Twenty {record, hold} seeds read by InitVvLogoPieces
 * (company_logos.cpp): the motion sequences in
 * countdown_17d7a4.cpp, then a {0, 0} terminator. Pieces 1-19 fly in, and
 * start their motion that many frames earlier on a screen bigger than
 * the art (logo_fly_in.h): 0 on the GBA. */
const struct slot_seed gVvLogoPieceSeeds[21] = {
    { gVvLogoPieceMotion00, 0 },
    { gVvLogoPieceMotion01, 0x5a - VV_LOGO_PIECE01_FLY_IN },
    { gVvLogoPieceMotion02, 0x60 - VV_LOGO_PIECE02_FLY_IN },
    { gVvLogoPieceMotion03, 0x64 - VV_LOGO_PIECE03_FLY_IN },
    { gVvLogoPieceMotion04, 0x68 - VV_LOGO_PIECE04_FLY_IN },
    { gVvLogoPieceMotion05, 0x6e - VV_LOGO_PIECE05_FLY_IN },
    { gVvLogoPieceMotion06, 0x73 - VV_LOGO_PIECE06_FLY_IN },
    { gVvLogoPieceMotion07, 0x78 - VV_LOGO_PIECE07_FLY_IN },
    { gVvLogoPieceMotion08, 0x7d - VV_LOGO_PIECE08_FLY_IN },
    { gVvLogoPieceMotion09, 0x83 - VV_LOGO_PIECE09_FLY_IN },
    { gVvLogoPieceMotion10, 0x89 - VV_LOGO_PIECE10_FLY_IN },
    { gVvLogoPieceMotion11, 0x8a - VV_LOGO_PIECE11_FLY_IN },
    { gVvLogoPieceMotion12, 0x8f - VV_LOGO_PIECE12_FLY_IN },
    { gVvLogoPieceMotion13, 0x93 - VV_LOGO_PIECE13_FLY_IN },
    { gVvLogoPieceMotion14, 0x97 - VV_LOGO_PIECE14_FLY_IN },
    { gVvLogoPieceMotion15, 0x9b - VV_LOGO_PIECE15_FLY_IN },
    { gVvLogoPieceMotion16, 0x9f - VV_LOGO_PIECE16_FLY_IN },
    { gVvLogoPieceMotion17, 0xa3 - VV_LOGO_PIECE17_FLY_IN },
    { gVvLogoPieceMotion18, 0xa6 - VV_LOGO_PIECE18_FLY_IN },
    { gVvLogoPieceMotion19, 0xb2 - VV_LOGO_PIECE19_FLY_IN },
    { 0, 0 },
};

/* The three BG banks' packages RunCompanyLogos (company_logos.cpp)
 * loads as PKG_A/PKG_B/PKG_C: only the tiles and map are set. */
const struct bg_package gVvLogoEmblemObj = {
    0,
    0,
    (void *)VvLogoEmblem::palette,
    (void *)VvLogoEmblem::tiles,
    NULL,
};

const struct bg_package gVvLogoLettersObj = {
    0,
    0,
    (void *)VvLogoLetters::palette,
    (void *)VvLogoLetters::tiles,
    NULL,
};

const struct bg_package gVvLogoUrlObj = {
    0,
    0,
    (void *)VvLogoUrl::palette,
    (void *)VvLogoUrl::tiles,
    NULL,
};
