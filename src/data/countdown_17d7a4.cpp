#include "frontend.hpp"
#include "logo_fly_in.h"

extern "C" {
#include "core.h"
}

/*
 * ROM 0x0817D7A4-0x0817E714. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */


extern const u8 gUniversalLogoBgPalette[];
extern const u8 gUniversalLogoBgTiles[];
extern const u8 gUniversalLogoBgMap[];
/* The BG2 picture LoadUniversalLogoBg (company_logos.cpp) loads. */
const struct bg_package gUniversalLogoBg = { 30, 20, (void *)gUniversalLogoBgPalette, (void *)gUniversalLogoBgTiles, (void *)gUniversalLogoBgMap };

/* The motion sequences of the twenty countdown slots of InitVvLogoPieces
 * (company_logos.cpp), which slot_seeds_17d6c0.cpp's
 * gVvLogoPieceSeeds seeds: each ends with a zero hold. The pieces' x and
 * y are on the 240x160 logo art, from its origin DISPLAY_ART_X/Y
 * (screen.h, docs/screen.md); the deltas move them between two such
 * points, so they stay as they are. Pieces 1-19 fly in from the right of
 * the art: their first step starts past the screen's edge
 * (LOGO_FLY_IN_STEP, logo_fly_in.h), and is the ROM's on the GBA. */
