#include "core.h"
#include "memory.h"

/* Sits right after the parked sub_8001624 (asm/code_3_1_9.s) and
 * before the still-raw pause-menu/SIO cluster. */

#if NON_MATCHING
struct unk_03001280 {
    u32 bldcntAlpha;
    u8 bldy;
};

extern struct unk_03001280 gUnknown_03001280;

/* Commits the `gUnknown_03001280` shadow to the real blend registers:
 * the word at `+0` covers both `REG_BLDCNT` and `REG_BLDALPHA` (a
 * single 32-bit write spanning the adjacent halfwords), and the low
 * 5 bits of the byte at `+4` become `REG_BLDY`.
 *
 * Parked: the ROM writes the word then does a separate `adds r2,#4`
 * on the same register before the second store (`str r0,[r2]; adds
 * r2,#4; ...; strh r0,[r2]`); this compiler always fuses that
 * store-then-increment-same-register pair into a single `stmia
 * r2!,{r0}` regardless of how the pointer increment is expressed in
 * C (a separate statement, a memory-clobber barrier in between, a
 * fresh pointer variable) - an unavoidable peephole optimization for
 * this exact instruction pair. */
void sub_8001624(void)
{
    register vu32 *bldReg asm("r2") = (vu32 *)0x04000050;
    register struct unk_03001280 *p asm("r1") = &gUnknown_03001280;
    register u32 word asm("r0") = p->bldcntAlpha;
    register s32 byte asm("r1");
    register s32 result asm("r0");

    *bldReg = word;
    bldReg = (vu32 *)((u8 *)bldReg + 4);
    byte = p->bldy;
    result = (u32)(byte << 27) >> 27;
    *(vu16 *)bldReg = result;
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

/* Axis-aligned box overlap test, X-axis edges inclusive (touching
 * counts as overlap) - the `sub_800B37C`-family collision checks in
 * actor_part*.c use the stricter `sub_8001688` below instead. */
u8 sub_8001640(struct aabb *a, struct aabb *b)
{
    u8 result = 0;

    if (a->field_8 > 0 && b->field_8 > 0) {
        s32 aMaxX = a->field_0 + a->field_8;
        s32 bMaxX = b->field_0 + b->field_8;

        if (a->field_0 <= bMaxX && b->field_0 <= aMaxX) {
            s32 aMaxY = a->field_4 + a->field_c;
            s32 bMaxY = b->field_4 + b->field_c;
            u8 temp = 0;

            if (a->field_4 < bMaxY && b->field_4 < aMaxY) {
                temp = 1;
            }
            result = temp;
        }
    }
    return result;
}

/* Same axis-aligned box overlap test as `sub_8001640`, but with the
 * X-axis edges exclusive too (touching does not count) - this is the
 * variant already referenced by name from `actor_part15.c`'s
 * `sub_800B37C` and the pool/grid collision functions in
 * `actor_part11.c`. */
u8 sub_8001688(struct aabb *a, struct aabb *b)
{
    u8 result = 0;

    if (a->field_8 > 0 && b->field_8 > 0) {
        s32 aMaxX = a->field_0 + a->field_8;
        s32 bMaxX = b->field_0 + b->field_8;

        if (a->field_0 < bMaxX && b->field_0 < aMaxX) {
            s32 aMaxY = a->field_4 + a->field_c;
            s32 bMaxY = b->field_4 + b->field_c;
            u8 temp = 0;

            if (a->field_4 < bMaxY && b->field_4 < aMaxY) {
                temp = 1;
            }
            result = temp;
        }
    }
    return result;
}

void sub_80016D0(u8 *address)
{
    mem_free(address);
}

void *sub_80016DC(u32 size)
{
    return mem_alloc(size, 0x80000000);
}
asm(".align 2, 0");
