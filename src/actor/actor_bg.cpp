extern "C" {
#include "core.h"
#include "math_util.h"
#include "gba/io_reg.h"
#include <libgcc.h>
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
}

/* The actor zone's BG0/BG1 scroll (InitActorBgScroll/UpdateActorBgScroll,
 * at the end of cell_anim.cpp until #770) and BG2 boss layer (the
 * shake, the layer depth, the scroll commit and its centre). */

/* Selects one of two fixed BG0/BG1 scroll-effect parameter sets
 * (arg0 == 0 vs nonzero), then derives the shared initial scroll
 * position/register state from them. */
void InitActorBgScroll(s32 arg0)
{
    s32 v;

    gActorBgScrollType = arg0;

    if (arg0 == CATEGORY_TYPE_POLAR) {
        gActorNearClipDepth = 0x88 << 5;
        gActorFarClipDepth = 0xa0 << 8;
        gActorFocalLength = 0xbc << 6;
        gActorBgWidth = 0x98 << 9;
        gActorBgHeight = 0xd0 << 8;
        gActorBgScrollEaseShift = 2;
        gActorBgScrollRangeX = 0x64;
        gActorBgScrollRangeY = 0x51;
        gActorBg0VOffset = arg0;
    } else {
        gActorNearClipDepth = 0xd0 << 5;
        gActorFarClipDepth = 0xaa << 8;
        gActorFocalLength = 0xe0 << 5;
        gActorBgWidth = 0x98 << 9;
        gActorBgHeight = 0xce << 8;
        gActorBgScrollEaseShift = 3;
        gActorBgScrollRangeX = 3 + 0xfd;
        gActorBgScrollRangeY = 0x96;
        gActorBg0VOffset = 2;
    }

    /* `v` stays in `r1` from the moment it's first computed
     * (as `gActorBgScrollMaxX`'s new value) through the sign-rounded
     * `/2`/`>>9`/`>>9` triple below, all reusing that same register in
     * place rather than reloading `gActorBgScrollMaxX` fresh - matching
     * the ROM's own single, unbroken chain of `r1` uses (see
     * docs/workflow.md step 3). `REG_BG0VOFS` is just
     * `gActorBg0VOffset` alone here, NOT
     * `gActorBg0VOffset + (v >> 9)` - there's no addition in the ROM's
     * own instructions for this store. */
    {
        s32 *ecPtr = &gActorBgScrollMaxX;

        v = gActorBgWidth + (s32)0xFFFF1000;
        *ecPtr = v;
    }
    gActorBgScrollMaxY = gActorBgHeight + (s32)0xFFFF6000;

    {
        s32 *d0Ptr = &gActorBgScrollX;

        v = v + (s32)((u32)v >> 31);
        *d0Ptr = v >> 1;
    }
    gActorBgScrollY = 0;

    {
        vu16 *bg0hofsPtr = &REG_BG0HOFS;

        v >>= 9;
        *bg0hofsPtr = v;
    }
    REG_BG0VOFS = gActorBg0VOffset;
    REG_BG1HOFS = v;
    REG_BG1VOFS = 0;

    gActorBgShake = 0;
    gActorBgLayerDepth = gActorFarClipDepth;
}

/* Eases the BG0/BG1 scroll accumulators (gActorBgScrollX/gActorBgScrollY)
 * toward their per-axis target/scale-derived offsets
 * (gActorBgScrollMaxX/gActorBgScrollMaxY), clamping each to
 * [0, target]. arg0/arg1 are the two axes' own driving values. Each
 * axis has its own target and delta locals (shared ones put the clamps in
 * other registers). */
