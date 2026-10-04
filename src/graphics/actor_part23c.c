#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Position-easing helper, called from `sub_8030734`/`sub_8030834`
 * (actor_part21d.c/actor_part21e.c): advances the position
 * accumulators (`gUnknown_03001540`/`gUnknown_03001544`) by their
 * per-frame deltas (`gUnknown_03001558`/`gUnknown_0300155C`), then
 * computes the player's (`gActorList`) signed distance from a
 * fixed keyframe-table-relative target point on each axis
 * (`self+0x1c`/`0x20` against `gUnknown_0300154C`/`gUnknown_03001550`
 * offset by `gStaticData_0817C3D8`'s box) and, per axis, nudges a
 * "shake"/camera-offset accumulator (`gUnknown_0300154C`/
 * `gUnknown_03001550`, via `ip`/`r8`) toward the target in small
 * discrete steps once the distance exceeds a `0x2CFF` threshold and a
 * finer `>>10` sub-threshold. Also clamps both accumulators against a
 * set of fixed ranges/bias points (`0xa000`/`0x4FFF`, `0xFFFFD300`/
 * `0x13FF`) and a final `0x180`/`-0x180`, `0x100`/`-0x100` hard clamp.
 */
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001558;
extern s32 gUnknown_03001544;
extern s32 gUnknown_0300155C;
extern struct actor_self *gActorList;
extern s32 gUnknown_0300154C;
extern const s16 gStaticData_0817C3D8[];
extern s32 gUnknown_03001550;

static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

/* The register split between `&gUnknown_03001558` (r6) and
 * `&gUnknown_0300155C` (r4) follows from how many stores each easing
 * block has before cross-jumping merges them: the X step stores its
 * `vx -+ 3` result once, the Y step stores in each branch. That makes
 * the Y address the higher-priority pseudo for global-alloc, as in the
 * ROM (docs/matching/issue-58-61-naked-retry.md). */
void sub_8030E08(void)
{
    s32 vx;
    s32 dx, dy, cx, cy, px, py;
    struct actor_self *pl;

    gUnknown_03001540 += gUnknown_03001558;
    gUnknown_03001544 += gUnknown_0300155C;

    pl = gActorList;
    px = pl->x;
    cx = gUnknown_0300154C - 0x1200;
    dx = px - cx - (gStaticData_0817C3D8[0] + gStaticData_0817C3D8[3] / 2);
    py = pl->y;
    cy = gUnknown_03001550 + 0x1800;
    dy = py - cy - (gStaticData_0817C3D8[1] + gStaticData_0817C3D8[4] / 2);

    if (Abs(dx) <= 0x2CFF) {
        s32 s = dx >> 10;
        s32 t;

        vx = gUnknown_03001558;
        if (s >= 0) {
            gUnknown_03001558 = vx;
            if (s == 0)
                goto dx_done;
            t = vx - 3;
        } else
            t = vx + 3;
        gUnknown_03001558 = t;
    }
dx_done:
    if (Abs(dy) <= 0x2CFF) {
        s32 v;

        if ((dy >> 10) >= 0) {
            v = gUnknown_0300155C;
            if ((dy >> 10) == 0)
                goto dy_done;
            gUnknown_0300155C = v - 2;
        } else {
            v = gUnknown_0300155C;
            gUnknown_0300155C = v + 2;
        }
    }
dy_done:

    if (gUnknown_0300154C <= 0x1400)
        gUnknown_03001558 += 6;
    if (gUnknown_0300154C > 0x4FFF)
        gUnknown_03001558 -= 6;
    if (gUnknown_03001550 <= -0x2D00)
        gUnknown_0300155C += 3;
    if (gUnknown_03001550 > 0x13FF)
        gUnknown_0300155C -= 3;

    {
        s32 *p = &gUnknown_03001558;
        s32 v = *p;

        if (v > 0x180)
            v = 0x180;
        *p = v;
        if (v < -0x180)
            v = -0x180;
        gUnknown_03001558 = v;
    }
    {
        s32 *p = &gUnknown_0300155C;
        s32 v = *p;

        if (v > 0x100)
            v = 0x100;
        *p = v;
        if (v < -0x100)
            v = -0x100;
        gUnknown_0300155C = v;
    }
}
