#include "actor_self.hpp"
#include "yeti.hpp"
#include "vehicle.hpp"
#include "cell_anim.hpp"
#include "actor_bg.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "actor.h"
}

using Engine::Gfx::CellAnim;

/* More of the `gYeti`-rooted object's lifecycle (see
 * polar_aku_aku.cpp's header comment): a state-flag setter, its
 * destructor, and its constructor. */

/* The run is over (PolarPlayer::FinishRun, or Hurt/Shock with no mask
 * left): the yeti stops following
 * (Update) and stops catching, and StateStop plays its last animation. */
void Yeti::Stop()
{
    state = YETI_STATE_STOP;
}

/* Destructor: frees the object. */
void Yeti::Destroy()
{
    delete anim;
}

/* Constructor: stashes the caller's argument in `gYetiParamsIndex`,
 * allocates and wires up a fresh instance (part table
 * `gYetiKeyframes`/`0817A880`, header byte `0xf`, reset via
 * `SetActorAnim`) into `gYeti`, resets the position-tracking
 * pair (`gYetiX` to 0, `gYetiDistance` to `0xA000`,
 * `gYetiPosition` derived the same way `YetiStateChase`/`YetiStateCharge` do),
 * primes `ActorBg::SetLayerDepth`, clears `gYetiState`, and finally calls
 * `LoadYetiGraphics` (the object's own initial VRAM-pattern/DMA setup,
 * parked separately - see docs/matching/archive/issue-54-actor-d3a8.md). */
void Yeti::Create(s32 level)
{
    paramsIndex = level;
    anim = new AnimPart(keyframes, (u32 *)frames, 0xf);

    x = 0;
    distance = INT_TO_Q8(160);
    position = INT_TO_Q8(CellAnim::GetDistance()) - distance;
    ActorBg::SetLayerDepth(distance);

    state = YETI_STATE_CHASE;
    LoadGraphics();
}

/* Sits right after `CreateYeti` above and before
 * `YetiStateCaught` below - the whole contiguous range that used
 * to be `asm/code_3_2_20_28568_c99c_e058.s`. */

/* Builds one page of the yeti's BG2 map: the 16x16 one-byte entries of
 * an affine screen, with a 10x10 block of consecutive tile numbers
 * (from `seed`) in the middle and tile 0xFF (the blank tile
 * LoadYetiGraphics clears at VRAM+0xBFC0) around it.
 *
 * A parameterized twin of `LoadYetiGraphics`'s (yeti_graphics.cpp) 16x16
 * map-fill loop, taking the destination buffer
 * (`dst`) and seed byte (`seed`) as real parameters instead of the
 * fixed stack buffer/`0`-or-`0x80` seed constants `LoadYetiGraphics` uses for
 * its own two inline copies of this same loop. No known caller anywhere
 * in the matched portion of this ROM region (`LoadYetiGraphics` always
 * inlines the loop itself rather than calling this) - kept byte-exact
 * regardless, per this project's standing convention for functions
 * without a confirmed call site. UNUSED.
 *
 * The condition is written as the "fill with 0xff" test so the 0xff
 * store comes first, as in the ROM. */
void Yeti::BuildBg2Map(u8 *dst, u8 seed)
{
    s32 y, x;

    for (y = 0; y < YETI_BG2_MAP_TILES; y++) {
        for (x = 0; x < YETI_BG2_MAP_TILES; x++) {
            if ((u32)(x - YETI_PICTURE_TILE_FIRST) >
                    YETI_PICTURE_TILE_LAST - YETI_PICTURE_TILE_FIRST ||
                y <= YETI_PICTURE_TILE_FIRST - 1 || y > YETI_PICTURE_TILE_LAST)
                dst[y * YETI_BG2_MAP_TILES + x] = 0xff;
            else
                dst[y * YETI_BG2_MAP_TILES + x] = seed++;
        }
    }
}

/* Genuine no-op stub sitting between `BuildYetiBg2Map` (the BG2 map
 * builder) and `YetiStateStop` - see
 * docs/matching/archive/issue-54-actor-d3a8.md. */
void Yeti::StateCaught()
{
}
