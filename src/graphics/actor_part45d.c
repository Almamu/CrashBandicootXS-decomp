#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md.
 *
 * A pair of ~130-170-instruction VRAM tile-remap loops (issue #56),
 * both 4-bit palette-index repacking loops into a `0x0600D000`-based
 * tile buffer via raw `REG_DMA3SAD`/`DAD`/`CNT` (`0x040000D4`) pokes:
 *
 * - `sub_802F7B0(u8 *src)`: starts a DMA3 32-bit copy of `src` itself
 *   into some destination read back from `src+0x200`/`src+0x202`
 *   (halfwords) and a header dword at `src+0x204`, computes a
 *   rounded-average buffer size from those two halfwords via
 *   `sub_8029AC4()`'s VRAM-tile-allocator result, then repacks a
 *   `src`-relative halfword array plus a nibble-packed byte array into
 *   two interleaved 32x-wide output rows (odd/even nibble halves) at
 *   that buffer, alternating source nibble high/low half each output
 *   row via an `ip`-held toggle flag; finishes by starting a second
 *   DMA3 copy (BG palette-ish header poke via `0x0400000A`) and a
 *   final `REG_DMA3CNT`-style transfer using the same rounded buffer
 *   size and row-derived sizing.
 * - `sub_802F8E8(u8 *dest, u16 *src, s32 rowCount, s32 colCount)`:
 *   the same nibble-toggling 32-wide interleaved-row repack loop
 *   (odd/even output row split via a `> 0x1f` column-index check)
 *   feeding a `0x0600D000`-based destination directly from explicit
 *   arguments instead of `sub_802F7B0`'s own header-driven setup,
 *   sharing the exact inner-loop shape and nibble-toggle idiom.
 *
 * Transcribed as NAKED asm, not plain C: both loops keep three extra
 * high registers (`r8`, `sb`/r9, `sl`/r10) simultaneously live across
 * the whole nested loop body (row count/pointer in one, column
 * bookkeeping in another, the alternating-row output pointer in the
 * third), on top of the raw `0x040000D4`/`0x0600D000` DMA hardware
 * pokes - every other DMA3-setup function already in this codebase
 * with this same `0x040000D4`/`0x0600D000` literal-pool shape
 * (actor_part26b.c, actor_part74.c, actor_part75.c, fade_screen_mode.c,
 * hud_digit_array.c, settings_menu8e.c, timer_util_aa90.c) is NAKED
 * too, not plain C with `REG_DMA3SAD`/`DAD`/`CNT` macros - this
 * compiler's register allocator never reproduces the ROM's specific
 * three-high-register nested-loop allocation for this shape. Every
 * load/store, branch and call below is confirmed correct against the
 * ROM disassembly - see docs/matching/issue-56-0x0802f0dc-actor.md. */
extern s32 sub_8029AC4(void);

NAKED void sub_802F7B0(void *srcArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x14\n\t"
        "ldr r2, 3f\n\t"
        "str r0, [r2]\n\t"
        "mov r1, #0xa0\n\t"
        "lsl r1, r1, #0x13\n\t"
        "str r1, [r2, #4]\n\t"
        "ldr r1, 4f\n\t"
        "str r1, [r2, #8]\n\t"
        "ldr r1, [r2, #8]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #2\n\t"
        "add r1, r0, r2\n\t"
        "mov r5, #0\n\t"
        "ldrsh r3, [r1, r5]\n\t"
        "mov r8, r3\n\t"
        "add r2, #2\n\t"
        "add r1, r0, r2\n\t"
        "mov r5, #0\n\t"
        "ldrsh r3, [r1, r5]\n\t"
        "str r3, [sp]\n\t"
        "mov r1, #0x81\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldm r0!, {r2}\n\t"
        "str r2, [sp, #4]\n\t"
        "mov r1, r8\n\t"
        "mul r1, r3\n\t"
        "add r1, #1\n\t"
        "lsr r2, r1, #0x1f\n\t"
        "add r1, r1, r2\n\t"
        "asr r1, r1, #1\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r0, r1\n\t"
        "str r1, [sp, #8]\n\t"
        "ldr r3, [sp, #4]\n\t"
        "lsl r1, r3, #5\n\t"
        "ldr r5, [sp, #8]\n\t"
        "add r6, r1, r5\n\t"
        "add r7, r0, #0\n\t"
        "ldr r5, 5f\n\t"
        "bl sub_8029AC4\n\t"
        "ldr r1, 6f\n\t"
        "add r1, r0, r1\n\t"
        "str r1, [sp, #0xc]\n\t"
        "mov r2, #0\n\t"
        "mov ip, r2\n\t"
        "mov r0, #0\n\t"
        "ldr r3, [sp]\n\t"
        "cmp r0, r3\n\t"
        "bge 12f\n\t"
    "1:\n\t"
        "mov r4, #0\n\t"
        "mov r1, #0x40\n\t"
        "add r1, r1, r5\n\t"
        "mov sb, r1\n\t"
        "add r0, #1\n\t"
        "mov sl, r0\n\t"
        "cmp r4, r8\n\t"
        "bge 11f\n\t"
        "mov r2, #0xf8\n\t"
        "lsl r2, r2, #3\n\t"
        "add r3, r5, r2\n\t"
        "add r2, r5, #0\n\t"
    "2:\n\t"
        "ldrh r5, [r7]\n\t"
        "ldr r0, [sp, #0xc]\n\t"
        "add r5, r5, r0\n\t"
        "str r5, [sp, #0x10]\n\t"
        "add r7, #2\n\t"
        "mov r1, ip\n\t"
        "cmp r1, #0\n\t"
        "beq 7f\n\t"
        "ldrb r5, [r6]\n\t"
        "lsr r1, r5, #4\n\t"
        "add r6, #1\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "3: .4byte 0x040000D4\n"
    "4: .4byte 0x80000100\n"
    "5: .4byte 0x0600D000\n"
    "6: .4byte 0xFFFFFE00\n"
    "7:\n\t"
        "mov r1, #0xf\n\t"
        "ldrb r0, [r6]\n\t"
        "and r1, r0\n\t"
    "8:\n\t"
        "mov r0, #1\n\t"
        "mov r5, ip\n\t"
        "eor r5, r0\n\t"
        "mov ip, r5\n\t"
        "lsl r1, r1, #0xc\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "orr r1, r0\n\t"
        "cmp r4, #0x1f\n\t"
        "bgt 9f\n\t"
        "strh r1, [r2]\n\t"
        "b 10f\n\t"
    "9:\n\t"
        "strh r1, [r3]\n\t"
    "10:\n\t"
        "add r3, #2\n\t"
        "add r2, #2\n\t"
        "add r4, #1\n\t"
        "cmp r4, r8\n\t"
        "blt 2b\n\t"
    "11:\n\t"
        "mov r5, sb\n\t"
        "mov r0, sl\n\t"
        "ldr r1, [sp]\n\t"
        "cmp r0, r1\n\t"
        "blt 1b\n\t"
    "12:\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #0x13\n\t"
        "ldrh r0, [r2]\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #2\n\t"
        "add r1, r3, #0\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r2]\n\t"
        "ldr r1, 13f\n\t"
        "ldr r5, 14f\n\t"
        "add r0, r5, #0\n\t"
        "strh r0, [r1]\n\t"
        "bl sub_8029AC4\n\t"
        "lsl r0, r0, #5\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #0x13\n\t"
        "add r0, r0, r1\n\t"
        "ldr r2, 15f\n\t"
        "ldr r3, [sp, #8]\n\t"
        "str r3, [r2]\n\t"
        "str r0, [r2, #4]\n\t"
        "ldr r5, [sp, #4]\n\t"
        "lsl r0, r5, #4\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #0x18\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2, #8]\n\t"
        "ldr r0, [r2, #8]\n\t"
        "add sp, #0x14\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "13: .4byte 0x0400000A\n"
    "14: .4byte 0x00005A07\n"
    "15: .4byte 0x040000D4\n"
    );
}

