#include "core.h"
#include "icon_manager.h"

/* Sits right after InitBresenhamLine (ROM 0x08000E6C, in src/util/line_util.c) and
 * before FormatCentiseconds (still raw in asm/code_3_1_3.s). */

/* A text-layout/word-wrap renderer: walks a NUL-terminated string one
 * "token" at a time (`GetWordLength` returns each token's byte length -
 * looks like it splits on word boundaries), drawing each token through
 * the OAM-icon system (`sub_803AD84`, returning the token's pixel
 * width; a second call at a different record slot appears to draw a
 * cursor/highlight) while accumulating a running pixel width against a
 * per-line budget (`box->field_8`). When the running width would
 * overflow, it advances to a new "line" (drawing a newline marker via
 * `sub_803AD80` at a third record slot, and re-drawing the
 * just-measured token at the line's start) and optionally "flushes"
 * (`sub_80006A8` then `sub_8006AAC(gUnknown_03001300)` - the same
 * OAM-shadow-buffer flush pattern used elsewhere) depending on a
 * `mode` parameter (0 = never flush per-token, 1 = flush after every
 * token, 2 = only flush after a line wrap) - and unconditionally
 * flushes once more after the whole string is consumed if `mode != 0`.
 * Recognizes two escape sequences, `/b` (nudge the render Y position
 * down by 4, a half-line break) and `/n` (full newline - same drawing
 * as an overflow-driven line advance), both introduced by a literal
 * `/` byte in the text.
 *
 * `box` (3rd parameter) has only field_0/field_4 (the starting X/Y,
 * copied into the render-target object's 0x110/0x114 fields before the
 * loop starts) and field_8 (the per-line pixel-width budget) used; the
 * render-target object (2nd parameter, `self`) reads 3 different
 * 8-byte record-array entries (+0x18/+0x28/+0x38, 0x10 bytes apart,
 * `{s16, pad, void *}`) off `self->0x130` depending on what it's
 * drawing - see `sub_8006770` (`src/graphics/oam_count.c`) for the
 * same record-array shape.
 *
 * Written as NAKED asm, not plain C: a full C reconstruction (kept in
 * git history) matched the ROM instruction-for-instruction except two
 * small, non-semantic codegen details neither register pins nor
 * barriers could close - (1) the ROM moves `self`/`cursor` into their
 * pinned registers *before* the box/limit incoming-stack-argument spill,
 * but agbcc always spills stack-homed arguments first regardless of C
 * source order; (2) two loop-bound comparisons (`posAccum >= limit`
 * near the top, `lineCount < limit` at the bottom) compile to a single
 * inverted branch in C, but the ROM has a redundant two-instruction
 * "correct-sense compare, branch on true, fall to an unconditional far
 * branch" pair at both spots - most likely a Thumb conditional-branch
 * encoding range (+-256 bytes) artifact from the ROM's original build
 * that a standalone reconstruction never grew large enough to trigger
 * either way. Every instruction below is confirmed byte-identical to
 * the ROM (this doc comment doubles as that derivation) - full NAKED
 * transcription, like this project's other hard-compiler-limitation
 * cases (`src/util/math_div_util.c`'s `nullsub_8`,
 * `src/system/link_cable.c`'s `sub_8001CB8`/`sub_8001DB4`,
 * `src/util/printf_util.c`'s `sub_8000CBC`), is more honest than
 * continuing to chase these two structural gaps through plain C. */
