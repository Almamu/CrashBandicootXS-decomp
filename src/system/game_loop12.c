#include "core.h"

#if NON_MATCHING
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
 * PARKED, NOT BYTE-MATCHING: every field, offset and branch is
 * confirmed against the ROM (including the jump table's own 19-entry
 * case grouping, reproduced with an explicit `switch`). The remaining
 * gap is register pressure in the `item->type == 0x1a` lookup block:
 * the ROM does the whole four-load chain (`list->8`, `item->6`,
 * `list->0xc`, the two intermediate dereferences) using only `r0`/`r1`
 * as scratch, aggressively overwriting each value the instant it's
 * dead (item's own address is destroyed by the very read that uses
 * it, the table address is destroyed by the read that dereferences
 * it, and so on). Every C shape tried here - inline expressions,
 * named locals in ROM order, named locals reusing a single pointer
 * variable across all four steps - keeps at least one of those values
 * alive in a third register, which collides with the outer loop's `i`
 * counter (itself already pinned to r2 by the surrounding loop
 * structure) and forces an extra `r7` push/pop the ROM does not have.
 * Parked rather than keep chasing this specific reuse pattern -
 * see docs/matching/issue-41-game-loop-25894.md. */
s32 sub_8025894(void *self, void *list)
{
    u8 *l = (u8 *)list;
    s32 count = 0;
    s32 i = (s32)(*(u16 *)(l + 2)) - 1;

    for (; i >= 0; i--) {
        u8 *group = *(u8 **)(l + 4) + i * 8;
        s32 j;

        for (j = 0; j < *(u16 *)(group + 2); j++) {
            u8 *item = *(u8 **)(group + 4) + j * 8;
            s32 type = *(u16 *)item;

            if (type == 0x1a) {
                void *p = *(u8 **)(l + 8);
                u16 idx = *(u16 *)(item + 6);

                p = (u8 *)p + idx * 2;
                {
                    u8 *recBase = *(u8 **)(l + 0xc);
                    u16 sub = *(u16 *)p;

                    p = recBase + sub;
                }
                type = *(s16 *)((u8 *)p + 8);
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
#endif /* NON_MATCHING */
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
