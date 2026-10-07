#include "core.h"
#include "level_data.h"
#include "level.h"
#include "constants/songs.h"

/*
 * ROM 0x0816C814-0x0816D1C8: the level table and the room records under
 * it. Linked in ROM order between data/data.s sections by ldscript.txt -
 * see docs/data.md and docs/levels.md.
 *
 * - five palette-entry lists for the level-start colour cycles;
 * - gLevelTable, one `struct level_info` per level;
 * - per level, its room list: the rooms played in order, and up to two
 *   extra rooms;
 * - gThemeMusicCues, the music cue of each level theme;
 * - the room records: a room's palette and `struct level_desc` (both
 *   defined in src/data/level_rooms_*.c, from data/levels/), or the
 *   actor category of a stage that has no room data.
 *
 * Every pointer is a symbol reference, so the room data can move.
 */

COMPILE_TIME_ASSERT(level_table_16c814_c, sizeof(struct level_info) == 0x24);
COMPILE_TIME_ASSERT(level_table_16c814_c, sizeof(struct level_room_list) == 0x10);
COMPILE_TIME_ASSERT(level_table_16c814_c, sizeof(struct level_room) == 0x14);

/* The rooms' palettes and descriptors (src/data/level_rooms_*.c). */
extern const u16 gRoom00Palette[];
extern const struct level_desc gRoom00Desc;
extern const u16 gRoom01Palette[];
extern const struct level_desc gRoom01Desc;
extern const u16 gRoom02Palette[];
extern const struct level_desc gRoom02Desc;
extern const u16 gRoom03Palette[];
extern const struct level_desc gRoom03Desc;
extern const u16 gRoom04Palette[];
extern const struct level_desc gRoom04Desc;
extern const u16 gRoom05Palette[];
extern const struct level_desc gRoom05Desc;
extern const u16 gRoom06Palette[];
extern const struct level_desc gRoom06Desc;
extern const u16 gRoom07Palette[];
extern const struct level_desc gRoom07Desc;
extern const u16 gRoom08Palette[];
extern const struct level_desc gRoom08Desc;
extern const u16 gRoom09Palette[];
extern const struct level_desc gRoom09Desc;
extern const u16 gRoom10Palette[];
extern const struct level_desc gRoom10Desc;
extern const u16 gRoom11Palette[];
extern const struct level_desc gRoom11Desc;
extern const u16 gRoom12Palette[];
extern const struct level_desc gRoom12Desc;
extern const u16 gRoom13Palette[];
extern const struct level_desc gRoom13Desc;
extern const u16 gRoom14Palette[];
extern const struct level_desc gRoom14Desc;
extern const u16 gRoom15Palette[];
extern const struct level_desc gRoom15Desc;
extern const u16 gRoom16Palette[];
extern const struct level_desc gRoom16Desc;
extern const u16 gRoom17Palette[];
extern const struct level_desc gRoom17Desc;
extern const u16 gRoom18Palette[];
extern const struct level_desc gRoom18Desc;
extern const u16 gRoom19Palette[];
extern const struct level_desc gRoom19Desc;
extern const u16 gRoom20Palette[];
extern const struct level_desc gRoom20Desc;
extern const u16 gRoom21Palette[];
extern const struct level_desc gRoom21Desc;
extern const u16 gRoom22Palette[];
extern const struct level_desc gRoom22Desc;
extern const u16 gRoom23Palette[];
extern const struct level_desc gRoom23Desc;
extern const u16 gRoom24Palette[];
extern const struct level_desc gRoom24Desc;
extern const u16 gRoom25Palette[];
extern const struct level_desc gRoom25Desc;
extern const u16 gRoom26Palette[];
extern const struct level_desc gRoom26Desc;
extern const u16 gRoom27Palette[];
extern const struct level_desc gRoom27Desc;
extern const u16 gRoom28Palette[];
extern const struct level_desc gRoom28Desc;
extern const u16 gRoom29Palette[];
extern const struct level_desc gRoom29Desc;
extern const u16 gRoom30Palette[];
extern const struct level_desc gRoom30Desc;
extern const u16 gRoom31Palette[];
extern const struct level_desc gRoom31Desc;
extern const u16 gRoom32Palette[];
extern const struct level_desc gRoom32Desc;
extern const u16 gRoom33Palette[];
extern const struct level_desc gRoom33Desc;
extern const u16 gRoom34Palette[];
extern const struct level_desc gRoom34Desc;
extern const u16 gRoom35Palette[];
extern const struct level_desc gRoom35Desc;
extern const u16 gRoom36Palette[];
extern const struct level_desc gRoom36Desc;
extern const u16 gRoom37Palette[];
extern const struct level_desc gRoom37Desc;
extern const u16 gRoom38Palette[];
extern const struct level_desc gRoom38Desc;
extern const u16 gRoom39Palette[];
extern const struct level_desc gRoom39Desc;
extern const u16 gRoom40Palette[];
extern const struct level_desc gRoom40Desc;