NAKED s32 sub_8000EE4(u8 *textParam, struct icon_manager *selfParam, void *box, s32 limit, s32 mode)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x1c\n\t"
        "mov sl, r0\n\t"
        "mov r8, r1\n\t"
        "str r2, [sp]\n\t"
        "str r3, [sp, #4]\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #0x10]\n\t"
        "ldr r1, [sp, #0x3c]\n\t"
        "cmp r1, #0\n\t"
        "beq 1f\n\t"
        "ldr r4, 5f\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006A90\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006A48\n\t"
    "1:\n\t"
        "ldr r2, [sp]\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r2, [r2, #4]\n\t"
        "mov r3, #0x88\n\t"
        "lsl r3, r3, #1\n\t"
        "add r3, r8\n\t"
        "str r0, [r3]\n\t"
        "mov r1, #0x8a\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r8\n\t"
        "str r2, [r1]\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #8]\n\t"
        "mov r2, #0\n\t"
        "str r2, [sp, #0xc]\n\t"
        "mov r2, sl\n\t"
        "ldrb r0, [r2]\n\t"
        "str r1, [sp, #0x18]\n\t"
        "cmp r0, #0\n\t"
        "bne 2f\n\t"
        "b 14f\n\t"
    "2:\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "ldr r1, [sp, #4]\n\t"
        "cmp r0, r1\n\t"
        "blt 3f\n\t"
        "b 14f\n\t"
    "3:\n\t"
        "str r3, [sp, #0x14]\n\t"
    "4:\n\t"
        "mov r0, sl\n\t"
        "bl GetWordLength\n\t"
        "add r7, r0, #0\n\t"
        "mov r6, sl\n\t"
        "add r2, r6, r7\n\t"
        "mov sl, r2\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #0x2f\n\t"
        "bne 9f\n\t"
        "add r6, #1\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #0x62\n\t"
        "beq 6f\n\t"
        "cmp r0, #0x6e\n\t"
        "beq 7f\n\t"
        "b 8f\n\t"
        ".align 2, 0\n\t"
    "5: .4byte gUnknown_03001300\n\t"
    "6:\n\t"
        "ldr r2, [sp, #0x14]\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r2, [sp, #0x18]\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #4\n\t"
        "ldr r2, [sp, #0x14]\n\t"
        "str r1, [r2]\n\t"
        "ldr r1, [sp, #0x18]\n\t"
        "str r0, [r1]\n\t"
    "7:\n\t"
        "mov r0, #0x98\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r8\n\t"
        "ldr r1, [r0]\n\t"
        "mov r2, #0x38\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r2, [r1, #0x3c]\n\t"
        "mov r1, #0xa\n\t"
        "bl sub_803AD80\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r1, [sp, #0xc]\n\t"
        "add r1, #1\n\t"
        "str r1, [sp, #0xc]\n\t"
    "8:\n\t"
        "ldr r2, [sp, #0x10]\n\t"
        "add r2, r2, r7\n\t"
        "str r2, [sp, #0x10]\n\t"
        "b 13f\n\t"
    "9:\n\t"
        "mov r4, #0x98\n\t"
        "lsl r4, r4, #1\n\t"
        "add r4, r8\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, #0x18\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r3, [r1, #0x1c]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "bl sub_803AD84\n\t"
        "mov sb, r0\n\t"
        "ldr r5, [sp, #8]\n\t"
        "add r5, sb\n\t"
        "ldr r1, [sp]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "cmp r5, r0\n\t"
        "bgt 10f\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, #0x28\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r3, [r1, #0x2c]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "bl sub_803AD84\n\t"
        "str r5, [sp, #8]\n\t"
        "ldr r0, [sp, #0x3c]\n\t"
        "cmp r0, #1\n\t"
        "bne 12f\n\t"
        "b 11f\n\t"
    "10:\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "add r2, #1\n\t"
        "str r2, [sp, #0xc]\n\t"
        "ldr r0, [sp, #4]\n\t"
        "cmp r2, r0\n\t"
        "bge 13f\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, #0x38\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r2, [r1, #0x3c]\n\t"
        "mov r1, #0xa\n\t"
        "bl sub_803AD80\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, #0x28\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r3, [r1, #0x2c]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "bl sub_803AD84\n\t"
        "mov r0, sb\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [sp, #0x3c]\n\t"
        "sub r0, #1\n\t"
        "cmp r0, #1\n\t"
        "bhi 12f\n\t"
    "11:\n\t"
        "bl sub_80006A8\n\t"
        "ldr r0, 16f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006AAC\n\t"
    "12:\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "add r1, r1, r7\n\t"
        "str r1, [sp, #0x10]\n\t"
    "13:\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #0\n\t"
        "beq 14f\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "ldr r0, [sp, #4]\n\t"
        "cmp r2, r0\n\t"
        "bge 14f\n\t"
        "b 4b\n\t"
    "14:\n\t"
        "ldr r1, [sp, #0x3c]\n\t"
        "cmp r1, #0\n\t"
        "beq 15f\n\t"
        "bl sub_80006A8\n\t"
        "ldr r0, 16f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8006AAC\n\t"
    "15:\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "add sp, #0x1c\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n\t"
    "16: .4byte gUnknown_03001300\n\t"
    );
}
