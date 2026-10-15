extern "C" {
#include "core.h"
#include "engine/actor/actor_anim.h"
#include "engine/actor/actor.h"
}

/*
 * ROM 0x080FF1B0-0x0814174C: BG0 cell animation B (categories 3-6),
 * category 3's BG1 picture and its spawnTable. Linked in ROM order
 * between games/xs/data/data.s sections by games/xs/ldscript.txt - see docs/data.md
 * ("Category backgrounds").
 */

/* BG0 cell animation B, ActorCategory::descriptors[3..6].cellAnim
 * (cellAnimSize = its size, 0x3E784), played by InitCellAnim/
 * UploadCellAnimFrame (cell_anim.cpp): 38x10 cells, 21 frames. Not a type-0
 * category, so there are no per-cell banks: ResetCellAnimBg maps the cells to
 * consecutive tiles in palette bank 0. Built from
 * games/xs/graphics/vehicles/jetpack/clouds.png, all 21 frames stacked top to
 * bottom. */
struct jetpack_cell_anim {
    struct cell_anim_header header;
    struct {
        u8 tiles[38 * 10 * 32];
    } frames[21];
};
COMPILE_TIME_ASSERT(jetpack_cell_anim_cpp, sizeof(struct jetpack_cell_anim) == 0x3E784);

extern const struct jetpack_cell_anim gCategoryFamily1CellAnim = {
    {
        {
#include "vehicles/jetpack/clouds.pal.inc"
        },
        38, 10,
    },
    {
#include "vehicles/jetpack/clouds_frames.inc"
    },
};

/* Category 3's BG1 picture (ActorCategory::descriptors[3].bgPicture,
 * loaded by LoadBgPicture in bg_picture.cpp): 38x16 map, 357 tiles. Built from
 * games/xs/graphics/vehicles/jetpack/skies/rocket_racket.png (palette and map) and
 * 13d934_picture_tiles.png (the tile set, grit's -fx external tileset). */
struct bg_picture_13d934 {
    struct bg_picture_header header;
    u16 map[38 * 16];
    u8 tiles[357 * 32];
    u8 banks[(38 * 16 + 1) / 2];
};
COMPILE_TIME_ASSERT(jetpack_cell_anim_cpp, sizeof(struct bg_picture_13d934) == 0x208 + 608 * 2 + 357 * 32 + 608 / 2);

extern const struct bg_picture_13d934 gCategory3BgPicture = {
    {
        {
#include "vehicles/jetpack/skies/rocket_racket.pal.inc"
        },
        38, 16, 357,
    },
    {
#include "vehicles/jetpack/skies/rocket_racket_map.inc"
    },
    {
#include "vehicles/jetpack/skies/rocket_racket_tiles.img.inc"
    },
    {
#include "vehicles/jetpack/skies/rocket_racket_banks.inc"
    },
};

/* Category 3's spawn list (ActorCategory::descriptors[3].spawnTable),
 * generated from its games/xs/data/levels/spawns/ file (`levels.py spawns`,
 * docs/levels.md "Actor categories"). */
#include "spawns/gCategory3SpawnTable.inc"
