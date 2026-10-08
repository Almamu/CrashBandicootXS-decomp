#include "core.h"
#include "memory.h"
#include "util.h"
#include "level.h"
#include "gfx.h"

/* Sits right after the parked CommitBlendRegs (asm/code_3_1_9.s) and
 * before the still-raw pause-menu/SIO cluster. */

/* Commits the `gBlendRegs` shadow to the real blend registers:
 * the word at `+0` covers both `REG_BLDCNT` and `REG_BLDALPHA` (a
 * single 32-bit write spanning the adjacent halfwords), and the low
 * 5 bits of the byte at `+4` become `REG_BLDY`. The mask is the ROM's
 * `(x << 27) >> 27` shift pair, which a plain `& 0x1f` would encode as
 * an AND with a constant instead. Built with old_agbcc, which derives
 * the BLDY address from BLDCNT's (`adds r2, #4`) as the ROM does; agbcc
 * loads it from a second literal. */
void CommitBlendRegs(void)
{
    u32 bldy;

    *(vu32 *)REG_ADDR_BLDCNT = gBlendRegs.blend.raw;
    bldy = gBlendRegs.bldy;
    REG_BLDY = (bldy << 27) >> 27;
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

void IwramFree(u8 *address)
{
    mem_free(address);
}

void *IwramAlloc(u32 size)
{
    return mem_alloc(size, 0x80000000);
}
