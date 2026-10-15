extern "C" {
#include "core.h"
#include "engine/actor/actor_anim.h"
#include "engine/actor/actor.h"
}

/*
 * ROM 0x080C0C38-0x080C2758: categories 1 and 2's spawn tables.
 * Linked in ROM order between data/data.s sections by games/xs/ldscript.txt - see
 * docs/data.md.
 */

/* Category 1's spawn list (ActorCategory::descriptors[1].spawnTable),
 * generated from its data/levels/spawns/ file (`levels.py spawns`,
 * docs/levels.md "Actor categories"). */
#include "spawns/gCategory1SpawnTable.inc"

/* Category 2's spawn list (ActorCategory::descriptors[2].spawnTable),
 * generated from its data/levels/spawns/ file (`levels.py spawns`,
 * docs/levels.md "Actor categories"). */
#include "spawns/gCategory2SpawnTable.inc"
