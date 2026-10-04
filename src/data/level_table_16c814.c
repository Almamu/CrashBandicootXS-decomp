#include "core.h"
#include "level_data.h"

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

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

COMPILE_TIME_ASSERT(sizeof(struct level_info) == 0x24);
COMPILE_TIME_ASSERT(sizeof(struct level_room_list) == 0x10);
COMPILE_TIME_ASSERT(sizeof(struct level_room) == 0x14);

/* The rooms' palettes and descriptors (src/data/level_rooms_*.c). */
extern const u16 gStaticData_0826443A[];
extern const struct level_desc gStaticData_08264730;
extern const u16 gStaticData_08263C14[];
extern const struct level_desc gStaticData_08263F4C;
extern const u16 gStaticData_08263410[];
extern const struct level_desc gStaticData_082636E0;
extern const u16 gStaticData_0827086E[];
extern const struct level_desc gStaticData_08270BCC;
extern const u16 gStaticData_08262B54[];
extern const struct level_desc gStaticData_08262E64;
extern const u16 gStaticData_0826FDFC[];
extern const struct level_desc gStaticData_08270154;
extern const u16 gStaticData_0826716E[];
extern const struct level_desc gStaticData_08267428;
extern const u16 gStaticData_08264B64[];
extern const struct level_desc gStaticData_08264E64;
extern const u16 gStaticData_08265484[];
extern const struct level_desc gStaticData_08265804;
extern const u16 gStaticData_0826235E[];
extern const struct level_desc gStaticData_08262654;
extern const u16 gStaticData_08265E66[];
extern const struct level_desc gStaticData_08266194;
extern const u16 gStaticData_0826F23A[];
extern const struct level_desc gStaticData_0826F5C0;
extern const u16 gStaticData_0826681A[];
extern const struct level_desc gStaticData_08266B40;
extern const u16 gStaticData_082BBB26[];
extern const struct level_desc gStaticData_082BBDE0;
extern const u16 gStaticData_082BCC84[];
extern const struct level_desc gStaticData_082BCF40;
extern const u16 gStaticData_082BDCDC[];
extern const struct level_desc gStaticData_082BDF98;
extern const u16 gStaticData_082BE822[];
extern const struct level_desc gStaticData_082BEADC;
extern const u16 gStaticData_0825E4EE[];
extern const struct level_desc gStaticData_0825E7DC;
extern const u16 gStaticData_082618C2[];
extern const struct level_desc gStaticData_08261C40;
extern const u16 gStaticData_08260458[];
extern const struct level_desc gStaticData_08260768;
extern const u16 gStaticData_0825EE4E[];
extern const struct level_desc gStaticData_0825F11C;
extern const u16 gStaticData_0825D544[];
extern const struct level_desc gStaticData_0825D81C;
extern const u16 gStaticData_0826E3F8[];
extern const struct level_desc gStaticData_0826E760;
extern const u16 gStaticData_0825CC4A[];
extern const struct level_desc gStaticData_0825CF20;
extern const u16 gStaticData_0825C35A[];
extern const struct level_desc gStaticData_0825C640;
extern const u16 gStaticData_0826D050[];
extern const struct level_desc gStaticData_0826D388;
extern const u16 gStaticData_0825B9B6[];
extern const struct level_desc gStaticData_0825BCDC;
extern const u16 gStaticData_08259FAA[];
extern const struct level_desc gStaticData_0825A390;
extern const u16 gStaticData_08254BEA[];
extern const struct level_desc gStaticData_08254ED0;
extern const u16 gStaticData_08254204[];
extern const struct level_desc gStaticData_082544FC;
extern const u16 gStaticData_0826A882[];
extern const struct level_desc gStaticData_0826AC10;
extern const u16 gStaticData_08253642[];
extern const struct level_desc gStaticData_082539B0;
extern const u16 gStaticData_0824DD8C[];
extern const struct level_desc gStaticData_0824E104;
extern const u16 gStaticData_08251FA6[];
extern const struct level_desc gStaticData_0825233C;
extern const u16 gStaticData_0824C0D2[];
extern const struct level_desc gStaticData_0824C400;
extern const u16 gStaticData_0826BE4E[];
extern const struct level_desc gStaticData_0826C1BC;
extern const u16 gStaticData_0826899A[];
extern const struct level_desc gStaticData_08268CF0;
extern const u16 gStaticData_082B9B58[];
extern const struct level_desc gStaticData_082B9ED0;
extern const u16 gStaticData_082BA962[];
extern const struct level_desc gStaticData_082BAC1C;
extern const u16 gStaticData_082BA554[];
extern const struct level_desc gStaticData_082BA810;
extern const u16 gStaticData_082BADDA[];
extern const struct level_desc gStaticData_082BB094;

