#ifndef GUARD_SPRITE_OAM_QUEUE_HPP
#define GUARD_SPRITE_OAM_QUEUE_HPP

/* The overflow OAM queue (engine/src/engine/gfx/sprite_frame.cpp, #915): the 3D stages
 * and the company logos queue their sprites' OAM entries and affine
 * scales here during a frame, and Flush appends them to the OAM shadow
 * buffer (OamBuffer, sprite_obj.hpp).
 *
 * An all-static class, like the OBJ tile allocator (docs/cplusplus.md,
 * "The OAM queue and the sprite frame cache as all-static classes"): its
 * variables are separate IWRAM globals (0x03001344-0x03001353) that no
 * other object reads, and static data members compile like those globals,
 * static member functions like plain functions. The data members are only
 * declared: cxx_symbols.txt (block "#915 SpriteOamQueue") maps each one to
 * its C name (`_Q36Engine3Gfx14SpriteOamQueue.entries` is gSpriteOamQueue, ...), whose
 * address is in games/xs/sym_iwram.txt, and the functions keep their C names the
 * same way (`Add__Q36Engine3Gfx14SpriteOamQueueUiUsi` is QueueSpriteFrameOam).
 *
 * In the namespace Engine::Gfx (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

struct queued_oam_entry;

namespace Engine
{
namespace Gfx
{

class SpriteOamQueue
{
public:
    /* QueueSpriteFrameOam: ATTR0 | ATTR1 << 16, ATTR2, and the affine
     * scale (Q8) when ATTR0's affine bit is set */
    static void Add(u32 attr01, u16 attr2, s32 priority);
    static void Destroy(); // FreeSpriteFrameOamQueue
    static void Flush();   // FlushSpriteFrameOamQueue
    static void Init();    // InitSpriteFrameOamQueue

private:
    /* IWRAM (games/xs/sym_iwram.txt). Both buffers are in EWRAM (Init). */
    static struct queued_oam_entry *entries; // gSpriteOamQueue - OAM_ENTRY_COUNT at most
    static s32 *affineScales;                // gSpriteAffineQueue - x | y << 16, deduplicated
    static s32 count;                        // gSpriteOamQueueCount
    static s32 affineCount;                  // gSpriteAffineQueueCount
};

} // namespace Gfx
} // namespace Engine

#endif // GUARD_SPRITE_OAM_QUEUE_HPP
