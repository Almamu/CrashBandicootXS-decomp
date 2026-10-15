#ifndef GUARD_ACTOR_CATEGORY_STATS_HPP
#define GUARD_ACTOR_CATEGORY_STATS_HPP

/* The actor category's run statistics (src/engine/actor/actor_category_stats.cpp,
 * #915): the deaths since the checkpoint (the mask assist, the spawns'
 * bonus kinds and the boss's retry level count them), the nitro crates
 * the player has missed (the room's pending switch crates) and the
 * checkpoint's course distance. ActorCategory::Init resets them when the
 * category starts and rewinds the missed nitros to the checkpoint's count
 * on every retry.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The actor category statistics as an all-static class"): its variables
 * are separate IWRAM globals that only the category code reads, and
 * static data members compile like those globals, static member
 * functions like plain functions. cxx_symbols.txt (block "#915
 * ActorCategoryStats") maps each member to its C name
 * (`_Q36Engine5Actor18ActorCategoryStats.deaths` is gActorCategoryDeaths, ...):
 * games/xs/sym_iwram.txt places the counters, iwram_data.cpp defines `checkpoint`
 * with the other initialised IWRAM globals. The functions keep their C
 * names the same way (`AddMissedNitro__Q36Engine5Actor18ActorCategoryStats` is
 * AddActorMissedNitro).
 *
 * In the namespace Engine::Actor (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

namespace Engine
{
namespace Actor
{

class ActorCategoryStats
{
public:
    static void AddMissedNitro();     // AddActorMissedNitro: the polar nitro crates
    static s32 GetMissedNitros();     // GetActorMissedNitros: the room's pending switch crates
    static s32 GetCheckpoint();       // GetActorCheckpoint
    static void SetCheckpoint(s32 z); // SetActorCheckpoint: the checkpoint crates
    static bool IsMaskAssistDue();    // IsActorMaskAssistDue

private:
    /* ActorCategory::Init resets the counters, rewinds `missedNitros` and
     * counts the deaths */
    friend class ActorCategory;

    /* IWRAM (games/xs/sym_iwram.txt) */
    static s32 deaths; // gActorCategoryDeaths - since the checkpoint
    /* gActorCategoryBossDeaths - the deaths since the checkpoint that count
     * towards the category's `retryBossDeaths`: being carried off by the
     * yeti (exit status 2) in a type-0 category, any death in the others */
    static s32 bossDeaths;
    static s32 missedNitros;           // gActorMissedNitros
    static s32 checkpointMissedNitros; // gActorCheckpointMissedNitros - at the checkpoint
    static s32 checkpoint;             // gActorCheckpoint (iwram_data.cpp) - the course distance
};

} // namespace Actor
} // namespace Engine

#endif // GUARD_ACTOR_CATEGORY_STATS_HPP
