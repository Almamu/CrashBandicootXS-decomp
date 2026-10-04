#include "core.h"

/*
 * ROM 0x0816C6A4-0x0816C814. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void nullsub_21();
extern void nullsub_22();
extern void sub_801E990();
extern void sub_801EA5C();
extern void sub_801EBF0();
extern void sub_801EC9C();
extern void sub_801ED6C();
extern void sub_801EE3C();
extern void sub_801EF0C();
extern void sub_801F050();
extern void sub_801F170();
extern void sub_801F2BC();
extern void sub_801F3DC();
extern void sub_801F528();
extern void sub_801F7B8();
extern void sub_801F8DC();
extern void sub_801FA3C();
extern void sub_801FB74();
extern void sub_801FCB4();
extern void sub_801FDEC();
extern void sub_801FEEC();
extern void sub_8020010();
extern void sub_8020138();
extern void sub_802026C();
extern void sub_80203A8();
extern void sub_80204EC();
extern void sub_802062C();
extern void sub_8020788();
extern void sub_80208C4();
extern void sub_80209EC();
extern void sub_8020B0C();
extern void sub_8020C18();
extern void sub_8020D4C();
extern void sub_8020E84();
extern void sub_8020F7C();
extern void sub_802107C();
extern void sub_802117C();
extern void sub_8021280();
extern void sub_8021388();
extern void sub_8021480();
extern void sub_802155C();
extern void sub_8021668();
extern void sub_8021748();
extern void sub_802183C();
extern void sub_80218C4();
extern void sub_80218E8();
extern void sub_802190C();
extern void sub_8021974();
extern void sub_8021998();
extern void sub_80219BC();
extern void sub_80219E0();
extern void sub_8021A00();
extern void sub_8021A4C();
extern void sub_8021A70();
extern void sub_8021A94();
extern void sub_8021AB8();
extern void sub_8021ADC();
extern void sub_8021B00();
extern void sub_8021B24();
extern void sub_8021B48();
extern void sub_8021B6C();
extern void sub_8021B90();
extern void sub_8021BB4();
extern void sub_8021BD8();
extern void sub_8021BFC();
extern void sub_8021C50();
extern void sub_8021C74();
extern void sub_8021C98();
extern void sub_8021CBC();
extern void sub_8021CE0();
extern void sub_8021D04();
extern void sub_8021D80();
extern void sub_8021DFC();
extern void sub_8021E78();
extern void sub_8021EF4();
extern void sub_8021F70();
extern void sub_802200C();
extern void sub_802209C();
extern void sub_8022158();
extern void sub_802218C();
extern void sub_80221A4();
extern void sub_80221BC();
extern void sub_80221D4();

/* The unified 92-slot function-pointer dispatch array (docs/rom_map.md
 * "Major correction: there is no second table"): the spawn function of
 * each entity type of the room data (docs/levels.md, "Entities").
 * CreateEntitySpawner (graphics_loading_21d80.c) hands it to sub_8025D4C
 * with a count of 0x5c, as gEntitySpawner; SpawnEntity calls entry
 * `type` with the entity's id, x, y and param. */
void (*const gEntitySpawnFuncs[92])() = {
    sub_801E990,
    sub_80221D4,
    sub_80221BC,
    sub_80221A4,
    sub_802218C,
    nullsub_22,
    sub_8022158,
    sub_801EA5C,
    sub_801EBF0,
    sub_802200C,
    sub_801EC9C,
    sub_801ED6C,
    sub_801EE3C,
    nullsub_21,
    nullsub_21,
    nullsub_21,
    sub_8021F70,
    nullsub_21,
    sub_8021EF4,
    sub_8021E78,
    sub_8021D80,
    sub_8021D04,
    sub_8021CE0,
    sub_8021CBC,
    sub_8021C98,
    sub_8021C74,
    sub_8021C50,
    sub_8021BFC,
    sub_8021BD8,
    sub_8021BB4,
    sub_8021B90,
    sub_8021B6C,
    sub_8021B48,
    sub_8021B24,
    sub_8021B00,
    sub_8021ADC,
    sub_8021AB8,
    sub_8021A94,
    sub_8021A70,
    sub_8021A4C,
    sub_801EF0C,
    sub_801F050,
    sub_801F170,
    sub_801F2BC,
    sub_801F3DC,
    sub_801F528,
    sub_8021A00,
    sub_801F7B8,
    sub_801F8DC,
    sub_801FA3C,
    sub_801FB74,
    sub_801FCB4,
    sub_801FDEC,
    sub_801FEEC,
    nullsub_21,
    sub_8020010,
    sub_8020138,
    sub_802026C,
    sub_80203A8,
    sub_80204EC,
    nullsub_21,
    sub_80219E0,
    nullsub_21,
    sub_80203A8,
    sub_802062C,
    sub_8020788,
    sub_80208C4,
    sub_80209EC,
    sub_8020B0C,
    sub_8021388,
    nullsub_21,
    sub_8021480,
    sub_802155C,
    sub_8021668,
    sub_8021DFC,
    sub_8020C18,
    sub_8020C18,
    sub_8020D4C,
    sub_80219BC,
    sub_8021998,
    sub_8021974,
    sub_8020E84,
    sub_8020F7C,
    sub_802107C,
    sub_802117C,
    sub_8021280,
    sub_802190C,
    sub_80218E8,
    sub_80218C4,
    sub_802209C,
    sub_802183C,
    sub_8021748,
};
