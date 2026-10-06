#include "core.h"
#include "match.h"
#include "actor.h"
#include "objects.h"
#include "level.h"

/* Probes the terrain along the edge of `self`'s hitbox `quad` that faces
 * direction `mode` (ProbeTerrain's 1 right, 2 left, 4 up, 8 down): moves
 * a copy of `self`'s Q8 position to the start of that edge
 * (`OffsetToHitboxEdgeStart`), converts it to pixels, resets
 * `self->probeTries` and runs `ProbeTerrain` over the edge's length
 * (`quad->w`), with `self`'s Q8 `y` as the value it snaps.
 *
 * If that first probe hits, `self->y` takes the snapped value and it
 * returns 1. Otherwise it clears `gLevelLayers->probeFlag` (saving it)
 * and retries up to 3 more times, 8 pixels lower each time, counting the
 * attempts in `self->probeTries`; whether a retry hits or all of them
 * miss it restores `probeFlag` and returns 0 - only a hit on the first,
 * un-nudged probe counts. CollideGroundSprite runs it with 8 to confirm
 * the floor its cheaper probes found.
 *
 * Real C (issue #9-#11 NAKED retry; matches under both compilers).
 * The "keeps `self+0x69`'s address in r6 for the whole retry loop and
 * reuses it for the termination test" shape is a second pointer: the
 * loop increments through a `t2 = tries` copy and tests through
 * `tries` (pinned to r6, which otherwise swaps with the saved flag
 * byte), and bumps `pos.y` through a `&pos` pointer. The first probe's
 * `u8` result is kept in a local (its 0 is what the ROM stores into
 * `+0x2a`), position and `origY` are one frame struct, and a hit inside
 * the loop restores the flag and returns on its own - that copy reloads
 * `gLevelLayers` from the literal pool, the loop-exit copy uses
 * the cached address, and cross-jumping shares their `strb`. */
#include "box_part.h"
#include "globals.h"

s32 ProbeHitboxEdgeTerrain(struct box_part *self, s32 mode, struct hitbox_quad *quad)
{
    struct {
        s32 x;
        s32 y;
        s32 origY;
    } f;
    u8 span = quad->w;
    MATCH_HOLD_REG(u8 *, tries, r6);
    u8 hit;

    f.origY = self->y;
    *(struct probe_pos *)&f = *(struct probe_pos *)self;
    OffsetToHitboxEdgeStart(&f, mode, quad);
    f.x >>= 8;
    f.y >>= 8;
    tries = &self->probeTries;
    *tries = 0;
    hit = (u8)ProbeTerrain(gLevelLayers, mode, (struct probe_pos *)&f, span, &f.origY);
    if (hit) {
        self->y = f.origY;
        return 1;
    }
    {
        u8 saved = gLevelLayers->probeFlag;
        u8 *t2;
        struct probe_pos *pp;

        gLevelLayers->probeFlag = 0;
        t2 = tries;
        pp = (struct probe_pos *)&f;
        do {
            (*t2)++;
            pp->y += 8;
            if ((u8)ProbeTerrain(gLevelLayers, mode, (struct probe_pos *)&f, span, &f.origY)) {
                gLevelLayers->probeFlag = saved;
                return 0;
            }
        } while (*tries <= 2);
        gLevelLayers->probeFlag = saved;
    }
    return 0;
}
