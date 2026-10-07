#include "core.h"
#include "math_util.h"
#include "match.h"
#include "memory.h"
#include "actor.h"
#include "vehicle.h"

/* More of the `gYeti`-rooted object's lifecycle (see
 * polar_aku_aku.c's header comment): a state-flag setter, its
 * destructor, and its constructor. */

/* Arms `gYetiState = 3` - a state value none of this chunk's
 * other functions read back, plausibly consumed by the vtable-dispatch
 * caller itself. */
void StopYeti(void)
{
    gYetiState = 3;
}

/* Destructor: frees the object. */
void DestroyYeti(void)
{
    mem_free(gYeti);
}

/* Constructor: stashes the caller's argument in `gYetiParamsIndex`,
 * allocates and wires up a fresh instance (part table
 * `gYetiKeyframes`/`0817A880`, header byte `0xf`, reset via
 * `SetActorAnim`) into `gYeti`, resets the position-tracking
 * pair (`gYetiX` to 0, `gYetiDistance` to `0xA000`,
 * `gYetiPosition` derived the same way `YetiStateChase`/`YetiStateCharge` do),
 * primes `SetActorBgLayerDepth`, clears `gYetiState`, and finally calls
 * `LoadYetiGraphics` (the object's own initial VRAM-pattern/DMA setup,
 * parked separately - see docs/matching/archive/issue-54-actor-d3a8.md). */
void CreateYeti(void *arg0)
{
    struct actor_self *obj;
    MATCH_HOLD_REG(u32, size, r0);
    MATCH_HOLD_REG(s32, flags, r1);
    MATCH_HOLD_REG(void **, bcAddr, r5);

    gYetiParamsIndex = (s32)arg0;
    bcAddr = (void **)&gYeti;
    asm volatile("mov %0, #0x1c" : "=r"(size));
    asm volatile("mov %0, #0x80\n\tlsl %0, %0, #0x18" : "=r"(flags));
    obj = mem_alloc(size, flags);
    {
        struct anim_frame_record *v0 = (struct anim_frame_record *)gYetiKeyframes;
        u32 *v1 = (u32 *)gYetiFrames;
        s32 v2 = 0xf;

        obj->anims = v0;
        obj->frameOffsets = v1;
        obj->palette = v2;
    }
    SetActorAnim(obj, 0);
    *bcAddr = obj;

    gYetiX = 0;
    gYetiDistance = 0xa000;
    gYetiPosition = INT_TO_Q8(GetCellAnimDistance()) - gYetiDistance;
    SetActorBgLayerDepth(gYetiDistance);

    gYetiState = 0;
    LoadYetiGraphics();
}

/* Sits right after `CreateYeti` above and before
 * `YetiStateCaught` below - the whole contiguous range that used
 * to be `asm/code_3_2_20_28568_c99c_e058.s`. */

/* Builds one page of the yeti's BG2 map: the 16x16 one-byte entries of
 * an affine screen, with a 10x10 block of consecutive tile numbers
 * (from `seed`) in the middle and tile 0xFF (the blank tile
 * LoadYetiGraphics clears at VRAM+0xBFC0) around it.
 *
 * A parameterized twin of `LoadYetiGraphics`'s (yeti_graphics.c) 16x16
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
void BuildYetiBg2Map(u8 *dst, u8 seed)
{
    s32 y, x;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if ((u32)(x - 3) > 9 || y <= 2 || y > 12)
                dst[y * 16 + x] = 0xff;
            else
                dst[y * 16 + x] = seed++;
        }
    }
}

/* Genuine no-op stub sitting between `BuildYetiBg2Map` (the BG2 map
 * builder) and `YetiStateStop` - see
 * docs/matching/archive/issue-54-actor-d3a8.md. */
void YetiStateCaught(void)
{
}