extern const struct level_room_list gLevelRoomLists[25];
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
 * sub_8023A1C (game_loop56.c), one list per theme case: each is the
 * `lists` argument of a sub_8027018 call on BG palette RAM.
 */
const u16 gThemePaletteCycle2[5] = { 0xb1, 0xb2, 0xb3, 0xb4, 0xb5 };
const u16 gThemePaletteCycle1A[9] = { 0x39, 0x3a, 0x3d, 0x3e, 0x75, 0x82, 0xbc, 0xea, 0xfc };
const u16 gThemePaletteCycle1B[9] = { 0x51, 0x52, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0xf1 };
const u16 gThemePaletteCycle3[16] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
};
const u16 gThemePaletteCycle5[5] = { 0x97, 0xb4, 0xf7, 0xf8, 0xff };

/*
 * The levels, indexed by level id (the game's level numbering; `nameText`
 * is the text id of the level's name). Read through local views all over
 * the game loop: `struct level_info` (actor_part_1b85c.c),
 * `threshold_table_entry` (settings_menu6.c, oam_count.c),
 * `MedalTableEntry` (game_loop17.c, game_loop18.c), `level_guard`
 * (graphics_loading_21280.c) and `gl_level_entry` (game_loop56.c).
 */
const struct level_info gLevelTable[25] = {
    { 1, 1, { 355, 275, 233 }, 4, 4, 0, &gLevelRoomLists[0] },
    { 2, 3, { 777, 677, 544 }, 4, 1, 0, &gLevelRoomLists[1] },
    { 3, 1, { 627, 557, 490 }, 4, 4, 0, &gLevelRoomLists[2] },
    { 4, 0, { 917, 771, 660 }, 4, 4, 0, &gLevelRoomLists[3] },
    { 5, 2, { 994, 756, 665 }, 5, 5, 0, &gLevelRoomLists[4] },
    { 6, 0, { 1030, 967, 880 }, 5, 5, 0, &gLevelRoomLists[5] },
    { 7, 4, { 1502, 1375, 1250 }, 5, 5, 0, &gLevelRoomLists[6] },
    { 8, 2, { 1094, 1030, 870 }, 5, 5, 0, &gLevelRoomLists[7] },
    { 9, 3, { 916, 770, 694 }, 5, 1, 0, &gLevelRoomLists[8] },
    { 10, 1, { 1618, 1588, 1200 }, 5, 5, 0, &gLevelRoomLists[9] },
    { 11, 0, { 1321, 1280, 1244 }, 6, 6, 0, &gLevelRoomLists[10] },
    { 12, 5, { 1184, 1158, 995 }, 6, 6, 0, &gLevelRoomLists[11] },
    { 13, 3, { 996, 817, 742 }, 6, 1, 0, &gLevelRoomLists[12] },
    { 14, 1, { 924, 834, 674 }, 6, 6, 0, &gLevelRoomLists[13] },
    { 15, 4, { 1782, 1649, 1563 }, 6, 6, 0, &gLevelRoomLists[14] },
    { 16, 5, { 1276, 1195, 1041 }, 7, 7, 0, &gLevelRoomLists[15] },
    { 17, 3, { 1293, 1104, 1017 }, 7, 1, 0, &gLevelRoomLists[16] },
    { 18, 4, { 2063, 1971, 1896 }, 7, 7, 0, &gLevelRoomLists[17] },
    { 19, 2, { 1484, 970, 896 }, 7, 7, 0, &gLevelRoomLists[18] },
    { 20, 5, { 1504, 1338, 1182 }, 7, 7, 0, &gLevelRoomLists[19] },
    { 22, 7, { 1000, 500, 250 }, 4, 5, 1, &gLevelRoomLists[20] },
    { 23, 8, { 1000, 500, 250 }, 5, 5, 1, &gLevelRoomLists[21] },
    { 21, 6, { 1000, 500, 250 }, 6, 6, 1, &gLevelRoomLists[22] },
    { 24, 9, { 1000, 500, 250 }, 7, 7, 1, &gLevelRoomLists[23] },
    { 25, 10, { 1000, 500, 250 }, 7, 7, 1, &gLevelRoomLists[24] },
};