extern const struct delta_record gVvLogoPieceMotion00[11] = {
    { 79, DISPLAY_ART_X + 120, DISPLAY_ART_Y + 80, 0, 76, 76, 0, 0, 0, 580, 580 },
    { 4, DISPLAY_ART_X + 120, DISPLAY_ART_Y + 80, 0, 256, 256, 0, -819, 0, 3276, 3276 },
    { 4, DISPLAY_ART_X + 120, DISPLAY_ART_Y + 79, 0, 307, 307, 0, 819, 0, -3276, -3276 },
    { 13, DISPLAY_ART_X + 120, DISPLAY_ART_Y + 80, 0, 256, 256, -383133, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 44, DISPLAY_ART_Y + 80, 0, 256, 256, -131072, 0, 0, -3277, 6553 },
    { 4, DISPLAY_ART_X + 36, DISPLAY_ART_Y + 80, 0, 204, 358, 131072, 0, 0, 3277, -6553 },
    { 4, DISPLAY_ART_X + 44, DISPLAY_ART_Y + 80, 0, 256, 256, -131072, 0, 0, -3277, 6553 },
    { 4, DISPLAY_ART_X + 36, DISPLAY_ART_Y + 80, 0, 204, 358, 131072, 0, 0, 3277, -6553 },
    { 71, DISPLAY_ART_X + 44, DISPLAY_ART_Y + 80, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 44, DISPLAY_ART_Y + 80, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion01[6] = {
    LOGO_FLY_IN_STEP(18, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 68, VV_LOGO_PIECE01_FLY_IN, VV_LOGO_PIECE01_DX, 0),
    { 4, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 68, 0, 256, 256, -262144, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 74, DISPLAY_ART_Y + 68, 0, 128, 384, 262144, 0, 0, 8192, -8192 },
    { 71, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion02[6] = {
    LOGO_FLY_IN_STEP(14, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 68, VV_LOGO_PIECE02_FLY_IN, VV_LOGO_PIECE02_DX, 0),
    { 2, DISPLAY_ART_X + 96, DISPLAY_ART_Y + 68, 0, 256, 256, -262144, 0, 0, -16384, 16384 },
    { 4, DISPLAY_ART_X + 88, DISPLAY_ART_Y + 68, 0, 128, 384, 262144, 0, 0, 8192, -8192 },
    { 71, DISPLAY_ART_X + 104, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 104, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion03[6] = {
    LOGO_FLY_IN_STEP(12, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 70, VV_LOGO_PIECE03_FLY_IN, VV_LOGO_PIECE03_DX, 0),
    { 4, DISPLAY_ART_X + 116, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 116, DISPLAY_ART_Y + 70, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 67, DISPLAY_ART_X + 116, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 116, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion04[6] = {
    LOGO_FLY_IN_STEP(14, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 70, VV_LOGO_PIECE04_FLY_IN, VV_LOGO_PIECE04_DX, 0),
    { 4, DISPLAY_ART_X + 131, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 131, DISPLAY_ART_Y + 70, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 61, DISPLAY_ART_X + 131, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 131, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion05[6] = {
    LOGO_FLY_IN_STEP(13, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 69, VV_LOGO_PIECE05_FLY_IN, VV_LOGO_PIECE05_DX, 0),
    { 4, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 69, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 56, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion06[6] = {
    LOGO_FLY_IN_STEP(13, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 68, VV_LOGO_PIECE06_FLY_IN, VV_LOGO_PIECE06_DX, 0),
    { 4, DISPLAY_ART_X + 157, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 157, DISPLAY_ART_Y + 68, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 51, DISPLAY_ART_X + 157, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 157, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion07[6] = {
    LOGO_FLY_IN_STEP(12, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 69, VV_LOGO_PIECE07_FLY_IN, VV_LOGO_PIECE07_DX, 0),
    { 4, DISPLAY_ART_X + 170, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 170, DISPLAY_ART_Y + 69, 0, 128, 384, 0, 0, 0, 8192, -8191 },
    { 47, DISPLAY_ART_X + 170, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 170, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion08[6] = {
    LOGO_FLY_IN_STEP(12, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 69, VV_LOGO_PIECE08_FLY_IN, VV_LOGO_PIECE08_DX, 0),
    { 4, DISPLAY_ART_X + 186, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 186, DISPLAY_ART_Y + 69, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 42, DISPLAY_ART_X + 186, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 186, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion09[6] = {
    LOGO_FLY_IN_STEP(11, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 70, VV_LOGO_PIECE09_FLY_IN, VV_LOGO_PIECE09_DX, 0),
    { 4, DISPLAY_ART_X + 202, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 202, DISPLAY_ART_Y + 70, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 37, DISPLAY_ART_X + 202, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 202, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion10[6] = {
    LOGO_FLY_IN_STEP(11, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 64, VV_LOGO_PIECE10_FLY_IN, VV_LOGO_PIECE10_DX, 0),
    { 4, DISPLAY_ART_X + 216, DISPLAY_ART_Y + 64, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 216, DISPLAY_ART_Y + 64, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 31, DISPLAY_ART_X + 216, DISPLAY_ART_Y + 64, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 216, DISPLAY_ART_Y + 64, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion11[6] = {
    LOGO_FLY_IN_STEP(19, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 92, VV_LOGO_PIECE11_FLY_IN, VV_LOGO_PIECE11_DX, 0),
    { 4, DISPLAY_ART_X + 98, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 98, DISPLAY_ART_Y + 92, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 22, DISPLAY_ART_X + 98, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 98, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion12[6] = {
    LOGO_FLY_IN_STEP(18, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 92, VV_LOGO_PIECE12_FLY_IN, VV_LOGO_PIECE12_DX, 0),
    { 4, DISPLAY_ART_X + 112, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 112, DISPLAY_ART_Y + 92, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 18, DISPLAY_ART_X + 112, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 112, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion13[4] = {
    LOGO_FLY_IN_STEP(17, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 95, VV_LOGO_PIECE13_FLY_IN, VV_LOGO_PIECE13_DX, 0),
    { 23, DISPLAY_ART_X + 126, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 126, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion14[6] = {
    LOGO_FLY_IN_STEP(16, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 92, VV_LOGO_PIECE14_FLY_IN, VV_LOGO_PIECE14_DX, 0),
    { 4, DISPLAY_ART_X + 136, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 136, DISPLAY_ART_Y + 92, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 12, DISPLAY_ART_X + 136, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 136, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion15[6] = {
    LOGO_FLY_IN_STEP(14, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 95, VV_LOGO_PIECE15_FLY_IN, VV_LOGO_PIECE15_DX, 0),
    { 4, DISPLAY_ART_X + 150, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 150, DISPLAY_ART_Y + 95, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 10, DISPLAY_ART_X + 150, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 150, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion16[6] = {
    LOGO_FLY_IN_STEP(12, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 95, VV_LOGO_PIECE16_FLY_IN, VV_LOGO_PIECE16_DX, 0),
    { 4, DISPLAY_ART_X + 166, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 166, DISPLAY_ART_Y + 95, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 8, DISPLAY_ART_X + 166, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 166, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion17[6] = {
    LOGO_FLY_IN_STEP(10, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 94, VV_LOGO_PIECE17_FLY_IN, VV_LOGO_PIECE17_DX, 0),
    { 4, DISPLAY_ART_X + 182, DISPLAY_ART_Y + 94, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 182, DISPLAY_ART_Y + 94, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 6, DISPLAY_ART_X + 182, DISPLAY_ART_Y + 94, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 182, DISPLAY_ART_Y + 94, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion18[6] = {
    LOGO_FLY_IN_STEP(9, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 91, VV_LOGO_PIECE18_FLY_IN, VV_LOGO_PIECE18_DX, 0),
    { 4, DISPLAY_ART_X + 198, DISPLAY_ART_Y + 91, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 198, DISPLAY_ART_Y + 91, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 4, DISPLAY_ART_X + 198, DISPLAY_ART_Y + 91, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 198, DISPLAY_ART_Y + 91, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion19[4] = {
    LOGO_FLY_IN_STEP(8, DISPLAY_ART_X + 240, DISPLAY_ART_Y + 100, VV_LOGO_PIECE19_FLY_IN, VV_LOGO_PIECE19_DX, 0),
    { 1, DISPLAY_ART_X + 92, DISPLAY_ART_Y + 100, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 92, DISPLAY_ART_Y + 100, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

/* The language names the language menu draws (DrawLanguageSelect,
 * language_select.cpp, through digit_glyphs_17e714.cpp's
 * gLanguageNames), in the order of the language setting. */
extern const u8 gLanguageNameEnglish[] = "english";
extern const u8 gLanguageNameFrench[] = "fran\347ais";
extern const u8 gLanguageNameGerman[] = "deutsch";
extern const u8 gLanguageNameSpanish[] = "espa\361ol";
extern const u8 gLanguageNameItalian[] = "italiano";
extern const u8 gLanguageNameDutch[12] = "nederlands";
/* (The last one is sized to 12 bytes: the two zero bytes after it are the
 * padding to the word-aligned table that follows.) */
