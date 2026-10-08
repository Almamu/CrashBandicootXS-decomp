#include "core.h"
#include "match.h"
#include "memory.h"
#include "util.h"
#include "level.h"
#include "gfx.h"

/* Sits right after the parked CommitBlendRegs (asm/code_3_1_9.s) and
 * before the still-raw pause-menu/SIO cluster. */

/* Commits the `gBlendRegs` shadow to the real blend registers:
 * the word at `+0` covers both `REG_BLDCNT` and `REG_BLDALPHA` (a
 * single 32-bit write spanning the adjacent halfwords), and the low
 * 5 bits of the byte at `+4` become `REG_BLDY`.
 *
 * The ROM writes the word then does a separate `adds r2,#4` on the
 * same register before the second store, where this compiler always
 * fuses a normal C-level store-then-increment-same-register pair into
 * a single `stmia r2!,{r0}` instead (an unavoidable peephole
 * optimization, regardless of how the increment is expressed in C).
 * The fix is the same one used for `SetDispcntMode`'s value-propagation
 * fold: emit the store-and-increment pair as one inline-asm block,
 * opaque to the peephole pass, so it can't recognize and fuse it. The
 * `bldy` mask is written as the ROM's own `(x << 27) >> 27` shift
 * pair rather than a plain `& 0x1f`, which this compiler would
 * otherwise encode as a direct AND-immediate instead. */
void CommitBlendRegs(void)
{
    MATCH_HOLD_REG(vu32 *, bldReg, r2) = (vu32 *)REG_ADDR_BLDCNT;
    struct blend_regs *src = &gBlendRegs;
    MATCH_HOLD_REG(u32, word, r0) = src->blend.raw;
    MATCH_HOLD_REG(u32, bldy, r1);
    MATCH_HOLD_REG(u32, masked, r0);

    asm volatile("str %1, [%0]\n\tadd %0, %0, #4" : "+r"(bldReg) : "r"(word));

    bldy = src->bldy;
    masked = (bldy << 27) >> 27;
    *(vu16 *)bldReg = masked;
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
