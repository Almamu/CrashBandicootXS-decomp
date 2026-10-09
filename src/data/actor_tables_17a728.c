#include "core.h"
#include "actor_anim.h"
#include "vehicle.h"

/*
 * ROM 0x0817A728-0x0817A840. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* 16-colour palettes HurtPolarPlayer and PolarPlayerStateShocked (polar_player.cpp)
 * queue for OBJ palette 0; PolarPlayerStateShocked blinks between the two. */
const u16 gPolarPlayerShockPalette[16] = {
    0x03E0, 0x768B, 0x7AF1, 0x4D83, 0x7586, 0x5D24, 0x3D04, 0x6E66,
    0x5589, 0x6A0B, 0x5DE5, 0x7A8F, 0x4231, 0x5B3A, 0x4EB5, 0x6BBE,
};

const u16 gPolarPlayerShockBlinkPalette[16] = {
    0x03E0, 0x01DD, 0x00DE, 0x0050, 0x00B7, 0x3461, 0x0005, 0x31DB,
    0x18C8, 0x31B1, 0x294C, 0x52D8, 0x3E32, 0x5F3B, 0x4675, 0x6BBF,
};

/* Boxes (struct anim_box): the one UpdatePolarNitroCrate (polar_crate.cpp) copies
 * into a part's +0x38 box, and the three UpdatePolarElectricFence (polar_objects.cpp)
 * picks from. */
const struct anim_box gPolarNitroCrateBox = { -15, -15, -2, 30, 30, 5 };
const struct anim_box gPolarElectricFenceLeftPostBox = { -24, -25, 0, 10, 40, 1 };
const struct anim_box gPolarElectricFenceRightPostBox = { 14, -25, 0, 10, 40, 1 };
const struct anim_box gPolarElectricFenceWireBox = { -14, -25, 0, 28, 40, 1 };

/* 16-colour palettes polar_objects.cpp queues for OBJ palette 14:
 * RefreshPolarAkuAku takes gPolarAkuAkuPalette1 + (tier - 1) * 0x20, so the
 * higher tiers read the palettes after it; UpdatePolarAkuAku blinks
 * between gPolarAkuAkuPalette3 and gPolarAkuAkuPalette2. */
const u16 gPolarAkuAkuPalette1[16] = {
    0x35AC, 0x2192, 0x14EF, 0x04F6, 0x1179, 0x023F, 0x03FF, 0x03E9,
    0x1248, 0x42FA, 0x5D87, 0x7E60, 0x4414, 0x401F, 0x001C, 0x0000,
};

const u16 gPolarAkuAkuPalette2[16] = {
    0x35AC, 0x26B4, 0x14EF, 0x04F6, 0x1179, 0x023F, 0x03FF, 0x03BF,
    0x02B7, 0x333C, 0x1AF7, 0x03FE, 0x0293, 0x03DF, 0x035F, 0x0000,
};

const u16 gPolarAkuAkuPalette3[16] = {
    0x03E0, 0x56B5, 0x2529, 0x2D6B, 0x3DEF, 0x4E73, 0x739C, 0x6F7B,
    0x4E73, 0x6739, 0x56B5, 0x6F7B, 0x4631, 0x6F7B, 0x6739, 0x0000,
};

/* The records YetiStateChase and YetiStateCharge (yeti_states.cpp) index by
 * gYetiParamsIndex: a value, then two random-roll thresholds (out of
 * 0x100) for the state change. */
const s32 gYetiChargeParams[6][3] = {
    { 243, 5, 1 },
    { 243, 10, 1 },
    { 243, 10, 1 },
    { 243, 15, 2 },
    { 243, 20, 2 },
    { 243, 30, 3 },
};
