#include "core.h"
#include "match.h"
#include "gba/io_reg.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"

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
 * ActorCategoryAttemptEndStub (cell_anim.c). */
void ActorCategoryEndStub(void)
{
}

/* Commits the BG0/BG1 scroll accumulators to the actual hardware
 * scroll registers, then clears the per-axis bias (gActorBgShake)
 * for the next frame. `x`/`yShift` are register-pinned (both reused
 * verbatim for the BG1 writes, matching the ROM's own register reuse),
 * and `dest`/`vofsDest` are materialized as explicit pointer locals
 * ahead of each store so this compiler loads the destination register's
 * address before the source value it's about to write - the ROM's own
 * instruction order - rather than the reverse order a plain
 * `REG_BG0HOFS = gActorBgScrollX >> 8;`/`REG_BG0VOFS = ...` compiles
 * to (see docs/workflow.md step 3). */
void CommitActorBgScroll(void)
{
    MATCH_HOLD_REG(s32, x, r2);
    MATCH_HOLD_REG(s32, yShift, r1);
    vu16 *dest = &REG_BG0HOFS;
    vu16 *vofsDest;

    x = gActorBgScrollX >> 8;
    *dest = x;
    vofsDest = &REG_BG0VOFS;
    yShift = gActorBgScrollY >> 8;
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
