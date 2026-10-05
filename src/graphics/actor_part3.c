#include "core.h"
#include "actor.h"
#include "box_part.h"

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern void *gLevelLayers;

/* Same shape as IsEntityNearCamera (graphics.c) - `part+0x25 == 1` is a fast
 * "always visible" override; otherwise `part+0xd` bit 2 gates an
 * on-screen check via `_call_via_r2`, using a 4-word "region" of
 * `{gLevelLayers's sub-object's two Q8 fields, 240<<8, 160<<8}`
 * (the GBA's screen width/height) and the same
 * `table+N`/`table+N+4` offset/pointer slot pair convention
 * IsEntityNearCamera reads at `table+0x40`, here at `table+0x30` (the
 * part's method-table entry, see PART_METHOD). */
s32 IsSpriteObjOnScreen(struct box_part *part)
{
    register s32 result asm("r3") = 0;

    if (*((u8 *)part + 0x25) == 1) {
        return 1;
    }

    {
        register u32 flags asm("r1");
        register s32 bit2 asm("r0");

        flags = *((u8 *)part + 0xd);
        bit2 = (flags >> 2) & 1;
        if (!bit2) {
            s32 buf[4];
            register void *subObj asm("r0");
            struct part_method *table;

            subObj = *(void **)((u8 *)gLevelLayers + 0x10);
            {
                s32 field0 = *(s32 *)subObj << 8;
                s32 field4 = *(s32 *)((u8 *)subObj + 4) << 8;

                buf[0] = field0;
                buf[1] = field4;
            }
            {
                s32 width = 0xf0 << 8;
                s32 height = 0xa0 << 8;

                buf[2] = width;
                buf[3] = height;
            }

            table = PART_METHOD(part, 0x30);
            result = (u8)_call_via_r2((u8 *)part + table->thisOffset, buf, table->fn);
        }
    }
    return result;
}

extern void GetSpriteBounds(void *dest, void *part);

/* Same `part+0x25`/`part+0xd` bit-2 fast-path shape as IsSpriteObjOnScreen
 * above, but the real check is an AABB-overlap test: `part`'s own box
 * (via GetSpriteBounds) against `region`'s `{s32 x, y, w, h}`. */
s32 SpriteObjOverlapsRect(struct actor *part, struct part_aabb *region)
{
    register s32 earlyResult asm("r3") = 0;

    if (*((u8 *)part + 0x25) == 1) {
        return 1;
    }

    {
        register u32 flags asm("r1");
        register s32 bit2 asm("r0");

        flags = *((u8 *)part + 0xd);
        bit2 = (flags >> 2) & 1;
        if (bit2) {
            return earlyResult;
        }
    }

    {
        struct part_aabb box;
        s32 x1, y1, x2, y2;
        s32 result;

        GetSpriteBounds(&box, part);

        x1 = box.x << 8;
        y1 = box.y << 8;
        x2 = x1 + (box.w << 8);
        y2 = y1 + (box.h << 8);

        result = 0;
        if (x2 > region->x) {
            if (x1 < region->x + region->w) {
                if (y2 > region->y) {
                    if (y1 < region->y + region->h) {
                        result = 1;
                    }
                }
            }
        }
        earlyResult = result;
        return earlyResult;
    }
}
asm(".align 2, 0");

/* Advances `part`'s per-keyframe animation timer by one tick, only
 * while `animating` is set. `timer` counts up each tick against the
 * current keyframe's `duration`; once it reaches it, `timer` resets and
 * `tick` (the step index) advances. When `tick` reaches the keyframe's
 * `steps`, both counters reset and - unless the keyframe's loop flag
 * (bit 1) is set - `animDone` is set.
 *
 * Matches under old_agbcc (see docs/matching/issue-9-naked-retry.md).
 * The ROM keeps `part` in `ip` for the whole function; that falls out
 * naturally from this plain shape under the old compiler. The two
 * details the shape pins down: the second half reads `tick` into a
 * local and then `*keyframes` once into `kf` (reused by the flag test,
 * while `frame` is re-read each time), and `animDone` is stored from a
 * local (`movs r1, #1` before the address, not after). */
void AdvanceSpriteAnim(struct box_part *part)
{
    if (part->animating) {
        s32 timer = part->timer;
        s32 tick;
        struct keyframe *kf;

        if (timer < (*part->keyframes)[part->frame].duration)
            part->timer = timer + 1;
        else {
            part->timer = 0;
            part->tick++;
        }

        tick = part->tick;
        kf = *part->keyframes;
        if (tick >= kf[part->frame].steps) {
            part->tick = 0;
            part->timer = 0;
            if (!(kf[part->frame].flags & 2)) {
                u8 done = TRUE;
                part->animDone = done;
            }
        }
    }
}
