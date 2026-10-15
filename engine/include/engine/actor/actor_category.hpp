#ifndef GUARD_ACTOR_CATEGORY_HPP
#define GUARD_ACTOR_CATEGORY_HPP

/* The actor category driver (engine/src/engine/actor/, #915): a room's category stage,
 * the 3D levels (the polar levels, the jetpack levels, the airship and
 * the hovercraft). Init runs the category from start to end: it builds
 * the VRAM pool, selects the category (its vtable, its spawn table, the
 * player and the boss), runs a frame per VBlank (every actor's Update and
 * the visible ones' Draw in draw order), and on a death retries from the
 * checkpoint. The category's vtable (struct category_vtable, actor_anim.h)
 * is a struct of plain function pointers for the player and the boss,
 * which the slot dispatchers call.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The actor category driver as an all-static class"): its variables are
 * separate IWRAM globals that only the category code reads, and static
 * data members compile like those globals, static member functions like
 * plain functions. Its statistics (ActorCategoryStats) and its spawn
 * table (ActorSpawns) are classes of their own. cxx_symbols.txt (block
 * "#915 ActorCategory") maps each member to its C name
 * (`_Q36Engine5Actor13ActorCategory.current` is gActorCategory, ...): games/xs/sym_iwram.txt
 * places the IWRAM ones, engine/src/engine/actor/data/actor_categories.cpp defines the
 * ROM tables. The functions keep their C names the same way
 * (`Init__Q36Engine5Actor13ActorCategoryi` is InitActorCategory).
 *
 * In the namespace Engine::Actor (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
#include "engine/actor/actor_anim.h"
}

namespace Game
{
namespace Entities
{
class ActorSelf; // actor_self.hpp
} // namespace Entities
} // namespace Game

namespace Engine
{
namespace Actor
{

class ActorCategory
{
public:
    static s32 Init(s32 category);         // InitActorCategory: a ROOM_EXIT_* status
    static s32 CountCrates(s32 category);  // CountCategoryCrates: the level queries
    static s32 GetFrameCount();            // GetActorCategoryFrameCount
    static void SetExitStatus(s32 status); // SetActorCategoryExitStatus: CATEGORY_EXIT_*
    /* IsTouchingPlayer: the vtable's slot 9 on `self`. A yes/no that stays
     * s32: every caller's `(u8)` is the ROM's truncation, but as a bool,
     * PolarPlayer::IsTouchingPlayer and JetpackPlayer::IsTouchingPlayer
     * (the hooks it calls) get a second return path (#855). */
    static s32 IsTouchingPlayer(Game::Entities::ActorSelf *self);

private:
    friend class ActorCategoryStats; // IsMaskAssistDue reads the selected descriptor
    friend class ActorPaletteCycle;  // Update reads `current` when a category has its own cycle

    static void SetupVramPool(); // SetupActorVramPool
    /* SelectActorCategory: the vtable, the spawn table, the player, what
     * is in view and the boss */
    static void Select(s32 type, struct actor_spawn_table *table,
                       struct anim_table_record *animTable, u8 active, s32 variant, s32 checkpoint);
    static s32 RunFrame();             // RunActorCategoryFrame: a CATEGORY_EXIT_* status
    static inline s32 OpenPauseMenu(); // Init's pause menu (actor_category_init.cpp)
    static bool CanPause();            // CanPauseActorCategory
    static void ReloadGraphics();      // ReloadActorCategoryGraphics
    static void DestroyActors();       // DestroyAllActors
    static void UpdateBg2();           // UpdateActorCategoryBg2

    /* IWRAM (games/xs/sym_iwram.txt) */
    static s32 current;                          // gActorCategory - the descriptors index
    static Game::Entities::ActorSelf **drawList; // gActorDrawList - 50 entries
    static s32 drawCount;                        // gActorDrawCount
    static s32 exitStatus;                       // gActorCategoryExitStatus - this frame's
    static const struct category_vtable *vtable; // gActorCategoryVtable - the type's
    static s32 frameCount;                       // gActorCategoryFrameCount - since Select

    /* ROM (engine/src/engine/actor/data/actor_categories.cpp) */
    static const struct category_descriptor descriptors[CATEGORY_COUNT]; // gActorCategories
    static const struct category_vtable vtables[CATEGORY_TYPE_COUNT];    // gActorCategoryVtables
};

} // namespace Actor
} // namespace Engine

#endif // GUARD_ACTOR_CATEGORY_HPP