extern const struct level_room_list gLevelRoomLists[LEVEL_COUNT];
extern const struct level_room *const gLevel00Rooms[1];
extern const struct level_room *const gLevel01Rooms[1];
extern const struct level_room *const gLevel02Rooms[1];
extern const struct level_room *const gLevel03Rooms[3];
extern const struct level_room *const gLevel04Rooms[1];
extern const struct level_room *const gLevel05Rooms[3];
extern const struct level_room *const gLevel06Rooms[1];
extern const struct level_room *const gLevel07Rooms[1];
extern const struct level_room *const gLevel08Rooms[1];
extern const struct level_room *const gLevel09Rooms[1];
extern const struct level_room *const gLevel10Rooms[3];
extern const struct level_room *const gLevel11Rooms[1];
extern const struct level_room *const gLevel12Rooms[1];
extern const struct level_room *const gLevel13Rooms[1];
extern const struct level_room *const gLevel14Rooms[1];
extern const struct level_room *const gLevel15Rooms[1];
extern const struct level_room *const gLevel16Rooms[1];
extern const struct level_room *const gLevel17Rooms[1];
extern const struct level_room *const gLevel18Rooms[1];
extern const struct level_room *const gLevel19Rooms[1];
extern const struct level_room *const gLevel24Rooms[1];
extern const struct level_room *const gLevel20Rooms[1];
extern const struct level_room *const gLevel22Rooms[1];
extern const struct level_room *const gLevel23Rooms[1];
extern const struct level_room *const gLevel21Rooms[1];
extern const struct level_room gLevelRoom00;
extern const struct level_room gLevelRoom01;
extern const struct level_room gLevelRoom02;
extern const struct level_room gLevelRoom03;
extern const struct level_room gLevelRoom04;
extern const struct level_room gLevelRoom05;
extern const struct level_room gLevelRoom06;
extern const struct level_room gLevelRoom07;
extern const struct level_room gLevelRoom08;
extern const struct level_room gLevelRoom09;
extern const struct level_room gLevelRoom10;
extern const struct level_room gLevelRoom11;
extern const struct level_room gLevelRoom12;
extern const struct level_room gLevelRoom13;
extern const struct level_room gLevelRoom14;
extern const struct level_room gLevelRoom15;
extern const struct level_room gLevelRoom16;
extern const struct level_room gLevelRoom17;
extern const struct level_room gLevelRoom18;
extern const struct level_room gLevelRoom19;
extern const struct level_room gLevelRoom20;
extern const struct level_room gLevelStage0;
extern const struct level_room gLevelRoom21;
extern const struct level_room gLevelRoom22;
extern const struct level_room gLevelRoom23;
extern const struct level_room gLevelStage1;
extern const struct level_room gLevelRoom24;
extern const struct level_room gLevelStage3;
extern const struct level_room gLevelRoom25;
extern const struct level_room gLevelRoom26;
extern const struct level_room gLevelRoom27;
extern const struct level_room gLevelRoom28;
extern const struct level_room gLevelStage2;
extern const struct level_room gLevelRoom29;
extern const struct level_room gLevelRoom30;
extern const struct level_room gLevelRoom31;
extern const struct level_room gLevelRoom32;
extern const struct level_room gLevelStage4;
extern const struct level_room gLevelRoom33;
extern const struct level_room gLevelRoom34;
extern const struct level_room gLevelStage5;
extern const struct level_room gLevelRoom35;
extern const struct level_room gLevelRoom36;
extern const struct level_room gLevelRoom37;
extern const struct level_room gLevelRoom38;
extern const struct level_room gLevelRoom39;
extern const struct level_room gLevelRoom40;
extern const struct level_room gLevelStage6;

