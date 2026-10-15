extern "C" {
#include "core.h"
#include "engine/actor/actor_anim.h"
#include "engine/actor/actor.h"
}

/*
 * ROM 0x0803B8B0-0x080B2120: BG0 cell animation A (categories 0-2) and
 * category 0's spawnTable. Linked in ROM order between games/xs/data/data.s
 * sections by games/xs/ldscript.txt - see docs/data.md ("Category backgrounds").
 */

/* BG0 cell animation A, ActorCategory::descriptors[0..2].cellAnim
 * (cellAnimSize = its size, 0x75B94), played by InitCellAnim/
 * UploadCellAnimFrame (cell_anim.cpp): 19x13 cells, 60 frames. These are type-0
 * categories, so each frame carries its cells' palette banks after the
 * tiles. Built from games/xs/graphics/vehicles/polar/ice_wall.png, all 60
 * frames stacked top to bottom (tools/grit_bg.py interleaves grit's tiles
 * and banks per frame). */
struct polar_cell_anim {
    struct cell_anim_header header;
    struct {
        u8 tiles[19 * 13 * 32];
        u8 banks[(19 * 13 + 7) / 8 * 4];
    } frames[60];
};
COMPILE_TIME_ASSERT(polar_cell_anim_cpp, sizeof(struct polar_cell_anim) == 0x75B94);

extern const struct polar_cell_anim gCategoryFamily0CellAnim = {
    {
        {
#include "vehicles/polar/ice_wall.pal.inc"
        },
        19, 13,
    },
    {
#include "vehicles/polar/ice_wall_frames.inc"
    },
};

/* Category 0's spawn list (ActorCategory::descriptors[0].spawnTable),
 * generated from its games/xs/data/levels/spawns/ file (`levels.py spawns`,
 * docs/levels.md "Actor categories"). */
#include "spawns/gCategory0SpawnTable.inc"
