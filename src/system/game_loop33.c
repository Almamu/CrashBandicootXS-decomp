#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). */

/* Full 4-octant Bresenham-line-style line-stepper: treats `(pos,
 * count)` and `(a, b)` as two `(position, value)` pairs, sorts them by
 * `count`/`b` (swapping both coordinates together if `count > b`),
 * then walks from the lower-`count` point toward the higher-`count`
 * one, returning the `pos` (`x`-like) coordinate the moment it steps
 * past `limit`, or -1 if the walk completes `|b - count|` steps
 * without ever reaching it. Which of `pos`/`count` is the "major"
 * (always-advancing) axis and which direction `pos` steps depends on
 * the sign and relative magnitude of `dx = a - pos` vs `dy = b -
 * count`, the classic 4-case Bresenham octant split - each case is a
 * fixed single-octant variant of the same shape
 * `sub_8010784`/`sub_80107C4` (game_loop31.c) already establish:
 *
 *   dx = a - pos; dy = b - count;
 *   if (dx > 0) {
 *       if (dx > dy) {           // X major, increasing
 *           for (twoDx = dx*2, twoDy = dy*2, diff = twoDy-twoDx,
 *                err = twoDy-dx, n = dx-1; n != -1; n--) {
 *               if (err >= 0) { count++; err += diff; } else { err += twoDy; }
 *               if (++pos >= limit) return count;
 *           }
 *       } else {                 // Y major, X advances conditionally, increasing
 *           for (twoDx = dx*2, twoDy = dy*2, diff = twoDx-twoDy,
 *                err = twoDx-dy, n = dy-1; n != -1; n--) {
 *               if (err >= 0) { if (++pos >= limit) return count; err += diff; }
 *               else { err += twoDx; }
 *               count++;
 *           }
 *       }
 *   } else {
 *       absDx = -dx;
 *       if (absDx > dy) {        // X major, decreasing
 *           for (twoAbsDx = absDx*2, twoDy = dy*2, diff = twoDy-twoAbsDx,
 *                err = twoDy-absDx, n = absDx-1; n != -1; n--) {
 *               if (err >= 0) { count++; err += diff; } else { err += twoDy; }
 *               if (--pos >= limit) return count;
 *           }
 *       } else {                 // Y major, X advances conditionally, decreasing
 *           for (twoAbsDx = absDx*2, twoDy = dy*2, diff = twoAbsDx-twoDy,
 *                err = twoAbsDx-dy, n = dy-1; n != -1; n--) {
 *               if (err >= 0) { if (--pos >= limit) return count; err += diff; }
 *               else { err += twoAbsDx; }
 *               count++;
 *           }
 *       }
 *   }
 *   return -1;
 *
 * Written as NAKED asm, not plain C: this compiler's cross-jump pass
 * notices the X-major-increasing case's own early-return (`adds
 * r0,r1,#0; b <exit>`) is byte-identical to the shared early-return
 * the other three cases already fold into one physical copy (matching
 * the ROM's own choice there), and folds *all four* into a single
 * shared tail - 4 bytes shorter than the ROM, which keeps the first
 * case's copy separate (its own early-return is never reached from
 * any other case) while still sharing the other three. A `asm
 * volatile("" : "+r"(count))` barrier right at that one return site
 * (the established fix for a same-value provable-equal cross-jump,
 * see docs/matching.md's `text_layout.c` entry) has no effect here
 * since this merge isn't value-driven - the barrier's own zero
 * instructions vanish before the late-stage cross-jump pass runs, and
 * no combination of restructuring found kept that one return
 * physically separate without changing the byte count elsewhere.
 * Every instruction below is checked byte-identical to the ROM. */