/*
 * Palette entries cycled by the level-start colour animations of
 * RunRoom (run_room.c), one list per theme case: each is the
 * `lists` argument of a AddPaletteCycle call on BG palette RAM.
 */
const u16 gThemePaletteCycle2[5] = { 0xb1, 0xb2, 0xb3, 0xb4, 0xb5 };
const u16 gThemePaletteCycle1A[9] = { 0x39, 0x3a, 0x3d, 0x3e, 0x75, 0x82, 0xbc, 0xea, 0xfc };
const u16 gThemePaletteCycle1B[9] = { 0x51, 0x52, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0xf1 };
const u16 gThemePaletteCycle3[16] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
};
const u16 gThemePaletteCycle5[5] = { 0x97, 0xb4, 0xf7, 0xf8, 0xff };

/*
 * The levels, indexed by level id (the game's level numbering; `nameText`
 * is the text id of the level's name). Declared in level.h.
 */
const struct level_info gLevelTable[LEVEL_COUNT] = {
    [LEVEL_JUNGLE_JAM] = { 1, 1, { 355, 275, 233 }, 4, 4, 0, &gLevelRoomLists[LEVEL_JUNGLE_JAM] },
    [LEVEL_SHIPWRECKED] = { 2, 3, { 777, 677, 544 }, 4, 1, 0, &gLevelRoomLists[LEVEL_SHIPWRECKED] },
    [LEVEL_TEMPLE_OF_BOOM] = { 3,
                               1,
                               { 627, 557, 490 },
                               4,
                               4,
                               0,
                               &gLevelRoomLists[LEVEL_TEMPLE_OF_BOOM] },
    [LEVEL_FROSTBITE_CAVERN] = { 4,
                                 0,
                                 { 917, 771, 660 },
                                 4,
                                 4,
                                 0,
                                 &gLevelRoomLists[LEVEL_FROSTBITE_CAVERN] },
    [LEVEL_JUST_IN_SLIME] = { 5,
                              2,
                              { 994, 756, 665 },
                              5,
                              5,
                              0,
                              &gLevelRoomLists[LEVEL_JUST_IN_SLIME] },
    [LEVEL_SNOW_CRASH] = { 6, 0, { 1030, 967, 880 }, 5, 5, 0, &gLevelRoomLists[LEVEL_SNOW_CRASH] },
    [LEVEL_ROCKET_RACKET] = { 7,
                              4,
                              { 1502, 1375, 1250 },
                              5,
                              5,
                              0,
                              &gLevelRoomLists[LEVEL_ROCKET_RACKET] },
    [LEVEL_JUST_HANGIN] = { 8,
                            2,
                            { 1094, 1030, 870 },
                            5,
                            5,
                            0,
                            &gLevelRoomLists[LEVEL_JUST_HANGIN] },
    [LEVEL_SHARK_ATTACK] = { 9,
                             3,
                             { 916, 770, 694 },
                             5,
                             1,
                             0,
                             &gLevelRoomLists[LEVEL_SHARK_ATTACK] },
    [LEVEL_RUINED] = { 10, 1, { 1618, 1588, 1200 }, 5, 5, 0, &gLevelRoomLists[LEVEL_RUINED] },
    [LEVEL_SNOW_JOB] = { 11, 0, { 1321, 1280, 1244 }, 6, 6, 0, &gLevelRoomLists[LEVEL_SNOW_JOB] },
    [LEVEL_ACE_OF_SPACE] = { 12,
                             5,
                             { 1184, 1158, 995 },
                             6,
                             6,
                             0,
                             &gLevelRoomLists[LEVEL_ACE_OF_SPACE] },
    [LEVEL_SUNKEN_CITY] = { 13,
                            3,
                            { 996, 817, 742 },
                            6,
                            1,
                            0,
                            &gLevelRoomLists[LEVEL_SUNKEN_CITY] },
    [LEVEL_DOWN_THE_HOLE] = { 14,
                              1,
                              { 924, 834, 674 },
                              6,
                              6,
                              0,
                              &gLevelRoomLists[LEVEL_DOWN_THE_HOLE] },
    [LEVEL_BLIMP_BONANZA] = { 15,
                              4,
                              { 1782, 1649, 1563 },
                              6,
                              6,
                              0,
                              &gLevelRoomLists[LEVEL_BLIMP_BONANZA] },
    [LEVEL_STAR_TO_FINISH] = { 16,
                               5,
                               { 1276, 1195, 1041 },
                               7,
                               7,
                               0,
                               &gLevelRoomLists[LEVEL_STAR_TO_FINISH] },
    [LEVEL_AIR_SUPPLY] = { 17,
                           3,
                           { 1293, 1104, 1017 },
                           7,
                           1,
                           0,
                           &gLevelRoomLists[LEVEL_AIR_SUPPLY] },
    [LEVEL_NO_FLY_ZONE] = { 18,
                            4,
                            { 2063, 1971, 1896 },
                            7,
                            7,
                            0,
                            &gLevelRoomLists[LEVEL_NO_FLY_ZONE] },
    [LEVEL_DRIP_DRIP_DRIP] = { 19,
                               2,
                               { 1484, 970, 896 },
                               7,
                               7,
                               0,
                               &gLevelRoomLists[LEVEL_DRIP_DRIP_DRIP] },
    [LEVEL_FINAL_COUNTDOWN] = { 20,
                                5,
                                { 1504, 1338, 1182 },
                                7,
                                7,
                                0,
                                &gLevelRoomLists[LEVEL_FINAL_COUNTDOWN] },
    [LEVEL_DINGODILE] = { 22, 7, { 1000, 500, 250 }, 4, 5, 1, &gLevelRoomLists[LEVEL_DINGODILE] },
    [LEVEL_N_GIN] = { 23, 8, { 1000, 500, 250 }, 5, 5, 1, &gLevelRoomLists[LEVEL_N_GIN] },
    [LEVEL_TINY] = { 21, 6, { 1000, 500, 250 }, 6, 6, 1, &gLevelRoomLists[LEVEL_TINY] },
    [LEVEL_NEO_CORTEX] = { 24, 9, { 1000, 500, 250 }, 7, 7, 1, &gLevelRoomLists[LEVEL_NEO_CORTEX] },
    [LEVEL_MEGA_MIX] = { 25, 10, { 1000, 500, 250 }, 7, 7, 1, &gLevelRoomLists[LEVEL_MEGA_MIX] },
};

