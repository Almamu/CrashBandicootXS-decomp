#ifndef GUARD_SPAWNERS_HPP
#define GUARD_SPAWNERS_HPP

/* The level's spawners as C++ (#664, docs/cplusplus.md, part 9).
 *
 * The entity spawner (EntitySpawner::instance, src/game/level/entity_spawner.cpp): the
 * table of spawn functions by entity type (gEntitySpawnFuncs), and the
 * methods that spawn the effect parts and the dropped pickups. Their
 * callers all pass EntitySpawner::Get(), the `this` the C called `pool` or
 * `unused`. It has no vtable: `delete instance` calls the
 * destructor directly. It has no C view (level.h's `struct
 * entity_spawner` tag went in #754).
 *
 * `#pragma interface`: no class here has a vtable, so there is none to emit;
 * the pragma keeps g++ from emitting out-of-line copies of the inline
 * methods (docs/cplusplus.md, "Emitting the vtables"). */
#pragma interface

#include "game/level/entity_flags.hpp"
#include "game/entities/sprite_obj.hpp"
#include "game/objects/pickups.hpp"
#include "engine/sys/key_input.hpp"

extern "C" {
#include "game/level/level.h"
#include "game/level/level_data.h"
#include "globals.h"
}

/* The parameter record of the room's entity `index` (level_data.h's
 * struct entity_params: the flags, then the per-type words). Through an
 * inline, the record's address gets a register copy of its own, as in
 * the ROM (Crate::SpawnBasic, SpawnStartMarker). */
static inline const struct entity_params *EntityParams(u16 index)
{
    const struct level_entity_list *lvl = gEntityFlags->list;

    return (const struct entity_params *)(lvl->params + lvl->paramOffsets[index]);
}

namespace Game
{
namespace Entities
{

/* The level's other part lists (level.h), as the class, as
 * TouchableList() and the others are (sprite_obj.hpp). */
static inline PartList *DecorationList()
{
    return gDecorationList;
}

/* The update-only list holds bare entities (the room exits and the seal
 * spawner): PartList calls only Entity's slots on them (IsInsideRect,
 * Update, OverlapsRect and the destructor). */
static inline void AddUpdateOnly(Entity *e)
{
    (gUpdateOnlyPartList)->Add((Sprite *)e);
}

} // namespace Entities

namespace Level
{

class EntitySpawner
{
public:
    const entity_spawn_fn *funcs; // 0x00 - gEntitySpawnFuncs
    s32 count;                    // 0x04 - its length

    EntitySpawner();  // InitEntitySpawner
    ~EntitySpawner(); // DestroyEntitySpawnerObj

    /* The singleton (#915), static: they take no spawner. PlayRoom
     * creates `instance` over gEntitySpawnFuncs when the room starts and
     * destroys it when it ends (spawn_markers.cpp). */
    static void Create();  // CreateEntitySpawner
    static void Destroy(); // DestroyEntitySpawner
    /* The room's spawner, between Create and Destroy: the crates, the
     * player, the enemies and the pickups all call through it. */
    static EntitySpawner *Get()
    {
        return instance;
    }
    /* LaunchEffectPart through `instance`, the new part harmful
     * (bit 2 set, bit 6 cleared): the enemies' floating popup and the
     * shooters' shots (enemy_ctrl.cpp). */
    static Game::Entities::MovingSprite *LaunchHarmfulEffectPart(s32 a, s32 b, s32 c, s32 d, s32 e,
                                                                 Game::Entities::MovingSprite *src);
    /* SpawnEntity: calls the spawn function of the record's type with the
     * record's three other halfwords. */
    void Spawn(u32 id, const struct level_entity *rec);
    void SetTable(const entity_spawn_fn *table, s32 n); // SetEntitySpawnerTable
    /* An effect part (a MovingSprite with an EffectCtrl) at (x, y),
     * clamped into the level, animation `anim` of the sprite banks. */
    Game::Entities::MovingSprite *SpawnEffectPart(s32 anim, s32 tag, s32 x, s32 y, s32 mirror);
    /* An effect part beside `src`, `margin` pixels clear of it, moving
     * away at `speed`. */
    Game::Entities::MovingSprite *LaunchEffectPart(s32 anim, s32 tag, s32 margin, s32 z, s32 speed,
                                                   Game::Entities::MovingSprite *src);
    /* A dropped wumpa and extra life (entity_spawner.cpp); `toHud`
     * flies it to the HUD. */
    Game::Objects::Wumpa *DropWumpa(u32 x, u32 y, u32 p3, u32 p4, bool toHud);
    Game::Objects::ExtraLife *DropExtraLife(u32 x, u32 y, u32 p3, u32 p5, bool toHud);

private:
    /* gEntitySpawner, the one spawner: an IWRAM global at 0x30012E4
     * (sym_iwram.txt; cxx_symbols.txt maps `_13EntitySpawner.instance`
     * to it), not a field. */
    static EntitySpawner *instance;
};

COMPILE_TIME_ASSERT(spawners_hpp, sizeof(EntitySpawner) == 8);

} // namespace Level
} // namespace Game

#endif /* !GUARD_SPAWNERS_HPP */
