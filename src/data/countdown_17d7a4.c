#include "core.h"

/*
 * ROM 0x0817D7A4-0x0817E714. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

#include "graphics_package.h"

/* graphics_loading_35d1c.c's `struct delta_record`: one step of a
 * countdown slot's motion. When the slot's hold count runs out it loads
 * the next record: a new hold count (0 ends the sequence), three Q16.16
 * positions, two Q24.8 velocities and the five per-frame deltas added to
 * them while the hold lasts. */
struct delta_record
{
    s16 hold;
    u16 dPosA;
    u16 dPosB;
    u16 dPosC;
    s16 dVelA;
    s16 dVelB;
    s32 deltaA;
    s32 deltaB;
    s32 deltaC;
    s32 deltaD;
    s32 deltaE;
};

extern const u8 gUniversalLogoBgPalette[];
extern const u8 gUniversalLogoBgTiles[];
extern const u8 gUniversalLogoBgMap[];
/* The BG2 picture LoadUniversalLogoBg (graphics_loading_3686c.c) loads. */
const struct bg_package gUniversalLogoBg = { 30, 20, (void *)gUniversalLogoBgPalette, (void *)gUniversalLogoBgTiles, (void *)gUniversalLogoBgMap };

/* The motion sequences of the twenty countdown slots of InitVvLogoPieces
 * (graphics_loading_35d1c.c), which slot_seeds_17d6c0.c's
 * gVvLogoPieceSeeds seeds: each ends with a zero hold. */
const struct delta_record gVvLogoPieceMotion00[11] = {
    { 79, 120, 80, 0, 76, 76, 0, 0, 0, 580, 580 },
    { 4, 120, 80, 0, 256, 256, 0, -819, 0, 3276, 3276 },
    { 4, 120, 79, 0, 307, 307, 0, 819, 0, -3276, -3276 },
    { 13, 120, 80, 0, 256, 256, -383133, 0, 0, 0, 0 },
    { 4, 44, 80, 0, 256, 256, -131072, 0, 0, -3277, 6553 },
    { 4, 36, 80, 0, 204, 358, 131072, 0, 0, 3277, -6553 },
    { 4, 44, 80, 0, 256, 256, -131072, 0, 0, -3277, 6553 },
    { 4, 36, 80, 0, 204, 358, 131072, 0, 0, 3277, -6553 },
    { 71, 44, 80, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 44, 80, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion01[6] = {
    { 18, 248, 68, 0, 256, 256, -575260, 0, 0, 0, 0 },
    { 4, 90, 68, 0, 256, 256, -262144, 0, 0, -8192, 8192 },
    { 4, 74, 68, 0, 128, 384, 262144, 0, 0, 8192, -8192 },
    { 71, 90, 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 90, 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion02[6] = {
    { 14, 248, 68, 0, 256, 256, -711533, 0, 0, 0, 0 },
    { 2, 96, 68, 0, 256, 256, -262144, 0, 0, -16384, 16384 },
    { 4, 88, 68, 0, 128, 384, 262144, 0, 0, 8192, -8192 },
    { 71, 104, 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 104, 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion03[6] = {
    { 12, 248, 70, 0, 256, 256, -720896, 0, 0, 0, 0 },
    { 4, 116, 70, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 116, 70, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 67, 116, 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 116, 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion04[6] = {
    { 14, 248, 70, 0, 256, 256, -547693, 0, 0, 0, 0 },
    { 4, 131, 70, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 131, 70, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 61, 131, 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 131, 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion05[6] = {
    { 13, 248, 69, 0, 256, 256, -514205, 0, 0, 0, 0 },
    { 4, 146, 69, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 146, 69, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 56, 146, 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 146, 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion06[6] = {
    { 13, 248, 68, 0, 256, 256, -458752, 0, 0, 0, 0 },
    { 4, 157, 68, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 157, 68, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 51, 157, 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 157, 68, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion07[6] = {
    { 12, 248, 69, 0, 256, 256, -425984, 0, 0, 0, 0 },
    { 4, 170, 69, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 170, 69, 0, 128, 384, 0, 0, 0, 8192, -8191 },
    { 47, 170, 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 170, 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion08[6] = {
    { 12, 248, 69, 0, 256, 256, -338602, 0, 0, 0, 0 },
    { 4, 186, 69, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 186, 69, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 42, 186, 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 186, 69, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion09[6] = {
    { 11, 248, 70, 0, 256, 256, -274059, 0, 0, 0, 0 },
    { 4, 202, 70, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 202, 70, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 37, 202, 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 202, 70, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion10[6] = {
    { 11, 248, 64, 0, 256, 256, -190650, 0, 0, 0, 0 },
    { 4, 216, 64, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 216, 64, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 31, 216, 64, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 216, 64, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion11[6] = {
    { 19, 248, 92, 0, 256, 256, -517389, 0, 0, 0, 0 },
    { 4, 98, 92, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 98, 92, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 22, 98, 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 98, 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion12[6] = {
    { 18, 248, 92, 0, 256, 256, -495160, 0, 0, 0, 0 },
    { 4, 112, 92, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 112, 92, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 18, 112, 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 112, 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion13[4] = {
    { 17, 248, 95, 0, 256, 256, -470317, 0, 0, 0, 0 },
    { 23, 126, 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 126, 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion14[6] = {
    { 16, 248, 92, 0, 256, 256, -458752, 0, 0, 0, 0 },
    { 4, 136, 92, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 136, 92, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 12, 136, 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 136, 92, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion15[6] = {
    { 14, 248, 95, 0, 256, 256, -458752, 0, 0, 0, 0 },
    { 4, 150, 95, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 150, 95, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 10, 150, 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 150, 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion16[6] = {
    { 12, 248, 95, 0, 256, 256, -447829, 0, 0, 0, 0 },
    { 4, 166, 95, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 166, 95, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 8, 166, 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 166, 95, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion17[6] = {
    { 10, 248, 94, 0, 256, 256, -432537, 0, 0, 0, 0 },
    { 4, 182, 94, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 182, 94, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 6, 182, 94, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 182, 94, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion18[6] = {
    { 9, 248, 91, 0, 256, 256, -364088, 0, 0, 0, 0 },
    { 4, 198, 91, 0, 256, 256, 0, 0, 0, -8192, 8192 },
    { 4, 198, 91, 0, 128, 384, 0, 0, 0, 8192, -8192 },
    { 4, 198, 91, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 198, 91, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const struct delta_record gVvLogoPieceMotion19[4] = {
    { 8, 240, 100, 0, 256, 256, -1212416, 0, 0, 0, 0 },
    { 1, 92, 100, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 1, 92, 100, 0, 256, 256, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

/* The language names the language menu draws (DrawLanguageSelect,
 * counter_selector_icons.c, through digit_glyphs_17e714.c's
 * gLanguageNames), in the order of the language setting. */
const u8 gLanguageNameEnglish[] = "english";
const u8 gLanguageNameFrench[] = "fran\347ais";
const u8 gLanguageNameGerman[] = "deutsch";
const u8 gLanguageNameSpanish[] = "espa\361ol";
const u8 gLanguageNameItalian[] = "italiano";
const u8 gLanguageNameDutch[12] = "nederlands";
/* (The last one is sized to 12 bytes: the two zero bytes after it are the
 * padding to the word-aligned table that follows.) */
