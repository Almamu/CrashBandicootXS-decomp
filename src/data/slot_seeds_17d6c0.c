#include "core.h"
#include "frontend.h"

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
extern const u8 gVvLogoEmblemPalette[];
extern const u8 gVvLogoLettersPalette[];
extern const u8 gVvLogoUrlPalette[];
extern const u8 gVvLogoEmblemTiles[];
extern const u8 gVvLogoLettersTiles[];
extern const u8 gVvLogoUrlTiles[];

/* Twenty {record, hold} seeds read by InitVvLogoPieces
 * (title_screen.c): the motion sequences in
 * countdown_17d7a4.c, then a {NULL, 0} terminator. */
const struct slot_seed gVvLogoPieceSeeds[21] = {
    { gVvLogoPieceMotion00, 0 },
    { gVvLogoPieceMotion01, 0x5a },
    { gVvLogoPieceMotion02, 0x60 },
    { gVvLogoPieceMotion03, 0x64 },
    { gVvLogoPieceMotion04, 0x68 },
    { gVvLogoPieceMotion05, 0x6e },
    { gVvLogoPieceMotion06, 0x73 },
    { gVvLogoPieceMotion07, 0x78 },
    { gVvLogoPieceMotion08, 0x7d },
    { gVvLogoPieceMotion09, 0x83 },
    { gVvLogoPieceMotion10, 0x89 },
    { gVvLogoPieceMotion11, 0x8a },
    { gVvLogoPieceMotion12, 0x8f },
    { gVvLogoPieceMotion13, 0x93 },
    { gVvLogoPieceMotion14, 0x97 },
    { gVvLogoPieceMotion15, 0x9b },
    { gVvLogoPieceMotion16, 0x9f },
    { gVvLogoPieceMotion17, 0xa3 },
    { gVvLogoPieceMotion18, 0xa6 },
    { gVvLogoPieceMotion19, 0xb2 },
    { NULL, 0 },
};

/* The three BG banks' packages RunCompanyLogos (title_screen.c)
 * loads as PKG_A/PKG_B/PKG_C: only the tiles and map are set. */
const struct bg_package gVvLogoEmblemObj = {
    0,
    0,
    (void *)gVvLogoEmblemPalette,
    (void *)gVvLogoEmblemTiles,
    NULL,
};

const struct bg_package gVvLogoLettersObj = {
    0,
    0,
    (void *)gVvLogoLettersPalette,
    (void *)gVvLogoLettersTiles,
    NULL,
};

const struct bg_package gVvLogoUrlObj = {
    0,
    0,
    (void *)gVvLogoUrlPalette,
    (void *)gVvLogoUrlTiles,
    NULL,
};
