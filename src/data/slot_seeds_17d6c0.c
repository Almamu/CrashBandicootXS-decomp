#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0817D6C0-0x0817D7A4. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gStaticData_0817D7B8[];
extern const u8 gStaticData_0817D918[];
extern const u8 gStaticData_0817D9D8[];
extern const u8 gStaticData_0817DA98[];
extern const u8 gStaticData_0817DB58[];
extern const u8 gStaticData_0817DC18[];
extern const u8 gStaticData_0817DCD8[];
extern const u8 gStaticData_0817DD98[];
extern const u8 gStaticData_0817DE58[];
extern const u8 gStaticData_0817DF18[];
extern const u8 gStaticData_0817DFD8[];
extern const u8 gStaticData_0817E098[];
extern const u8 gStaticData_0817E158[];
extern const u8 gStaticData_0817E218[];
extern const u8 gStaticData_0817E298[];
extern const u8 gStaticData_0817E358[];
extern const u8 gStaticData_0817E418[];
extern const u8 gStaticData_0817E4D8[];
extern const u8 gStaticData_0817E598[];
extern const u8 gStaticData_0817E658[];
extern const u8 gStaticData_08631A68[];
extern const u8 gStaticData_08631A90[];
extern const u8 gStaticData_08631AB8[];
extern const u8 gStaticData_08634270[];
extern const u8 gStaticData_08636EF4[];
extern const u8 gStaticData_08637604[];

/* graphics_loading_35d1c.c's view of one countdown-slot seed. */
struct slot_seed
{
    const void *record;
    s32 hold;
};

/* Twenty {record, hold} seeds read by sub_8036600
 * (graphics_loading_35d1c.c): the motion sequences in
 * countdown_17d7a4.c, then a {NULL, 0} terminator. */
const struct slot_seed gStaticData_0817D6C0[21] = {
    { gStaticData_0817D7B8, 0 },
    { gStaticData_0817D918, 0x5a },
    { gStaticData_0817D9D8, 0x60 },
    { gStaticData_0817DA98, 0x64 },
    { gStaticData_0817DB58, 0x68 },
    { gStaticData_0817DC18, 0x6e },
    { gStaticData_0817DCD8, 0x73 },
    { gStaticData_0817DD98, 0x78 },
    { gStaticData_0817DE58, 0x7d },
    { gStaticData_0817DF18, 0x83 },
    { gStaticData_0817DFD8, 0x89 },
    { gStaticData_0817E098, 0x8a },
    { gStaticData_0817E158, 0x8f },
    { gStaticData_0817E218, 0x93 },
    { gStaticData_0817E298, 0x97 },
    { gStaticData_0817E358, 0x9b },
    { gStaticData_0817E418, 0x9f },
    { gStaticData_0817E4D8, 0xa3 },
    { gStaticData_0817E598, 0xa6 },
    { gStaticData_0817E658, 0xb2 },
    { NULL, 0 },
};

/* The three BG banks' packages sub_80361B0 (graphics_loading_35d1c.c)
 * loads as PKG_A/PKG_B/PKG_C: only the tiles and map are set. */
const struct bg_package gStaticData_0817D768 = {
    0,
    0,
    (void *)gStaticData_08631A68,
    (void *)gStaticData_08634270,
    NULL,
};

const struct bg_package gStaticData_0817D77C = {
    0,
    0,
    (void *)gStaticData_08631A90,
    (void *)gStaticData_08636EF4,
    NULL,
};

const struct bg_package gStaticData_0817D790 = {
    0,
    0,
    (void *)gStaticData_08631AB8,
    (void *)gStaticData_08637604,
    NULL,
};
