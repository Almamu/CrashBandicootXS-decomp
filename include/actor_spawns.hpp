#ifndef GUARD_ACTOR_SPAWNS_HPP
#define GUARD_ACTOR_SPAWNS_HPP

/* The actor category's spawn table (src/actor/actor_spawn.cpp, #915): the
 * category's spawn records (struct actor_spawn_table, actor.h), ordered
 * by depth, and how far the category has spawned them. SelectActorCategory
 * sets the table up and spawns what is in view; RunActorCategoryFrame
 * spawns the records that scroll into view, or, while the spawns are
 * paused (the airship's and the hovercraft's fights), moves `offset` on
 * with the course instead. The accessors are what the homing actors
 * (AimJetpackPlane, AimPolarPenguin) use to fly from spawn to spawn.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The actor spawn table as an all-static class"): its variables are
 * separate IWRAM globals that only the category code reads, and static
 * data members compile like those globals, static member functions like
 * plain functions. cxx_symbols.txt (block "#915 ActorSpawns") maps each
 * member to its C name (`_11ActorSpawns.table` is gActorSpawnTable, ...),
 * which sym_iwram.txt places. The functions keep their C names the same
 * way (`GetX__11ActorSpawnsi` is GetActorSpawnX).
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
#include "actor.h" /* the friends below have C linkage */
}

class ActorSpawns
{
public:
    static s32 GetOffset();            // GetActorSpawnOffset: Q8
    static void Resume();              // ResumeActorSpawns
    static void Pause();               // PauseActorSpawns
    static s32 GetNextTarget(s32 idx); // GetActorSpawnNextTarget: -1 at the chain's end
    static s32 GetZ(s32 idx);          // GetActorSpawnZ: Q8, offset by `offset`
    static s32 GetY(s32 idx);          // GetActorSpawnY: Q8
    static s32 GetX(s32 idx);          // GetActorSpawnX: Q8
    static s32 GetKindIndex(s32 idx);  // GetActorSpawnKindIndex: the kind less 0x20

private:
    /* SelectActorCategory sets the table up, RunActorCategoryFrame walks it */
    friend void SelectActorCategory(s32 type, struct actor_spawn_table *table,
                                    struct anim_table_record *animTable, u8 active, s32 variant,
                                    s32 checkpoint);
    friend s32 RunActorCategoryFrame(void);

    /* IWRAM (sym_iwram.txt) */
    static struct actor_spawn_table *table; // gActorSpawnTable - the category's spawnTable
    static s32 index;                       // gActorSpawnIndex - the next record to spawn
    /* gActorSpawnUseBonus - SelectActorCategory's `active` argument (the
     * deaths reached the category's `bonusKindDeaths`): spawns use their
     * record's `bonusKind` */
    static u8 useBonus;
    static u8 paused; // gActorSpawnsPaused
    /* gActorSpawnOffset - the course distance travelled while paused,
     * added to the records' depths (whole units) */
    static s32 offset;
};

#endif // GUARD_ACTOR_SPAWNS_HPP
