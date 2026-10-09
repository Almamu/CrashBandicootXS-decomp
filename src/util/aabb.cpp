/* The two AABB overlap tests (AabbOverlapsInclusiveX, AabbOverlaps).
 * CommitBlendRegs went to the end of gfx/display.cpp and IwramFree/IwramAlloc
 * to system/iwram_alloc.cpp (#767; all old_agbcc). */

extern "C" {
#include "core.h"
#include "memory.h"
#include "util.h"
#include "level.h"
#include "gfx.h"
}

/* Axis-aligned box overlap test, X-axis edges inclusive (touching
 * counts as overlap) - the `PlayerTouchesBox`-family collision checks in
 * the sprite-object files use the stricter `AabbOverlaps` below instead. */
u8 AabbOverlapsInclusiveX(struct aabb *a, struct aabb *b)
{
    u8 result = 0;

    if (a->w > 0 && b->w > 0) {
        s32 aMaxX = a->x + a->w;
        s32 bMaxX = b->x + b->w;

        if (a->x <= bMaxX && b->x <= aMaxX) {
            s32 aMaxY = a->y + a->h;
            s32 bMaxY = b->y + b->h;
            u8 temp = 0;

            if (a->y < bMaxY && b->y < aMaxY) {
                temp = 1;
            }
            result = temp;
        }
    }
    return result;
}

/* Same axis-aligned box overlap test as `AabbOverlapsInclusiveX`, but with the
 * X-axis edges exclusive too (touching does not count) - this is the
 * variant already referenced by name from `player_update.c`'s
 * `PlayerTouchesBox` and the pool/grid collision functions in
 * `part_list.cpp`. */
u8 AabbOverlaps(struct aabb *a, struct aabb *b)
{
    u8 result = 0;

    if (a->w > 0 && b->w > 0) {
        s32 aMaxX = a->x + a->w;
        s32 bMaxX = b->x + b->w;

        if (a->x < bMaxX && b->x < aMaxX) {
            s32 aMaxY = a->y + a->h;
            s32 bMaxY = b->y + b->h;
            u8 temp = 0;

            if (a->y < bMaxY && b->y < aMaxY) {
                temp = 1;
            }
            result = temp;
        }
    }
    return result;
}
