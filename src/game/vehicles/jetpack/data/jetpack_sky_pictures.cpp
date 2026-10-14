extern "C" {
#include "core.h"
#include "engine/actor/actor_anim.h"
#include "engine/actor/actor.h"
}

/*
 * ROM 0x08151AC4-0x0815A050: categories 4-6's BG1 pictures and
 * spawn tables. Linked in ROM order between data/data.s sections by
 * ldscript.txt - see docs/data.md ("Category backgrounds").
 */

/* Category 4's BG1 picture (ActorCategory::descriptors[4].bgPicture,
 * loaded by LoadBgPicture in bg_picture.cpp): 38x16 map, 237 tiles. Built from
 * graphics/vehicles/jetpack/skies/blimp_bonanza.png (palette and map) and
 * 151ac4_picture_tiles.png (the tile set, grit's -fx external tileset). */
struct jetpack_sky_pictures {
    struct bg_picture_header header;
    u16 map[38 * 16];
    u8 tiles[237 * 32];
    u8 banks[(38 * 16 + 1) / 2];
};
COMPILE_TIME_ASSERT(jetpack_sky_pictures_cpp, sizeof(struct jetpack_sky_pictures) == 0x208 + 608 * 2 + 237 * 32 + 608 / 2);

extern const struct jetpack_sky_pictures gCategory4BgPicture = {
    {
        {
#include "vehicles/jetpack/skies/blimp_bonanza.pal.inc"
        },
        38, 16, 237,
    },
    {
#include "vehicles/jetpack/skies/blimp_bonanza_map.inc"
    },
    {
#include "vehicles/jetpack/skies/blimp_bonanza_tiles.img.inc"
    },
    {
#include "vehicles/jetpack/skies/blimp_bonanza_banks.inc"
    },
};

/* Category 4's spawn list (ActorCategory::descriptors[4].spawnTable),
 * generated from its data/levels/spawns/ file (`levels.py spawns`,
 * docs/levels.md "Actor categories"). */
#include "spawns/gCategory4SpawnTable.inc"

/* The BG1 picture shared by categories 5 and 6
 * (ActorCategory::descriptors[5..6].bgPicture, loaded by LoadBgPicture
 * in bg_picture.cpp): 38x16 map, 374 tiles. Built from
 * graphics/vehicles/jetpack/skies/no_fly_zone.png (palette and map) and
 * 155260_picture_tiles.png (the tile set, grit's -fx external tileset). */
struct bg_picture_155260 {
    struct bg_picture_header header;
    u16 map[38 * 16];
    u8 tiles[374 * 32];
    u8 banks[(38 * 16 + 1) / 2];
};
COMPILE_TIME_ASSERT(jetpack_sky_pictures_cpp, sizeof(struct bg_picture_155260) == 0x208 + 608 * 2 + 374 * 32 + 608 / 2);

extern const struct bg_picture_155260 gCategory5BgPicture = {
    {
        {
#include "vehicles/jetpack/skies/no_fly_zone.pal.inc"
        },
        38, 16, 374,
    },
    {
#include "vehicles/jetpack/skies/no_fly_zone_map.inc"
    },
    {
#include "vehicles/jetpack/skies/no_fly_zone_tiles.img.inc"
    },
    {
#include "vehicles/jetpack/skies/no_fly_zone_banks.inc"
    },
};

/* Category 5's spawn list (ActorCategory::descriptors[5].spawnTable),
 * generated from its data/levels/spawns/ file (`levels.py spawns`,
 * docs/levels.md "Actor categories"). */
#include "spawns/gCategory5SpawnTable.inc"

/* Category 6's spawn list (ActorCategory::descriptors[6].spawnTable),
 * generated from its data/levels/spawns/ file (`levels.py spawns`,
 * docs/levels.md "Actor categories"). */
#include "spawns/gCategory6SpawnTable.inc"
