#ifndef GUARD_SPAWNERS_HPP
#define GUARD_SPAWNERS_HPP

/* The level's spawners as C++ (#664, docs/cplusplus.md, part 9).
 *
 * The entity spawner (gEntitySpawner, src/level/entity_spawner.cpp): the
 * table of spawn functions by entity type (gEntitySpawnFuncs), and the
 * methods that spawn the effect parts and the dropped pickups. Their
 * callers all pass gEntitySpawner, the `this` the C called `pool` or
 * `unused`. It has no vtable: `delete gEntitySpawner` calls the
 * destructor directly. level.h's struct entity_spawner is its C view.
 *
 * `#pragma interface`: no class here has a vtable, so there is none to emit;
 * the pragma keeps g++ from emitting out-of-line copies of the inline
 * methods (docs/cplusplus.md, "Emitting the vtables"). */
#pragma interface

#include "sprite_obj.hpp"
#include "pickups.hpp"

extern "C" {
#include "level.h"
#include "level_data.h"
#include "globals.h"
}

/* The parameter record of the room's entity `index` (level_data.h's
 * struct entity_params: the flags, then the per-type words). Through an
 * inline, the record's address gets a register copy of its own, as in
 * the ROM (SpawnBasicCrate, SpawnStartMarker). */
static inline const struct entity_params *EntityParams(u16 index)
{
    const struct level_entity_list *lvl = gEntityFlags->list;

    return (const struct entity_params *)((const u8 *)lvl->params + lvl->paramOffsets[index]);
}

/* The level's other part lists (level.h), as the class, as
 * TouchableList() and the others are (sprite_obj.hpp). */
static inline PartList *DecorationList()
{
    return (PartList *)gDecorationList;
}

/* The update-only list holds bare entities (the room exits and the seal
 * spawner): PartList calls only Entity's slots on them (IsInsideRect,
 * Update, OverlapsRect and the destructor). */
static inline void AddUpdateOnly(Entity *e)
{
    ((PartList *)gUpdateOnlyPartList)->Add((Sprite *)e);
}

/* The room's entity flags (gEntityFlags; src/level/entity_flags.cpp):
 * level.h's struct entity_flags, the room's entity list and its "gone"
 * and "activated" bitmaps, with the bitmaps' accessors. It has no vtable.
 * InitLevelState (spawn_pickups.cpp) makes it. */
class LevelEntityFlags : public entity_flags
{
public:
    LevelEntityFlags();                                           // InitEntityFlags
    ~LevelEntityFlags();                                          // DestroyEntityFlags
    s32 CountCrateEntities(const struct level_entity_list *list); // CountCrateEntities
    void SetGone(s32 n);                                          // SetEntityIdGone (UNUSED)
    s32 IsGone(s32 n);                                            // IsEntityIdGone
    s32 IsActivated(s32 n);                                       // IsEntityIdActivated
    void SetActivated(s32 n);                                     // SetEntityIdActivated
    void MarkActivated(s32 n);                                    // MarkEntityIdActivated
    void SetPos(s32 val);                                         // SetEntityFlagsPos (UNUSED)
};

COMPILE_TIME_ASSERT(spawners_hpp, sizeof(LevelEntityFlags) == 0x408);

/* The key input object (gInput; system.h): UpdateKeys and
 * GetDpadDirection take it but read gKeys. Its constructor is ClearKeys
 * (src/system/irq.c, still C, declared `void ClearKeys(void)` there),
 * which leaves `this` in r0 as a constructor returns it. */
class KeyInput
{
public:
    u32 unused;

    KeyInput(); // ClearKeys
};

class EntitySpawner
{
public:
    const entity_spawn_fn *funcs; // 0x00 - gEntitySpawnFuncs
    s32 count;                    // 0x04 - its length

    EntitySpawner();  // InitEntitySpawner
    ~EntitySpawner(); // DestroyEntitySpawnerObj
    /* SpawnEntity: calls the spawn function of the record's type with the
     * record's three other halfwords. */
    void Spawn(u32 id, const struct level_entity *rec);
    void SetTable(const entity_spawn_fn *table, s32 n); // SetEntitySpawnerTable
    /* An effect part (a MovingSprite with an EffectCtrl) at (x, y),
     * clamped into the level, animation `anim` of the sprite banks. */
    MovingSprite *SpawnEffectPart(s32 anim, s32 tag, s32 x, s32 y, s32 mirror);
    /* An effect part beside `src`, `margin` pixels clear of it, moving
     * away at `speed`. */
    MovingSprite *LaunchEffectPart(s32 anim, s32 tag, s32 margin, s32 z, s32 speed,
                                   MovingSprite *src);
    /* A dropped wumpa and extra life (drop_extra_life.cpp); `toHud`
     * flies it to the HUD. */
    Wumpa *DropWumpa(u32 x, u32 y, u32 p3, u32 p4, bool toHud);
    ExtraLife *DropExtraLife(u32 x, u32 y, u32 p3, u32 p5, bool toHud);
};

COMPILE_TIME_ASSERT(spawners_hpp, sizeof(EntitySpawner) == sizeof(struct entity_spawner));

#endif /* !GUARD_SPAWNERS_HPP */
