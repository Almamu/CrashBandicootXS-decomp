#include "bg_layer.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include "level.h"
}

/* MovingSprite's step probe (#664, include/sprite_obj.hpp). */

/* Probes the terrain along the edge of the sprite's hitbox `quad` that
 * faces `mode` (ProbeTerrain's 1 right, 2 left, 4 up, 8 down): from the
 * start of that edge (OffsetToHitboxEdgeStart), in pixels, over the edge's
 * width, snapping `y`. A hit there sets `y` and returns 1. Otherwise it
 * clears the layers' `probeFlag` and retries up to 3 more times, 8 pixels
 * lower each time, counting the tries in `probeTries`; a retry's hit
 * doesn't count, and either way `probeFlag` is restored and it returns
 * 0. CollideGroundSprite runs it with 8 to confirm the floor its cheaper
 * probes found. */
s32 MovingSprite::ProbeEdgeTerrain(s32 mode, const struct hitbox_quad *quad)
{
    struct {
        struct probe_pos pos;
        s32 origY;
    } f;
    u8 span = quad->w;
    /* Kept from the C: unpinned, `tries` and `saved` swap r6 and r7. */
    MATCH_HOLD_REG(u8 *, tries, r6);
    u8 hit;

    f.origY = y;
    f.pos = *(struct probe_pos *)&Pos();
    OffsetToHitboxEdgeStart(&f.pos, mode, (void *)quad);
    f.pos.x = Q8_TO_INT(f.pos.x);
    f.pos.y = Q8_TO_INT(f.pos.y);
    tries = &probeTries;
    *tries = 0;
    hit = (u8)ProbeTerrain(gLevelLayers, mode, &f.pos, span, &f.origY);
    if (hit) {
        y = f.origY;
        return 1;
    }
    {
        u8 saved = gLevelLayers->probeFlag;
        u8 *t2;
        struct probe_pos *pp;

        gLevelLayers->probeFlag = 0;
        t2 = tries;
        pp = &f.pos;
        do {
            (*t2)++;
            pp->y += 8;
            if ((u8)ProbeTerrain(gLevelLayers, mode, &f.pos, span, &f.origY)) {
                gLevelLayers->probeFlag = saved;
                return 0;
            }
        } while (*tries <= 2);
        gLevelLayers->probeFlag = saved;
    }
    return 0;
}
