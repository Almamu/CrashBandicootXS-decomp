#include "core.h"
#include "actor_anim.h"

/*
 * ROM 0x0817A728-0x0817A840. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* 16-colour palettes sub_802B730 and sub_802BB4C (actor_part127.c)
 * queue for OBJ palette 0; sub_802BB4C blinks between the two. */
const u16 gStaticData_0817A728[16] = {
    0x03E0, 0x768B, 0x7AF1, 0x4D83, 0x7586, 0x5D24, 0x3D04, 0x6E66,
    0x5589, 0x6A0B, 0x5DE5, 0x7A8F, 0x4231, 0x5B3A, 0x4EB5, 0x6BBE,
};

const u16 gStaticData_0817A748[16] = {
    0x03E0, 0x01DD, 0x00DE, 0x0050, 0x00B7, 0x3461, 0x0005, 0x31DB,
    0x18C8, 0x31B1, 0x294C, 0x52D8, 0x3E32, 0x5F3B, 0x4675, 0x6BBF,
};

/* Boxes (struct anim_box): the one sub_802C6C0 (actor_part19g.c) copies
 * into a part's +0x38 box, and the three sub_802CC9C (actor_part126.c)
 * picks from. */
const struct anim_box gStaticData_0817A768 = { -15, -15, -2, 30, 30, 5 };
const struct anim_box gStaticData_0817A774 = { -24, -25, 0, 10, 40, 1 };
const struct anim_box gStaticData_0817A780 = { 14, -25, 0, 10, 40, 1 };
const struct anim_box gStaticData_0817A78C = { -14, -25, 0, 28, 40, 1 };

/* 16-colour palettes actor_part126.c queues for OBJ palette 14:
 * sub_802D204 takes gStaticData_0817A798 + (tier - 1) * 0x20, so the
 * higher tiers read the palettes after it; sub_802D2DC blinks
 * between gStaticData_0817A7D8 and gStaticData_0817A7B8. */
const u16 gStaticData_0817A798[16] = {
    0x35AC, 0x2192, 0x14EF, 0x04F6, 0x1179, 0x023F, 0x03FF, 0x03E9,
    0x1248, 0x42FA, 0x5D87, 0x7E60, 0x4414, 0x401F, 0x001C, 0x0000,
};

const u16 gStaticData_0817A7B8[16] = {
    0x35AC, 0x26B4, 0x14EF, 0x04F6, 0x1179, 0x023F, 0x03FF, 0x03BF,
    0x02B7, 0x333C, 0x1AF7, 0x03FE, 0x0293, 0x03DF, 0x035F, 0x0000,
};

const u16 gStaticData_0817A7D8[16] = {
    0x03E0, 0x56B5, 0x2529, 0x2D6B, 0x3DEF, 0x4E73, 0x739C, 0x6F7B,
    0x4E73, 0x6739, 0x56B5, 0x6F7B, 0x4631, 0x6F7B, 0x6739, 0x0000,
};

/* The records sub_802DB2C and sub_802DCC0 (actor_part59.c) index by
 * gUnknown_030014D4: a value, then two random-roll thresholds (out of
 * 0x100) for the state change. */
const s32 gStaticData_0817A7F8[6][3] = {
    { 243, 5, 1 },
    { 243, 10, 1 },
    { 243, 10, 1 },
    { 243, 15, 2 },
    { 243, 20, 2 },
    { 243, 30, 3 },
};