void UpdateActorBgScroll(s32 arg0, s32 arg1)
{
    s32 targetX, targetY;
    s32 deltaX, deltaY;
    s32 cur;
    s32 shift;

    targetX = gActorBgScrollMaxX;
    deltaX = __divsi3(arg0 * Q8_TO_INT(targetX), gActorBgScrollRangeX);
    deltaX += targetX / 2;
    cur = gActorBgScrollX;
    deltaX -= cur;
    shift = gActorBgScrollEaseShift;
    deltaX >>= shift;
    cur += deltaX;
    gActorBgScrollX = cur;
    LIMIT_MIN(cur, 0);
    cur = MIN(cur, targetX);
    gActorBgScrollX = cur;

    targetY = gActorBgScrollMaxY;
    deltaY = __divsi3(arg1 * Q8_TO_INT(targetY), gActorBgScrollRangeY);
    deltaY += targetY / 2;
    cur = gActorBgScrollY;
    deltaY -= cur;
    deltaY >>= shift;
    deltaY -= gActorBgShake;
    cur += deltaY;
    gActorBgScrollY = cur;
    LIMIT_MIN(cur, 0);
    cur = MIN(cur, targetY);
    gActorBgScrollY = cur;
}

void ShakeActorBg(s32 arg0)
{
    gActorBgShake = arg0;
}

/* gActorBgLayerDepth's setter and getter: the depth past which actors
 * draw behind the BG2 boss layer. The yeti, airship and hovercraft set it
 * to their own distance; UpdateActor/UpdateActorDepth compare each actor's
 * depth with it. */
void SetActorBgLayerDepth(s32 arg0)
{
    gActorBgLayerDepth = arg0;
}

s32 GetActorBgLayerDepth(void)
{
    return gActorBgLayerDepth;
}

/* Empty hook InitActorCategory calls once the category is over (after its
 * retry loop), before freeing the sprite caches. See
 * ActorCategoryAttemptEndStub (cell_anim.cpp). */
void ActorCategoryEndStub(void)
{
}

/* Commits the BG0/BG1 scroll accumulators to the actual hardware
 * scroll registers, then clears the per-axis bias (gActorBgShake)
 * for the next frame. `x`/`yShift` are locals reused verbatim for the
 * BG1 writes (matching the ROM's own register reuse),
 * and `dest`/`vofsDest` are materialized as explicit pointer locals
 * ahead of each store so this compiler loads the destination register's
 * address before the source value it's about to write - the ROM's own
 * instruction order - rather than the reverse order a plain
 * `REG_BG0HOFS = gActorBgScrollX >> 8;`/`REG_BG0VOFS = ...` compiles
 * to (see docs/workflow.md step 3). */
void CommitActorBgScroll(void)
{
    s32 x;
    s32 yShift;
    vu16 *dest = &REG_BG0HOFS;
    vu16 *vofsDest;

    x = Q8_TO_INT(gActorBgScrollX);
    *dest = x;
    vofsDest = &REG_BG0VOFS;
    yShift = Q8_TO_INT(gActorBgScrollY);
    *vofsDest = gActorBg0VOffset + yShift;
    REG_BG1HOFS = x;
    REG_BG1VOFS = yShift;

    gActorBgShake = 0;
}

/* A pair of "target minus current, halved toward zero" getters for this
 * BG2 affine scroll/zoom effect subsystem - gActorBgHeight/F8 are
 * target extents, gActorBgScrollY/D0 the current position (both
 * written by InitActorBgScroll/UpdateActorBgScroll, still raw). The `/2` is written
 * as the ROM's own round-toward-zero shift idiom
 * (`(x + (x>>31)) >> 1`, the ">>31" op arithmetic-shifting in the
 * value's sign bit as a 0/1 rounding nudge) rather than plain
 * division, matching this compiler's own signed-divide-by-2 codegen
 * either way - written explicitly since the standalone idiom is the
 * form seen used throughout this file's cluster. */

s32 GetActorBgCenterY(void)
{
    return (gActorBgHeight / 2) - gActorBgScrollY;
}

s32 GetActorBgCenterX(void)
{
    return (gActorBgWidth / 2) - gActorBgScrollX;
}