NAKED s32 sub_800FDC8(s32 pos, s32 count, s32 a, s32 b, s32 limit)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r7, [sp, #0x14]\n\t"
        "cmp r1, r3\n\t"
        "ble 1f\n\t"
        "add r0, r1, #0\n\t"
        "add r1, r3, #0\n\t"
        "add r3, r0, #0\n\t"
        "add r0, r4, #0\n\t"
        "add r4, r2, #0\n\t"
        "add r2, r0, #0\n\t"
    "1:\n\t"
        "sub r2, r2, r4\n\t"
        "sub r3, r3, r1\n\t"
        "cmp r2, #0\n\t"
        "ble 8f\n\t"
        "cmp r2, r3\n\t"
        "ble 5f\n\t"
        "lsl r3, r3, #1\n\t"
        "lsl r0, r2, #1\n\t"
        "sub r6, r3, r0\n\t"
        "sub r0, r3, r2\n\t"
        "sub r2, #1\n\t"
        "mov r5, #1\n\t"
        "neg r5, r5\n\t"
        "cmp r2, r5\n\t"
        "beq 13f\n\t"
    "2:\n\t"
        "cmp r0, #0\n\t"
        "blt 3f\n\t"
        "add r1, #1\n\t"
        "add r0, r0, r6\n\t"
        "b 4f\n\t"
    "3:\n\t"
        "add r0, r0, r3\n\t"
    "4:\n\t"
        "add r4, #1\n\t"
        "cmp r4, r7\n\t"
        "bge 12f\n\t"
        "sub r2, #1\n\t"
        "cmp r2, r5\n\t"
        "bne 2b\n\t"
        "b 13f\n\t"
    "12:\n\t"
        "add r0, r1, #0\n\t"
        "b 14f\n\t"
    "5:\n\t"
        "lsl r2, r2, #1\n\t"
        "lsl r0, r3, #1\n\t"
        "sub r6, r2, r0\n\t"
        "sub r0, r2, r3\n\t"
        "sub r3, #1\n\t"
        "mov r5, #1\n\t"
        "neg r5, r5\n\t"
        "cmp r3, r5\n\t"
        "beq 13f\n\t"
    "6:\n\t"
        "cmp r0, #0\n\t"
        "blt 7f\n\t"
        "add r4, #1\n\t"
        "cmp r4, r7\n\t"
        "bge 11f\n\t"
        "add r0, r0, r6\n\t"
        "b 9f\n\t"
    "7:\n\t"
        "add r0, r0, r2\n\t"
    "9:\n\t"
        "add r1, #1\n\t"
        "sub r3, #1\n\t"
        "cmp r3, r5\n\t"
        "bne 6b\n\t"
        "b 13f\n\t"
    "8:\n\t"
        "neg r2, r2\n\t"
        "cmp r2, r3\n\t"
        "ble 10f\n\t"
        "lsl r3, r3, #1\n\t"
        "lsl r0, r2, #1\n\t"
        "sub r6, r3, r0\n\t"
        "sub r0, r3, r2\n\t"
        "sub r2, #1\n\t"
        "mov r5, #1\n\t"
        "neg r5, r5\n\t"
        "cmp r2, r5\n\t"
        "beq 13f\n\t"
    "16:\n\t"
        "cmp r0, #0\n\t"
        "blt 17f\n\t"
        "add r1, #1\n\t"
        "add r0, r0, r6\n\t"
        "b 18f\n\t"
    "17:\n\t"
        "add r0, r0, r3\n\t"
    "18:\n\t"
        "sub r4, #1\n\t"
        "cmp r4, r7\n\t"
        "bge 11f\n\t"
        "sub r2, #1\n\t"
        "cmp r2, r5\n\t"
        "bne 16b\n\t"
        "b 13f\n\t"
    "11:\n\t"
        "add r0, r1, #0\n\t"
        "b 14f\n\t"
    "10:\n\t"
        "lsl r2, r2, #1\n\t"
        "lsl r0, r3, #1\n\t"
        "sub r6, r2, r0\n\t"
        "sub r0, r2, r3\n\t"
        "sub r3, #1\n\t"
        "mov r5, #1\n\t"
        "neg r5, r5\n\t"
        "cmp r3, r5\n\t"
        "beq 13f\n\t"
    "19:\n\t"
        "cmp r0, #0\n\t"
        "blt 20f\n\t"
        "sub r4, #1\n\t"
        "cmp r4, r7\n\t"
        "bge 11b\n\t"
        "add r0, r0, r6\n\t"
        "b 21f\n\t"
    "20:\n\t"
        "add r0, r0, r2\n\t"
    "21:\n\t"
        "add r1, #1\n\t"
        "sub r3, #1\n\t"
        "cmp r3, r5\n\t"
        "bne 19b\n\t"
    "13:\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
    "14:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}
