#ifndef GUARD_ACTOR_BG_HPP
#define GUARD_ACTOR_BG_HPP

/* The 3D stages' BG scroll (src/engine/actor/actor_bg.cpp, #915): the BG0/BG1
 * scroll that eases after the player, its shake, its centre, and the depth
 * of the BG2 boss layer that the actors draw behind.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The actor BG scroll as an all-static class"): its variables are
 * separate IWRAM globals (0x030013CC-0x030013FF) that no other object
 * reads, and static data members compile like those globals, static
 * member functions like plain functions. The data members are only
 * declared: cxx_symbols.txt (block "#915 ActorBg") maps each one to its C
 * name (`_Q36Engine5Actor7ActorBg.scrollX` is gActorBgScrollX, ...), whose address is in
 * sym_iwram.txt, and the functions keep their C names the same way
 * (`Shake__Q36Engine5Actor7ActorBgi` is ShakeActorBg).
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

class ActorBg
{
public:
    static void InitScroll(s32 type);       // InitActorBgScroll: the category type's parameters
    static void UpdateScroll(s32 x, s32 y); // UpdateActorBgScroll: eases after the player
    static void Shake(s32 amount);          // ShakeActorBg
    static void SetLayerDepth(s32 depth);   // SetActorBgLayerDepth
    static s32 GetLayerDepth();             // GetActorBgLayerDepth
    static void CommitScroll();             // CommitActorBgScroll
    static s32 GetCenterY();                // GetActorBgCenterY
    static s32 GetCenterX();                // GetActorBgCenterX

private:
    /* IWRAM (sym_iwram.txt). The scroll and the sizes are Q8 pixels. */
    static s32 scrollY; // gActorBgScrollY
    static s32 scrollX; // gActorBgScrollX
    static s32 shake;   // gActorBgShake - a Y bias UpdateScroll subtracts, CommitScroll clears
    /* The depth of the BG2 boss layer (the yeti, airship or hovercraft):
     * actors deeper than it get OBJ priority 2 and draw behind it (bit 15
     * of their sortKey). InitScroll resets it to gActorFarClipDepth; the
     * bosses set it to their own distance. */
    static s32 layerDepth; // gActorBgLayerDepth
    static s32 scrollType; // gActorBgScrollType - InitScroll's category type
    static s32 easeShift;  // gActorBgScrollEaseShift - UpdateScroll eases by 1 / (1 << easeShift)
    /* UpdateScroll's input ranges: an input of +-range scrolls the BG to
     * either end of [0, maxX/maxY]. */
    static s32 rangeX;     // gActorBgScrollRangeX
    static s32 rangeY;     // gActorBgScrollRangeY
    static s32 maxX;       // gActorBgScrollMaxX - width less the screen's
    static s32 maxY;       // gActorBgScrollMaxY - height less the screen's
    static s32 bg0VOffset; // gActorBg0VOffset - BG0's extra vertical offset (pixels)
    static s32 width;      // gActorBgWidth
    static s32 height;     // gActorBgHeight
};

} // namespace Actor
} // namespace Engine

#endif // GUARD_ACTOR_BG_HPP
