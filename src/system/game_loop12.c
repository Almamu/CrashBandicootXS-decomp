#include "core.h"

/* GitHub issue #41: 0x08025894-0x08025FC8. Counts, across every group
 * in `list` (a `{count:u16 @2, groups:ptr @4}` header) and every item
 * in each group (`{count:u16 @2, items:ptr @4}`, items 8 bytes apart),
 * how many items have an "effective type" that falls in
 * `[0x15, 0x27]` but is not one of `{0x18, 0x1a, 0x1b, 0x1c, 0x1d}`
 * (the ROM's jump table sends those five cases to the no-increment
 * path, everything else in range to the increment path, anything
 * outside the range skips the table read entirely via the `bhi`
 * short-circuit). An item's raw type (`u16 @item+0`) is used directly
 * unless it's `0x1a`, in which case the effective type is instead
 * looked up indirectly: `list->8` (a `u16` array) indexed by
 * `item->6` gives a byte offset into `list->0xc`, and the effective
 * type is the `s16` eight bytes past that.
 *
 * The `item->type == 0x1a` lookup does its whole four-load chain
 * using only `r0`/`r1` as scratch in the ROM, aggressively overwriting
 * each value the instant it's dead (item's own address is destroyed
 * by the very read that uses it, the table address is destroyed by
 * the read that dereferences it, and so on) - every plain-C shape
 * tried here kept at least one of those values alive in a third
 * register, colliding with the outer loop's `i` counter (pinned to
 * `r2` by the surrounding loop structure) and forcing an extra `r7`
 * push/pop the ROM does not have. Matched by emitting that one block
 * as an opaque `asm volatile` computing the effective type directly
 * from `l`/`item`, with `r0`/`r1` named explicitly in the asm text -
 * this keeps the block's own internal register churn invisible to the
 * surrounding function-level allocator, so `i` stays cleanly in `r2`
 * and the `r7` push/pop disappears. Splitting `i`'s own init
 * (`*(u16 *)(l + 2)` then `- 1`) into two statements was also needed:
 * as one combined expression this compiler loads the count into a
 * scratch register before subtracting into `i`'s register, instead of
 * the ROM's direct load-then-decrement-in-place into the same
 * register - see docs/matching/issue-41-game-loop-25894.md. */
s32 sub_8025894(void *self, void *list)
{
    u8 *l = (u8 *)list;
    s32 count = 0;
    s32 i;

    i = *(u16 *)(l + 2);
    i -= 1;

    for (; i >= 0; i--) {
        u8 *group = *(u8 **)(l + 4) + i * 8;
        s32 j;

        for (j = 0; j < *(u16 *)(group + 2); j++) {
            u8 *item = *(u8 **)(group + 4) + j * 8;
            s32 type = *(u16 *)item;

            if (type == 0x1a) {
                register void *itemReg asm("r1") = item;
                register s32 result asm("r0");

                asm volatile (
                    "ldr r0, [%1, #8]\n\t"
                    "ldrh r1, [r1, #6]\n\t"
                    "lsl r1, r1, #1\n\t"
                    "add r1, r1, r0\n\t"
                    "ldr r0, [%1, #0xc]\n\t"
                    "ldrh r1, [r1]\n\t"
                    "add r0, r1, r0\n\t"
                    "mov r1, #8\n\t"
                    "ldrsh r0, [r0, r1]\n\t"
                    : "=r" (result)
                    : "r" (l), "r" (itemReg)
                );
                type = result;
            }

            switch (type) {
                case 0x18: case 0x1a: case 0x1b: case 0x1c: case 0x1d:
                    break;
                case 0x15: case 0x16: case 0x17: case 0x19:
                case 0x1e: case 0x1f: case 0x20: case 0x21: case 0x22:
                case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
                    count++;
                    break;
            }
        }
    }
    return count;
}
asm(".align 2, 0");

/* Sets bit `n` (floor-divided into a 32-bit-word row, same idiom as
 * `sub_8025554` in game_loop5.c) of the bitmap array that starts 8
 * bytes into `self` - the first of a family of three overlapping
 * bitmap arrays this ROM region reads/writes at `self+8`/`self+0x208`/
 * `self+0x308` (see `sub_8025968`/`sub_802599C`/`sub_80259D4`/
 * `sub_8025A0C` alongside it, and `game_loop13.c`). */
void sub_8025944(void *self, s32 n)
{
    u8 *base = (u8 *)self;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 8;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    *word |= mask;
}

/* Tests bit `n` of the same `self+8` bitmap array `sub_8025944` sets. */
s32 sub_8025968(void *self, s32 n)
{
    u8 *base = (u8 *)self;
    s32 result = 0;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 8;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    if (*word & mask) {
        result = 1;
    }
    return result;
}

/* Tests bit `n` of the second bitmap array, at `self+0x208`. */
s32 sub_802599C(void *self, s32 n)
{
    u8 *base = (u8 *)self;
    s32 result = 0;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 0x208;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    if (*word & mask) {
        result = 1;
    }
    return result;
}

asm(".align 2, 0");
