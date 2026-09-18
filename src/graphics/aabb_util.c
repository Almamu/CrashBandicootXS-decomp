#include "core.h"
#include "memory.h"

/* Sits right after the parked sub_8001624 (asm/code_3_1_9.s) and
 * before the still-raw pause-menu/SIO cluster. */

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
 * Written as NAKED asm, not plain C: a full C reconstruction (kept in
 * git history) got the logic right, but the ROM writes the word then
 * does a separate `adds r2,#4` on the same register before the second
 * store (`str r0,[r2]; adds r2,#4; ...; strh r0,[r2]`); this compiler
 * always fuses that store-then-increment-same-register pair into a
 * single `stmia r2!,{r0}` regardless of how the pointer increment is
 * expressed in C (a separate statement, a memory-clobber barrier in
 * between, a fresh pointer variable) - an unavoidable peephole
 * optimization for this exact instruction pair. Every instruction
 * below is confirmed byte-identical to the ROM - full NAKED
 * transcription, like this project's other hard-compiler-limitation
 * cases (see `src/util/printf_util.c`'s `sub_8000CBC` for the
 * established pattern), is more honest than continuing to chase this
 * one peephole fusion through plain C. */
NAKED void sub_8001624(void)
{
    asm(
        "ldr r2, 1f\n\t"
        "ldr r1, 2f\n\t"
        "ldr r0, [r1]\n\t"
        "str r0, [r2]\n\t"
        "add r2, #4\n\t"
        "ldrb r1, [r1, #4]\n\t"
        "lsl r0, r1, #0x1b\n\t"
        "lsr r0, r0, #0x1b\n\t"
        "strh r0, [r2]\n\t"
        "bx lr\n\t"
        ".align 2, 0\n\t"
    "1: .4byte 0x04000050\n\t"
    "2: .4byte gUnknown_03001280\n\t"
    );
}
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
