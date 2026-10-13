#include "frontend.hpp"

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
 * points, so they stay as they are. */
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
    { 18, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 68, 0, 256, 256, -575260, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 68, 0, 256, 256, -262144, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 74, DISPLAY_ART_Y + 68, 0, 128, 384, 262144, 0, 0, 8192, -8192 },
    { 71, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 90, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion02[6] = {
    { 14, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 68, 0, 256, 256, -711533, 0, 0, 0, 0 },
    { 2, DISPLAY_ART_X + 96, DISPLAY_ART_Y + 68, 0, 256, 256, -262144, 0, 0, -16384, 16384 },
    { 4, DISPLAY_ART_X + 88, DISPLAY_ART_Y + 68, 0, 128, 384, 262144, 0, 0, 8192, -8192 },
    { 71, DISPLAY_ART_X + 104, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 104, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion03[6] = {
    { 12, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 70, 0, 256, 256, -720896, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 116, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 116, DISPLAY_ART_Y + 70, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 67, DISPLAY_ART_X + 116, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 116, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion04[6] = {
    { 14, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 70, 0, 256, 256, -547693, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 131, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 131, DISPLAY_ART_Y + 70, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 61, DISPLAY_ART_X + 131, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 131, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion05[6] = {
    { 13, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 69, 0, 256, 256, -514205, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 69, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 56, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 146, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion06[6] = {
    { 13, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 68, 0, 256, 256, -458752, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 157, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 157, DISPLAY_ART_Y + 68, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 51, DISPLAY_ART_X + 157, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 157, DISPLAY_ART_Y + 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion07[6] = {
    { 12, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 69, 0, 256, 256, -425984, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 170, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 170, DISPLAY_ART_Y + 69, 0, 128, 384, 0, 0, 0, 8192, -8191 },
    { 47, DISPLAY_ART_X + 170, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 170, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion08[6] = {
    { 12, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 69, 0, 256, 256, -338602, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 186, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 186, DISPLAY_ART_Y + 69, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 42, DISPLAY_ART_X + 186, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 186, DISPLAY_ART_Y + 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion09[6] = {
    { 11, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 70, 0, 256, 256, -274059, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 202, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 202, DISPLAY_ART_Y + 70, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 37, DISPLAY_ART_X + 202, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 202, DISPLAY_ART_Y + 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion10[6] = {
    { 11, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 64, 0, 256, 256, -190650, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 216, DISPLAY_ART_Y + 64, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 216, DISPLAY_ART_Y + 64, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 31, DISPLAY_ART_X + 216, DISPLAY_ART_Y + 64, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 216, DISPLAY_ART_Y + 64, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion11[6] = {
    { 19, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 92, 0, 256, 256, -517389, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 98, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 98, DISPLAY_ART_Y + 92, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 22, DISPLAY_ART_X + 98, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 98, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion12[6] = {
    { 18, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 92, 0, 256, 256, -495160, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 112, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 112, DISPLAY_ART_Y + 92, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 18, DISPLAY_ART_X + 112, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 112, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion13[4] = {
    { 17, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 95, 0, 256, 256, -470317, 0, 0, 0, 0 },
    { 23, DISPLAY_ART_X + 126, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 126, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion14[6] = {
    { 16, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 92, 0, 256, 256, -458752, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 136, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 136, DISPLAY_ART_Y + 92, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 12, DISPLAY_ART_X + 136, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 136, DISPLAY_ART_Y + 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion15[6] = {
    { 14, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 95, 0, 256, 256, -458752, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 150, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 150, DISPLAY_ART_Y + 95, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 10, DISPLAY_ART_X + 150, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 150, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion16[6] = {
    { 12, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 95, 0, 256, 256, -447829, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 166, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 166, DISPLAY_ART_Y + 95, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 8, DISPLAY_ART_X + 166, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 166, DISPLAY_ART_Y + 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion17[6] = {
    { 10, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 94, 0, 256, 256, -432537, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 182, DISPLAY_ART_Y + 94, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 182, DISPLAY_ART_Y + 94, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 6, DISPLAY_ART_X + 182, DISPLAY_ART_Y + 94, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 182, DISPLAY_ART_Y + 94, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion18[6] = {
    { 9, DISPLAY_ART_X + 248, DISPLAY_ART_Y + 91, 0, 256, 256, -364088, 0, 0, 0, 0 },
    { 4, DISPLAY_ART_X + 198, DISPLAY_ART_Y + 91, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, DISPLAY_ART_X + 198, DISPLAY_ART_Y + 91, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 4, DISPLAY_ART_X + 198, DISPLAY_ART_Y + 91, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, DISPLAY_ART_X + 198, DISPLAY_ART_Y + 91, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

extern const struct delta_record gVvLogoPieceMotion19[4] = {
    { 8, DISPLAY_ART_X + 240, DISPLAY_ART_Y + 100, 0, 256, 256, -1212416, 0, 0, 0, 0 },
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
