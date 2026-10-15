#ifndef GUARD_ACTOR_PALETTE_CYCLE_HPP
#define GUARD_ACTOR_PALETTE_CYCLE_HPP

/* The 3D stages' BG palette cycle (engine/src/engine/actor/actor_palette_cycle.cpp,
 * #915): while it is enabled, every cell animation frame DMAs one frame
 * of `frames` to BG palette RAM, and every 0x24 frames moves the cursor
 * one frame toward its bound.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The actor palette cycle as an all-static class"): its variables are
 * separate IWRAM globals (0x03001464-0x0300147B) that no other object
 * reads, and static data members compile like those globals, static
 * member functions like plain functions. The data members are only
 * declared: cxx_symbols.txt (block "#915 ActorPaletteCycle") maps each
 * one to its C name (`_Q36Engine5Actor17ActorPaletteCycle.frame` is
 * gActorPaletteCycleFrame, ...), whose address is in games/xs/sym_iwram.txt; the
 * ROM tables are defined in engine/src/engine/actor/data/actor_palette_cycle_frames.cpp. The
 * functions keep their C names the same way (`Update__Q36Engine5Actor17ActorPaletteCycle`
 * is UpdateActorPaletteCycle).
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

class ActorPaletteCycle
{
public:
    typedef u16 Frame[14 * 16]; // a frame of the cycle: BG palettes 0-13

    static void Restore();       // RestoreActorPaletteCycle: the saved cursor and bound
    static void Save();          // SaveActorPaletteCycle
    static void Update();        // UpdateActorPaletteCycle: CellAnim::Advance's
    static void Set(s32 idx);    // SetActorPaletteCycle: startFrames/targetFrames[idx]
    static void Enable(u8 flag); // EnableActorPaletteCycle

private:
    /* IWRAM (games/xs/sym_iwram.txt) */
    static u8 enabled;      // gActorPaletteCycleEnabled
    static s32 savedFrame;  // gSavedActorPaletteCycleFrame
    static s32 savedTarget; // gSavedActorPaletteCycleTarget
    static s32 frame;       // gActorPaletteCycleFrame - the cursor, indexes frames
    static s32 target;      // gActorPaletteCycleTarget - the frame the cursor stops at
    static s32 timer;       // gActorPaletteCycleTimer - the calls since the cursor moved

    /* ROM (engine/src/engine/actor/data/actor_palette_cycle_frames.cpp) */
    static const u16 frames[32][14 * 16]; // gActorPaletteCycleFrames - BG palettes 0-13
    static const s32 startFrames[4];      // gActorPaletteCycleStartFrames
    static const s32 targetFrames[4];     // gActorPaletteCycleTargetFrames
    /* Per category, its frames, when a category names its own
     * "palette_cycle" (defined by actor_categories.inc then only;
     * category_palette_cycles.h) */
    static const Frame *const categoryFrames[];
};

} // namespace Actor
} // namespace Engine

#endif // GUARD_ACTOR_PALETTE_CYCLE_HPP