/* Each level's rooms (the `rooms` of its `struct level_info`). */
const struct level_room_list gLevelRoomLists[LEVEL_COUNT] = {
    { ARRAY_COUNT(gLevel00Rooms), gLevel00Rooms, &gLevelRoom00, NULL },
    { ARRAY_COUNT(gLevel01Rooms), gLevel01Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel02Rooms), gLevel02Rooms, &gLevelRoom01, &gLevelRoom15 },
    { ARRAY_COUNT(gLevel03Rooms), gLevel03Rooms, &gLevelRoom02, NULL },
    { ARRAY_COUNT(gLevel04Rooms), gLevel04Rooms, &gLevelRoom03, NULL },
    { ARRAY_COUNT(gLevel05Rooms), gLevel05Rooms, &gLevelRoom04, &gLevelRoom13 },
    { ARRAY_COUNT(gLevel06Rooms), gLevel06Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel07Rooms), gLevel07Rooms, &gLevelRoom05, &gLevelRoom14 },
    { ARRAY_COUNT(gLevel08Rooms), gLevel08Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel09Rooms), gLevel09Rooms, &gLevelRoom06, NULL },
    { ARRAY_COUNT(gLevel10Rooms), gLevel10Rooms, &gLevelRoom07, NULL },
    { ARRAY_COUNT(gLevel11Rooms), gLevel11Rooms, &gLevelRoom08, &gLevelRoom16 },
    { ARRAY_COUNT(gLevel12Rooms), gLevel12Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel13Rooms), gLevel13Rooms, &gLevelRoom09, NULL },
    { ARRAY_COUNT(gLevel14Rooms), gLevel14Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel15Rooms), gLevel15Rooms, &gLevelRoom10, NULL },
    { ARRAY_COUNT(gLevel16Rooms), gLevel16Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel17Rooms), gLevel17Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel18Rooms), gLevel18Rooms, &gLevelRoom11, NULL },
    { ARRAY_COUNT(gLevel19Rooms), gLevel19Rooms, &gLevelRoom12, NULL },
    { ARRAY_COUNT(gLevel20Rooms), gLevel20Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel21Rooms), gLevel21Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel22Rooms), gLevel22Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel23Rooms), gLevel23Rooms, NULL, NULL },
    { ARRAY_COUNT(gLevel24Rooms), gLevel24Rooms, NULL, NULL },
};

