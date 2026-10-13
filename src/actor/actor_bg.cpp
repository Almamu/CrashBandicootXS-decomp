#include "vehicle.hpp"
#include "actor_bg.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "gba/io_reg.h"
#include <libgcc.h>
#include "actor.h"
#include "bosses.h"
}

/* The actor zone's BG0/BG1 scroll (InitScroll/UpdateScroll,
 * at the end of cell_anim.cpp until #770) and BG2 boss layer (the
 * shake, the layer depth, the scroll commit and its centre). */

/* Selects one of two fixed BG0/BG1 scroll-effect parameter sets by the
 * category `type` (CATEGORY_TYPE_POLAR or not), then derives the shared initial scroll
 * position/register state from them. */
void ActorBg::InitScroll(s32 type)
{
    s32 v;

    scrollType = type;

    if (type == CATEGORY_TYPE_POLAR) {
        gActorNearClipDepth = INT_TO_Q8(17);
        gActorFarClipDepth = INT_TO_Q8(160);
        gActorFocalLength = ACTOR_FOCAL_LENGTH_POLAR;
        width = INT_TO_Q8(304);
        height = INT_TO_Q8(208);
        easeShift = 2;
        rangeX = 0x64;
        rangeY = 0x51;
        bg0VOffset = type;
    } else {
        gActorNearClipDepth = INT_TO_Q8(26);
        gActorFarClipDepth = INT_TO_Q8(170);
        gActorFocalLength = ACTOR_FOCAL_LENGTH;
        width = INT_TO_Q8(304);
        height = INT_TO_Q8(206);
        easeShift = 3;
        rangeX = 3 + 0xfd;
        rangeY = 0x96;
        bg0VOffset = 2;
    }

    /* `v` stays in `r1` from the moment it's first computed
     * (as `maxX`'s new value) through the sign-rounded
     * `/2`/`>>9`/`>>9` triple below, all reusing that same register in
     * place rather than reloading `maxX` fresh - matching
     * the ROM's own single, unbroken chain of `r1` uses (see
     * docs/workflow.md step 3). `REG_BG0VOFS` is just
     * `bg0VOffset` alone here, NOT
     * `bg0VOffset + (v >> 9)` - there's no addition in the ROM's
     * own instructions for this store. */
    {
        s32 *ecPtr = &maxX;

        v = width - INT_TO_Q8(DISPLAY_WIDTH);
        *ecPtr = v;
    }
    maxY = height - INT_TO_Q8(DISPLAY_HEIGHT);

    s32 *d0Ptr = &scrollX;

    v = v + (s32)((u32)v >> 31);
    *d0Ptr = v >> 1;
    scrollY = 0;

    vu16 *bg0hofsPtr = &REG_BG0HOFS;

    v >>= 9;
    *bg0hofsPtr = v;
    REG_BG0VOFS = bg0VOffset;
    REG_BG1HOFS = v;
    REG_BG1VOFS = 0;

    shake = 0;
    layerDepth = gActorFarClipDepth;
}

/* Eases the BG0/BG1 scroll accumulators (scrollX/scrollY)
 * toward their per-axis target/scale-derived offsets
 * (maxX/maxY), clamping each to
 * [0, target]. `x`/`y` are the two axes' own driving values (the
 * polar and jetpack players pass their position). Each
 * axis has its own target and delta locals (shared ones put the clamps in
 * other registers). */
void ActorBg::UpdateScroll(s32 x, s32 y)
{
    s32 targetX, targetY;
    s32 deltaX, deltaY;
    s32 cur;
    s32 shift;

    targetX = maxX;
    deltaX = __divsi3(x * Q8_TO_INT(targetX), rangeX);
    deltaX += targetX / 2;
    cur = scrollX;
    deltaX -= cur;
    shift = easeShift;
    deltaX >>= shift;
    cur += deltaX;
    scrollX = cur;
    LIMIT_MIN(cur, 0);
    cur = MIN(cur, targetX);
    scrollX = cur;

    targetY = maxY;
    deltaY = __divsi3(y * Q8_TO_INT(targetY), rangeY);
    deltaY += targetY / 2;
    cur = scrollY;
    deltaY -= cur;
    deltaY >>= shift;
    deltaY -= shake;
    cur += deltaY;
    scrollY = cur;
    LIMIT_MIN(cur, 0);
    cur = MIN(cur, targetY);
    scrollY = cur;
}

void ActorBg::Shake(s32 amount)
{
    shake = amount;
}

/* layerDepth's setter and getter: the depth past which actors
 * draw behind the BG2 boss layer. The yeti, airship and hovercraft set it
 * to their own distance; UpdateActor/UpdateActorDepth compare each actor's
 * depth with it. */
void ActorBg::SetLayerDepth(s32 depth)
{
    layerDepth = depth;
}

s32 ActorBg::GetLayerDepth()
{
    return layerDepth;
}

/* Empty hook ActorCategory::Init calls once the category is over (after its
 * retry loop), before freeing the sprite caches. See
 * ActorCategoryAttemptEndStub (cell_anim.cpp). */
void ActorCategoryEndStub(void)
{
}

/* Commits the BG0/BG1 scroll accumulators to the actual hardware
 * scroll registers, then clears the per-axis bias (shake)
 * for the next frame. `x`/`yShift` are locals reused verbatim for the
 * BG1 writes (matching the ROM's own register reuse),
 * and `dest`/`vofsDest` are materialized as explicit pointer locals
 * ahead of each store so this compiler loads the destination register's
 * address before the source value it's about to write - the ROM's own
 * instruction order - rather than the reverse order a plain
 * `REG_BG0HOFS = scrollX >> 8;`/`REG_BG0VOFS = ...` compiles
 * to (see docs/workflow.md step 3). */
void ActorBg::CommitScroll()
{
    s32 x;
    s32 yShift;
    vu16 *dest = &REG_BG0HOFS;
    vu16 *vofsDest;

    x = Q8_TO_INT(scrollX);
    *dest = x;
    vofsDest = &REG_BG0VOFS;
    yShift = Q8_TO_INT(scrollY);
    *vofsDest = bg0VOffset + yShift;
    REG_BG1HOFS = x;
    REG_BG1VOFS = yShift;

    shake = 0;
}

/* A pair of "target minus current, halved toward zero" getters for this
 * BG2 affine scroll/zoom effect subsystem - height/F8 are
 * target extents, scrollY/D0 the current position (both
 * written by InitScroll/UpdateScroll, still raw). The `/2` is written
 * as the ROM's own round-toward-zero shift idiom
 * (`(x + (x>>31)) >> 1`, the ">>31" op arithmetic-shifting in the
 * value's sign bit as a 0/1 rounding nudge) rather than plain
 * division, matching this compiler's own signed-divide-by-2 codegen
 * either way - written explicitly since the standalone idiom is the
 * form seen used throughout this file's cluster. */

s32 ActorBg::GetCenterY()
{
    return (height / 2) - scrollY;
}

s32 ActorBg::GetCenterX()
{
    return (width / 2) - scrollX;
}
