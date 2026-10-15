#include "game/entities/actor_self.hpp"
#include "engine/sys/vram_dma_queue.hpp"
#include "game/vehicles/vehicle.hpp"
#include "engine/actor/actor_palette_cycle.hpp"
#include "engine/actor/actor_category.hpp"
#include "category_palette_cycles.h"

extern "C" {
#include "engine/util/math_util.h"
#include "match.h"
#include <libgcc.h>
#include "engine/actor/actor.h"
#include "engine/gfx/gfx.h"
#include "globals.h"
}

using Engine::Actor::ActorPaletteCycle;
using Engine::Sys::VramDmaQueue;

/* The actor zone's BG palette cycle (gActorPaletteCycle*): restore,
 * save, the per-frame DMA step, the per-category seed and the switch,
 * ROM 0x0802AB08-0x0802AC28.
 * Built with old_agbcp and -fno-implement-inlines, like actor_self.cpp (actor.cpp then), where
 * it was until #770. See docs/matching/archive/issue-50-actor-2a69c.md. */

/* Loads the palette cycle's cursor and bound (`frame`/`target`, see
 * Update) from their saved
 * copies and restarts the DMA timer. */
void ActorPaletteCycle::Restore()
{
    frame = savedFrame;
    target = savedTarget;
    timer = 0;
}

/* The inverse of Restore. */
void ActorPaletteCycle::Save()
{
    savedFrame = frame;
    savedTarget = target;
}

/* While enabled: DMAs one 0x1c0-byte frame of `frames` (the cursor's) to
 * BG palette RAM, and every 0x24 calls moves the cursor one step toward
 * the bound (holding it there); Save/Set swap the ends for a ping-pong.
 * The frames are ACTOR_PALETTE_CYCLE_FRAMES (category_palette_cycles.h):
 * `frames`, or the current category's own when a category names one
 * (levels.json's "palette_cycle"). */
void ActorPaletteCycle::Update()
{
    if (!enabled)
        return;
    VramDmaQueue::Add(ACTOR_PALETTE_CYCLE_FRAMES[frame], BG_PALETTE(0), 0x1c0, 0x10);
    if (++timer > 0x23) {
        timer = 0;

        s32 *cur = &frame;
        s32 bound = target;
        s32 v = *cur;
        s32 r;

        if (bound - v >= 0) {
            r = v;
            if (bound != r)
                r++;
        } else {
            r = v - 1;
        }
        *cur = r;
    }
}

/* Seeds the palette cycle's cursor and bound from the per-category tables
 * (startFrames/targetFrames) and
 * restarts the DMA timer. */
void ActorPaletteCycle::Set(s32 idx)
{
    frame = startFrames[idx];
    target = targetFrames[idx];
    timer = 0;
}

/* Turns the palette cycle on or off, resetting the cursor, the bound and
 * the timer, and saves that state (Save). */
void ActorPaletteCycle::Enable(u8 flag)
{
    enabled = flag;
    frame = 0;
    target = 0;
    timer = 0;
    Save();
}