/* Each level's rooms (the `rooms` of its `struct level_info`). */
const struct level_room_list gLevelRoomLists[25] = {
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
 * sub_8024498 (game_loop18.c).
 */
const u8 gThemeMusicCues[11] = { 2, 0, 3, 1, 5, 4, 7, 8, 9, 10, 10 };

/* The extra rooms of the room lists (rooms 00-16). */
const struct level_room gLevelRoom00 = { gStaticData_0826443A, &gStaticData_08264730, 0, 0, 0, 0 }; /* data/levels/room00_264730 */
const struct level_room gLevelRoom01 = { gStaticData_08263C14, &gStaticData_08263F4C, 0, 0, 0, 0 }; /* data/levels/room01_263f4c */
const struct level_room gLevelRoom02 = { gStaticData_08263410, &gStaticData_082636E0, 0, 0, 0, 0 }; /* data/levels/room02_2636e0 */
const struct level_room gLevelRoom03 = { gStaticData_0827086E, &gStaticData_08270BCC, 0, 0, 0, 0 }; /* data/levels/room03_270bcc */
const struct level_room gLevelRoom04 = { gStaticData_08262B54, &gStaticData_08262E64, 0, 0, 0, 0 }; /* data/levels/room04_262e64 */
const struct level_room gLevelRoom05 = { gStaticData_0826FDFC, &gStaticData_08270154, 0, 0, 0, 0 }; /* data/levels/room05_270154 */
const struct level_room gLevelRoom06 = { gStaticData_0826716E, &gStaticData_08267428, 0, 0, 0, 0 }; /* data/levels/room06_267428 */
const struct level_room gLevelRoom07 = { gStaticData_08264B64, &gStaticData_08264E64, 0, 0, 0, 0 }; /* data/levels/room07_264e64 */
const struct level_room gLevelRoom08 = { gStaticData_08265484, &gStaticData_08265804, 0, 0, 0, 0 }; /* data/levels/room08_265804 */
const struct level_room gLevelRoom09 = { gStaticData_0826235E, &gStaticData_08262654, 0, 0, 0, 0 }; /* data/levels/room09_262654 */
const struct level_room gLevelRoom10 = { gStaticData_08265E66, &gStaticData_08266194, 0, 0, 0, 0 }; /* data/levels/room10_266194 */
const struct level_room gLevelRoom11 = { gStaticData_0826F23A, &gStaticData_0826F5C0, 0, 0, 0, 0 }; /* data/levels/room11_26f5c0 */
const struct level_room gLevelRoom12 = { gStaticData_0826681A, &gStaticData_08266B40, 0, 0, 0, 0 }; /* data/levels/room12_266b40 */
const struct level_room gLevelRoom13 = { gStaticData_082BBB26, &gStaticData_082BBDE0, 0, 0, 0, 0 }; /* data/levels/room13_2bbde0 */
const struct level_room gLevelRoom14 = { gStaticData_082BCC84, &gStaticData_082BCF40, 0, 0, 0, 0 }; /* data/levels/room14_2bcf40 */
const struct level_room gLevelRoom15 = { gStaticData_082BDCDC, &gStaticData_082BDF98, 0, 0, 0, 0 }; /* data/levels/room15_2bdf98 */
const struct level_room gLevelRoom16 = { gStaticData_082BE822, &gStaticData_082BEADC, 2, 0, 0, 0 }; /* data/levels/room16_2beadc */

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
const struct level_room gLevelRoom17 = { gStaticData_0825E4EE, &gStaticData_0825E7DC, 0, 0, 0, 0 }; /* data/levels/room17_25e7dc */
const struct level_room gLevelRoom18 = { gStaticData_082618C2, &gStaticData_08261C40, 1, 0, 1, 0x1004 }; /* data/levels/room18_261c40 */
const struct level_room gLevelRoom19 = { gStaticData_08260458, &gStaticData_08260768, 0, 0, 0, 0 }; /* data/levels/room19_260768 */
const struct level_room gLevelRoom20 = { gStaticData_0825EE4E, &gStaticData_0825F11C, 0, 0, 0, 0 }; /* data/levels/room20_25f11c */
const struct level_room gLevelStage0 = { NULL, NULL, 3, 0, 0, 0 }; /* actor category 0 */
const struct level_room gLevelRoom21 = { gStaticData_0825D544, &gStaticData_0825D81C, 0, 0, 0, 0 }; /* data/levels/room21_25d81c */
const struct level_room gLevelRoom22 = { gStaticData_0826E3F8, &gStaticData_0826E760, 0, 0, 1, 0x1004 }; /* data/levels/room22_26e760 */
const struct level_room gLevelRoom23 = { gStaticData_0825CC4A, &gStaticData_0825CF20, 0, 0, 0, 0 }; /* data/levels/room23_25cf20 */
const struct level_room gLevelStage1 = { NULL, NULL, 3, 0, 1, 0 }; /* actor category 1 */
const struct level_room gLevelRoom24 = { gStaticData_0825C35A, &gStaticData_0825C640, 0, 0, 0, 0 }; /* data/levels/room24_25c640 */
const struct level_room gLevelStage3 = { NULL, NULL, 3, 0, 3, 0 }; /* actor category 3 */
const struct level_room gLevelRoom25 = { gStaticData_0826D050, &gStaticData_0826D388, 0, 0, 1, 0x1004 }; /* data/levels/room25_26d388 */
const struct level_room gLevelRoom26 = { gStaticData_0825B9B6, &gStaticData_0825BCDC, 1, 0, 1, 0x1004 }; /* data/levels/room26_25bcdc */
const struct level_room gLevelRoom27 = { gStaticData_08259FAA, &gStaticData_0825A390, 0, 0, 0, 0 }; /* data/levels/room27_25a390 */
const struct level_room gLevelRoom28 = { gStaticData_08254BEA, &gStaticData_08254ED0, 0, 0, 0, 0 }; /* data/levels/room28_254ed0 */
const struct level_room gLevelStage2 = { NULL, NULL, 3, 0, 2, 0 }; /* actor category 2 */
const struct level_room gLevelRoom29 = { gStaticData_08254204, &gStaticData_082544FC, 0, 0, 0, 0 }; /* data/levels/room29_2544fc */
const struct level_room gLevelRoom30 = { gStaticData_0826A882, &gStaticData_0826AC10, 0, 0, 0, 0 }; /* data/levels/room30_26ac10 */
const struct level_room gLevelRoom31 = { gStaticData_08253642, &gStaticData_082539B0, 1, 0, 1, 0x1004 }; /* data/levels/room31_2539b0 */
const struct level_room gLevelRoom32 = { gStaticData_0824DD8C, &gStaticData_0824E104, 0, 0, 0, 0 }; /* data/levels/room32_24e104 */
const struct level_room gLevelStage4 = { NULL, NULL, 3, 0, 4, 0 }; /* actor category 4 */
const struct level_room gLevelRoom33 = { gStaticData_08251FA6, &gStaticData_0825233C, 0, 0, 0, 0 }; /* data/levels/room33_25233c */
const struct level_room gLevelRoom34 = { gStaticData_0824C0D2, &gStaticData_0824C400, 1, 0, 1, 0x1004 }; /* data/levels/room34_24c400 */
const struct level_room gLevelStage5 = { NULL, NULL, 3, 0, 5, 0 }; /* actor category 5 */
const struct level_room gLevelRoom35 = { gStaticData_0826BE4E, &gStaticData_0826C1BC, 0, 0, 1, 0x1004 }; /* data/levels/room35_26c1bc */
const struct level_room gLevelRoom36 = { gStaticData_0826899A, &gStaticData_08268CF0, 0, 0, 0, 0 }; /* data/levels/room36_268cf0 */
const struct level_room gLevelRoom37 = { gStaticData_082B9B58, &gStaticData_082B9ED0, 0, 0, 0, 0 }; /* data/levels/room37_2b9ed0 */
const struct level_room gLevelRoom38 = { gStaticData_082BA962, &gStaticData_082BAC1C, 1, 0, 0, 0 }; /* data/levels/room38_2bac1c */
const struct level_room gLevelRoom39 = { gStaticData_082BA554, &gStaticData_082BA810, 0, 0, 0, 0 }; /* data/levels/room39_2ba810 */
const struct level_room gLevelRoom40 = { gStaticData_082BADDA, &gStaticData_082BB094, 0, 0, 0, 0 }; /* data/levels/room40_2bb094 */
const struct level_room gLevelStage6 = { NULL, NULL, 3, 0, 6, 0 }; /* actor category 6 */