/* Same nibble-toggling 32-wide interleaved-row repack loop as
 * `sub_802F7B0` above, feeding a `0x0600D000`-based destination
 * directly from explicit arguments instead of `sub_802F7B0`'s own
 * header-driven setup - see that function's comment for the full
 * account of why this is NAKED. */
NAKED void sub_802F8E8(void *destArg, void *srcArg, s32 rowCount, s32 colCount)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "add r5, r1, #0\n\t"
        "mov r8, r2\n\t"
        "str r3, [sp]\n\t"
        "ldr r6, 1f\n\t"
        "bl sub_8029AC4\n\t"
        "ldr r1, 2f\n\t"
        "add r1, r1, r0\n\t"
        "mov sl, r1\n\t"
        "mov r7, #0\n\t"
        "mov r0, #0\n\t"
        "b 10f\n\t"
        ".align 2, 0\n"
    "1: .4byte 0x0600D000\n"
    "2: .4byte 0xFFFFFE00\n"
    "3:\n\t"
        "mov r3, #0\n\t"
        "mov r1, #0x40\n\t"
        "add r1, r1, r6\n\t"
        "mov ip, r1\n\t"
        "add r0, #1\n\t"
        "mov sb, r0\n\t"
        "cmp r3, r8\n\t"
        "bge 9f\n\t"
        "add r2, r6, #0\n\t"
    "4:\n\t"
        "ldrh r6, [r5]\n\t"
        "add r6, sl\n\t"
        "add r5, #2\n\t"
        "cmp r7, #0\n\t"
        "beq 5f\n\t"
        "ldrb r0, [r4]\n\t"
        "lsr r1, r0, #4\n\t"
        "add r4, #1\n\t"
        "b 6f\n\t"
    "5:\n\t"
        "mov r1, #0xf\n\t"
        "ldrb r0, [r4]\n\t"
        "and r1, r0\n\t"
    "6:\n\t"
        "mov r0, #1\n\t"
        "eor r7, r0\n\t"
        "lsl r1, r1, #0xc\n\t"
        "orr r1, r6\n\t"
        "cmp r3, #0x1f\n\t"
        "bgt 7f\n\t"
        "strh r1, [r2]\n\t"
        "b 8f\n\t"
    "7:\n\t"
        "mov r6, #0xf8\n\t"
        "lsl r6, r6, #3\n\t"
        "add r0, r2, r6\n\t"
        "strh r1, [r0]\n\t"
    "8:\n\t"
        "add r2, #2\n\t"
        "add r3, #1\n\t"
        "cmp r3, r8\n\t"
        "blt 4b\n\t"
    "9:\n\t"
        "mov r6, ip\n\t"
        "mov r0, sb\n\t"
    "10:\n\t"
        "ldr r1, [sp]\n\t"
        "cmp r0, r1\n\t"
        "blt 3b\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}
