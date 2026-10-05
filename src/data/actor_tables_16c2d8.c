#include "core.h"

/*
 * ROM 0x0816C2D8-0x0816C418. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The 12-byte vector records sub_8017ECC and its siblings
 * (actor_part27b.c) look up by type id. */
const s32 gStaticData_0816C2D8[4][3] = {
    { 0, 0, 0 },
    { 0, 32, 450 },
    { 0, 32, 620 },
    { 450, 32, 750 },
};

/* UpdateTiny / SetTinyState (actor_part_18008.c): a value per round. */
const u8 gTinyRoundAnchors[3] = {
    4, 1, 0,
};

/* PickTinyHopTarget (actor_part_18008.c): a sequence of 0-4 values. */
const u8 gTinyHopTargets[77] = {
    1, 1, 2, 1, 1, 0, 2, 0, 3, 3, 0, 0, 0, 3, 3, 1,
    1, 2, 4, 4, 3, 3, 3, 3, 3, 1, 1, 2, 1, 1, 0, 2,
    0, 3, 3, 0, 0, 0, 3, 3, 1, 1, 2, 2, 3, 3, 3, 3,
    3, 3, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 0, 0,
};

/* sub_80196B8 (actor_part_1967c.c): indexed by the source's index. */
const u8 gStaticData_0816C358[4] = {
    0x10, 0xE, 0xA, 0x20,
};

/* sub_8018E4C (actor_part_188d0.c): step counts and two timer
 * thresholds per config index. */
const u8 gStaticData_0816C35C[3] = {
    24, 22, 18,
};
const u8 gStaticData_0816C35F[3] = {
    4, 4, 4,
};
const u8 gStaticData_0816C362[3] = {
    2, 2, 2,
};

/* UpdateDingodile (actor_part_1967c.c): two pairs of threshold tables. */
const s32 gStaticData_0816C368[4] = {
    0xF000, 0xA000, 0x4B00, 0x0,
};
const s32 gStaticData_0816C378[6] = {
    0x10400, 0xD200, 0xA000, 0x6E00, 0x4B00, 0x0,
};
const s32 gStaticData_0816C390[4] = {
    0x4B00, 0xA000, 0xF000, 0x40000,
};
const s32 gStaticData_0816C3A0[6] = {
    0x4B00, 0x6E00, 0xA000, 0xD200, 0xFA00, 0x40000,
};

/* {x, y, z} vectors (gobj_1a794.h's `struct vec3`): sub_801A64C
 * (actor_part_1967c.c), and sub_801A7AC (actor_part_1a794.c) through the
 * entries of entry_set_16c418.c. */
const s32 gStaticData_0816C3B8[4][3] = {
    { 0, 0, 0 },
    { 0, 180, 300 },
    { -400, 30, 0 },
    { 0, 288, 288 },
};

/* The argument blocks sub_801A2A8 (actor_part_1967c.c) passes to a
 * method. */
const s32 gStaticData_0816C3E8[3] = {
    0, 7, -1024,
};
const s32 gStaticData_0816C3F4[9] = {
    0, 4, 736, -1024, 64, 1024, -600, 10, 0,
};
