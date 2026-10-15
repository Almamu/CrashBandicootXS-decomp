#ifndef GUARD_COLLECTED_SPAWNS_HPP
#define GUARD_COLLECTED_SPAWNS_HPP

/* The collected spawns (engine/src/engine/actor/actor_spawn_collected.cpp, #915): the
 * spawn records of the 3D stages' crates the player has broken, at most
 * 15. The factories skip them when the category restarts from a
 * checkpoint; ActorCategory::Init clears the list.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The collected spawns as an all-static class"): its variables are
 * separate IWRAM globals that no other object reads, and static data
 * members compile like those globals, static member functions like plain
 * functions. cxx_symbols.txt (block "#915 CollectedSpawns") maps each
 * member to its C name (`_Q36Engine5Actor15CollectedSpawns.spawns` is gCollectedSpawns,
 * ...): games/xs/sym_iwram.txt places `spawns`, iwram_data.cpp defines `count` with
 * the other initialised IWRAM globals. The functions keep their C names
 * the same way (`Clear__Q36Engine5Actor15CollectedSpawns` is ClearCollectedSpawns).
 *
 * In the namespace Engine::Actor (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

struct actor_spawn;

namespace Engine
{
namespace Actor
{

class CollectedSpawns
{
public:
    static bool Contains(struct actor_spawn *spawn); // IsSpawnCollected
    static void Add(struct actor_spawn *spawn);      // MarkSpawnCollected: unless full or there
    static void Clear();                             // ClearCollectedSpawns

private:
    static struct actor_spawn *spawns[]; // gCollectedSpawns - 15 at most
    static s32 count;                    // gCollectedSpawnCount (iwram_data.cpp)
};

} // namespace Actor
} // namespace Engine

#endif // GUARD_COLLECTED_SPAWNS_HPP