/*
 * The music cue of each level theme (`struct level_info.theme`), read by
 * PlayRoomMusic (level_query.c).
 */
const u8 gThemeMusicCues[11] = {
    SONG_ARCTIC,    SONG_JUNGLE, SONG_SEWERS, SONG_UNDERWATER, SONG_ROCKET_CRASH, SONG_FUTURE,
    SONG_DINGODILE, SONG_N_GIN,  SONG_TINY,   SONG_NEO_CORTEX, SONG_NEO_CORTEX,
};

/* The extra rooms of the room lists (rooms 00-16). */
const struct level_room gLevelRoom00 = {
    gRoom00Palette, &gRoom00Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room00_264730 */
const struct level_room gLevelRoom01 = {
    gRoom01Palette, &gRoom01Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room01_263f4c */
const struct level_room gLevelRoom02 = {
    gRoom02Palette, &gRoom02Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room02_2636e0 */
const struct level_room gLevelRoom03 = {
    gRoom03Palette, &gRoom03Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room03_270bcc */
const struct level_room gLevelRoom04 = {
    gRoom04Palette, &gRoom04Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room04_262e64 */
const struct level_room gLevelRoom05 = {
    gRoom05Palette, &gRoom05Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room05_270154 */
const struct level_room gLevelRoom06 = {
    gRoom06Palette, &gRoom06Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room06_267428 */
const struct level_room gLevelRoom07 = {
    gRoom07Palette, &gRoom07Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room07_264e64 */
const struct level_room gLevelRoom08 = {
    gRoom08Palette, &gRoom08Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room08_265804 */
const struct level_room gLevelRoom09 = {
    gRoom09Palette, &gRoom09Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room09_262654 */
const struct level_room gLevelRoom10 = {
    gRoom10Palette, &gRoom10Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room10_266194 */
const struct level_room gLevelRoom11 = {
    gRoom11Palette, &gRoom11Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room11_26f5c0 */
const struct level_room gLevelRoom12 = {
    gRoom12Palette, &gRoom12Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room12_266b40 */
const struct level_room gLevelRoom13 = {
    gRoom13Palette, &gRoom13Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room13_2bbde0 */
const struct level_room gLevelRoom14 = {
    gRoom14Palette, &gRoom14Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room14_2bcf40 */
const struct level_room gLevelRoom15 = {
    gRoom15Palette, &gRoom15Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room15_2bdf98 */
const struct level_room gLevelRoom16 = {
    gRoom16Palette, &gRoom16Desc, ROOM_KIND_HOVER, 0, 0, 0
}; /* data/levels/room16_2beadc */

/* The rooms of each level, in play order. */
const struct level_room *const gLevel00Rooms[1] = { &gLevelRoom17 };
const struct level_room *const gLevel01Rooms[1] = { &gLevelRoom18 };
const struct level_room *const gLevel02Rooms[1] = { &gLevelRoom19 };
const struct level_room *const gLevel03Rooms[3] = { &gLevelRoom20, &gLevelStage0, &gLevelRoom21 };
const struct level_room *const gLevel04Rooms[1] = { &gLevelRoom22 };
const struct level_room *const gLevel05Rooms[3] = { &gLevelRoom23, &gLevelStage1, &gLevelRoom24 };
const struct level_room *const gLevel06Rooms[1] = { &gLevelStage3 };
const struct level_room *const gLevel07Rooms[1] = { &gLevelRoom25 };
const struct level_room *const gLevel08Rooms[1] = { &gLevelRoom26 };
const struct level_room *const gLevel09Rooms[1] = { &gLevelRoom27 };
const struct level_room *const gLevel10Rooms[3] = { &gLevelRoom28, &gLevelStage2, &gLevelRoom29 };
const struct level_room *const gLevel11Rooms[1] = { &gLevelRoom30 };
const struct level_room *const gLevel12Rooms[1] = { &gLevelRoom31 };
const struct level_room *const gLevel13Rooms[1] = { &gLevelRoom32 };
const struct level_room *const gLevel14Rooms[1] = { &gLevelStage4 };
const struct level_room *const gLevel15Rooms[1] = { &gLevelRoom33 };
const struct level_room *const gLevel16Rooms[1] = { &gLevelRoom34 };
const struct level_room *const gLevel17Rooms[1] = { &gLevelStage5 };
const struct level_room *const gLevel18Rooms[1] = { &gLevelRoom35 };
const struct level_room *const gLevel19Rooms[1] = { &gLevelRoom36 };
const struct level_room *const gLevel24Rooms[1] = { &gLevelRoom37 };
const struct level_room *const gLevel20Rooms[1] = { &gLevelRoom38 };
const struct level_room *const gLevel22Rooms[1] = { &gLevelRoom39 };
const struct level_room *const gLevel23Rooms[1] = { &gLevelRoom40 };
const struct level_room *const gLevel21Rooms[1] = { &gLevelStage6 };

/* The rooms of the lists above (rooms 17-40 and the category stages). */
const struct level_room gLevelRoom17 = {
    gRoom17Palette, &gRoom17Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room17_25e7dc */
const struct level_room gLevelRoom18 = { gRoom18Palette, &gRoom18Desc, ROOM_KIND_UNDERWATER, 0, 1,
                                         0x1004 }; /* data/levels/room18_261c40 */
const struct level_room gLevelRoom19 = {
    gRoom19Palette, &gRoom19Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room19_260768 */
const struct level_room gLevelRoom20 = {
    gRoom20Palette, &gRoom20Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room20_25f11c */
const struct level_room gLevelStage0 = {
    NULL, NULL, ROOM_KIND_CATEGORY, 0, CATEGORY_FROSTBITE_CAVERN, 0
};
const struct level_room gLevelRoom21 = {
    gRoom21Palette, &gRoom21Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room21_25d81c */
const struct level_room gLevelRoom22 = { gRoom22Palette, &gRoom22Desc, ROOM_KIND_ON_FOOT, 0, 1,
                                         0x1004 }; /* data/levels/room22_26e760 */
const struct level_room gLevelRoom23 = {
    gRoom23Palette, &gRoom23Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room23_25cf20 */
const struct level_room gLevelStage1 = {
    NULL, NULL, ROOM_KIND_CATEGORY, 0, CATEGORY_SNOW_CRASH, 0
};
const struct level_room gLevelRoom24 = {
    gRoom24Palette, &gRoom24Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room24_25c640 */
const struct level_room gLevelStage3 = { NULL, NULL, ROOM_KIND_CATEGORY, 0, CATEGORY_ROCKET_RACKET,
                                         0 };
const struct level_room gLevelRoom25 = { gRoom25Palette, &gRoom25Desc, ROOM_KIND_ON_FOOT, 0, 1,
                                         0x1004 }; /* data/levels/room25_26d388 */
const struct level_room gLevelRoom26 = { gRoom26Palette, &gRoom26Desc, ROOM_KIND_UNDERWATER, 0, 1,
                                         0x1004 }; /* data/levels/room26_25bcdc */
const struct level_room gLevelRoom27 = {
    gRoom27Palette, &gRoom27Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room27_25a390 */
const struct level_room gLevelRoom28 = {
    gRoom28Palette, &gRoom28Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room28_254ed0 */
const struct level_room gLevelStage2 = { NULL, NULL, ROOM_KIND_CATEGORY, 0, CATEGORY_SNOW_JOB, 0 };
const struct level_room gLevelRoom29 = {
    gRoom29Palette, &gRoom29Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room29_2544fc */
const struct level_room gLevelRoom30 = {
    gRoom30Palette, &gRoom30Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room30_26ac10 */
const struct level_room gLevelRoom31 = { gRoom31Palette, &gRoom31Desc, ROOM_KIND_UNDERWATER, 0, 1,
                                         0x1004 }; /* data/levels/room31_2539b0 */
const struct level_room gLevelRoom32 = {
    gRoom32Palette, &gRoom32Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room32_24e104 */
const struct level_room gLevelStage4 = { NULL, NULL, ROOM_KIND_CATEGORY, 0, CATEGORY_BLIMP_BONANZA,
                                         0 };
const struct level_room gLevelRoom33 = {
    gRoom33Palette, &gRoom33Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room33_25233c */
const struct level_room gLevelRoom34 = { gRoom34Palette, &gRoom34Desc, ROOM_KIND_UNDERWATER, 0, 1,
                                         0x1004 }; /* data/levels/room34_24c400 */
const struct level_room gLevelStage5 = {
    NULL, NULL, ROOM_KIND_CATEGORY, 0, CATEGORY_NO_FLY_ZONE, 0
};
const struct level_room gLevelRoom35 = { gRoom35Palette, &gRoom35Desc, ROOM_KIND_ON_FOOT, 0, 1,
                                         0x1004 }; /* data/levels/room35_26c1bc */
const struct level_room gLevelRoom36 = {
    gRoom36Palette, &gRoom36Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room36_268cf0 */
const struct level_room gLevelRoom37 = {
    gRoom37Palette, &gRoom37Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room37_2b9ed0 */
const struct level_room gLevelRoom38 = {
    gRoom38Palette, &gRoom38Desc, ROOM_KIND_UNDERWATER, 0, 0, 0
}; /* data/levels/room38_2bac1c */
const struct level_room gLevelRoom39 = {
    gRoom39Palette, &gRoom39Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room39_2ba810 */
const struct level_room gLevelRoom40 = {
    gRoom40Palette, &gRoom40Desc, ROOM_KIND_ON_FOOT, 0, 0, 0
}; /* data/levels/room40_2bb094 */
const struct level_room gLevelStage6 = { NULL, NULL, ROOM_KIND_CATEGORY, 0, CATEGORY_N_GIN, 0 };
