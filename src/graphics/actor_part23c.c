#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Position-easing helper, called from `AirshipStateFireballs`/`AirshipStateCannon`
 * (actor_part21d.c/actor_part21e.c): advances the position
 * accumulators (`gAirshipX`/`gAirshipY`) by their
 * per-frame deltas (`gAirshipVelX`/`gAirshipVelY`), then
 * computes the player's (`gActorList`) signed distance from a
 * fixed keyframe-table-relative target point on each axis
 * (`self+0x1c`/`0x20` against `gAirshipScreenX`/`gAirshipScreenY`
 * offset by `gAirshipBox`'s box) and, per axis, nudges a
 * "shake"/camera-offset accumulator (`gAirshipScreenX`/
 * `gAirshipScreenY`, via `ip`/`r8`) toward the target in small
 * discrete steps once the distance exceeds a `0x2CFF` threshold and a
 * finer `>>10` sub-threshold. Also clamps both accumulators against a
 * set of fixed ranges/bias points (`0xa000`/`0x4FFF`, `0xFFFFD300`/
 * `0x13FF`) and a final `0x180`/`-0x180`, `0x100`/`-0x100` hard clamp.
 */
extern s32 gAirshipX;
extern s32 gAirshipVelX;
extern s32 gAirshipY;
extern s32 gAirshipVelY;
extern struct actor_self *gActorList;
extern s32 gAirshipScreenX;
extern const s16 gAirshipBox[];
extern s32 gAirshipScreenY;

static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

/* The register split between `&gAirshipVelX` (r6) and
 * `&gAirshipVelY` (r4) follows from how many stores each easing
 * block has before cross-jumping merges them: the X step stores its
 * `vx -+ 3` result once, the Y step stores in each branch. That makes
 * the Y address the higher-priority pseudo for global-alloc, as in the
 * ROM (docs/matching/issue-58-61-naked-retry.md). */
void sub_8030E08(void)
{
    s32 vx;
    s32 dx, dy, cx, cy, px, py;
    struct actor_self *pl;

    gAirshipX += gAirshipVelX;
    gAirshipY += gAirshipVelY;

    pl = gActorList;
    px = pl->x;
    cx = gAirshipScreenX - 0x1200;
    dx = px - cx - (gAirshipBox[0] + gAirshipBox[3] / 2);
    py = pl->y;
    cy = gAirshipScreenY + 0x1800;
    dy = py - cy - (gAirshipBox[1] + gAirshipBox[4] / 2);

    if (Abs(dx) <= 0x2CFF) {
        s32 s = dx >> 10;
        s32 t;

        vx = gAirshipVelX;
        if (s >= 0) {
            gAirshipVelX = vx;
            if (s == 0)
                goto dx_done;
            t = vx - 3;
        } else
            t = vx + 3;
        gAirshipVelX = t;
    }
dx_done:
    if (Abs(dy) <= 0x2CFF) {
        s32 v;

        if ((dy >> 10) >= 0) {
            v = gAirshipVelY;
            if ((dy >> 10) == 0)
                goto dy_done;
            gAirshipVelY = v - 2;
        } else {
            v = gAirshipVelY;
            gAirshipVelY = v + 2;
        }
    }
dy_done:

    if (gAirshipScreenX <= 0x1400)
        gAirshipVelX += 6;
    if (gAirshipScreenX > 0x4FFF)
        gAirshipVelX -= 6;
    if (gAirshipScreenY <= -0x2D00)
        gAirshipVelY += 3;
    if (gAirshipScreenY > 0x13FF)
        gAirshipVelY -= 3;

    {
        s32 *p = &gAirshipVelX;
        s32 v = *p;

        if (v > 0x180)
            v = 0x180;
        *p = v;
        if (v < -0x180)
            v = -0x180;
        gAirshipVelX = v;
    }
    {
        s32 *p = &gAirshipVelY;
        s32 v = *p;

        if (v > 0x100)
            v = 0x100;
        *p = v;
        if (v < -0x100)
            v = -0x100;
        gAirshipVelY = v;
    }
}
