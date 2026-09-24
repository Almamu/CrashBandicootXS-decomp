#include "core.h"

/* GitHub issue #14: 0x08010A0C-0x08010D54, continuing the physics/
 * collision subsystem (game_loop17.c-game_loop27.c). `sub_8010B6C` is
 * this chunk's final and by far largest function - the collision-
 * candidate scan/resolve helper `sub_80106DC` (game_loop23.c) already
 * calls once a frame as `sub_8010B6C(gUnknown_030012D8 + 0x108)`. */

extern void *gUnknown_030012D8;
extern void sub_800E08C(void *neighbor, s32 kind, void *field10, void *field14,
                         s32 field18, s32 field4, s32 field8, s32 field1c,
                         u8 field20, u8 field21, u8 extra);

/* Scans `self`'s neighbor-candidate list - `self+8` onward is an array
 * of 0x24-byte "candidate" records (`neighbor` pointer at `+0`, a
 * position pair at `+4`/`+8`, a `kind` tag at `+0xc`, three more fields
 * at `+0x10`/`+0x14`/`+0x18`/`+0x1c`, two flag bytes at `+0x20`/`+0x21`)
 * where `records[0]` is the previous/seed candidate (already resolved
 * on some earlier call, kept around as the initial "current best") and
 * `records[1..count-1]` are new candidates queued this frame - looking
 * for whichever is closest to the player (`gUnknown_030012D8`) by
 * Y-distance (X-distance as tiebreak, both measured from the player's
 * own position). Any candidate whose Y-distance jumps more than 8 past
 * the running-best Y-distance, or whose own `kind` field is 4, is
 * treated as a forced/priority hit and resolved immediately via
 * `sub_800E08C()` without affecting the running "nearest" comparison;
 * the rest are only compared against each other. After the scan, the
 * overall nearest candidate (`records[0]` itself if nothing else ever
 * qualified) is *also* resolved via `sub_800E08C()` - with its 11th
 * byte argument set to whether any forced/priority hit happened during
 * the scan, unlike every in-loop call, which always passes 0 there -
 * and the list is reset (`count = 0`, `field4 = 0`) for the next frame.
 * `sub_800E08C`'s own 9th-11th (byte) arguments land in this function's
 * own stack frame at `sp+0x10`/`sp+0x14`/`sp+0x18` - those addresses
 * are precomputed once, outside the loop, confirming they're genuinely
 * stack-passed call arguments (AAPCS-style slots 9-11), not separate
 * mystery locals.
 *
 * NAKED, not plain C: every field offset, branch, and call argument
 * here is understood and cross-referenced against the mirror-image
 * writer `sub_8010D54` (right after this issue's own range) and the
 * caller `sub_80106DC` (game_loop23.c) - but the ROM builds nearly
 * every record-field address in both the loop body and the two
 * `sub_800E08C` call sites as a *running pointer*, incremented by
 * 0x24 once per iteration, with up to twelve of them (`r8`/`sb`/`sl`
 * among them) live across a single 0x68-byte stack frame - the same
 * "long, non-uniform stretch of field accesses via running-pointer
 * increments" gap already NAKED-parked (and, for the smaller
 * `sub_800A734` case, later closed) in
 * docs/matching/issue-9-0x08007634-actor.md, at a much larger scale
 * (three times the live-cursor count, on a stack frame twice the
 * size) - beyond what C-level register pins can realistically express.
 * Transcribed instruction-for-instruction from the ROM disassembly
 * instead: the ROM's suffixed Thumb mnemonics (`movs`/`adds`/`subs`/
 * `lsls`/`rsbs`) are written in their suffix-less forms here (`mov`/
 * `add`/`sub`/`lsl`/`neg`), which this project's assembler invocation
 * accepts identically. The single `gUnknown_030012D8` literal load
 * keeps the ROM's own mid-function pool placement (right after the
 * loop's first `sub_800E08C` call site's `b` past it), matching the
 * ROM's own literal-pool split point exactly - verified via a full
 * clean `make compare`, not just an isolated byte-diff. See
 * docs/matching/issue-14-0x08010a0c-graphics.md for the original
 * parking write-up and this follow-up's verification. */
NAKED void sub_8010B6C(void *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x68\n\t"
        "add r7, r0, #0\n\t"
        "ldr r1, [r7]\n\t"
        "cmp r1, #0\n\t"
        "bne 1f\n\t"
        "b 14f\n\t"
    "1:\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r2, [r0]\n\t"
        "str r2, [sp, #0x24]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "str r0, [sp, #0x28]\n\t"
        "mov r3, #0\n\t"
        "str r3, [sp, #0x1c]\n\t"
        "ldr r0, [r7, #8]\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "mov sb, r0\n\t"
        "sub r4, r4, r2\n\t"
        "str r4, [sp, #0x20]\n\t"
        "cmp r4, #0\n\t"
        "bge 2f\n\t"
        "neg r4, r4\n\t"
        "str r4, [sp, #0x20]\n\t"
    "2:\n\t"
        "mov r5, sb\n\t"
        "ldr r0, [sp, #0x28]\n\t"
        "sub r5, r5, r0\n\t"
        "mov sb, r5\n\t"
        "cmp r5, #0\n\t"
        "bge 3f\n\t"
        "neg r5, r5\n\t"
        "mov sb, r5\n\t"
    "3:\n\t"
        "mov r2, #0\n\t"
        "mov ip, r2\n\t"
        "mov r3, #1\n\t"
        "str r3, [sp, #0x2c]\n\t"
        "add r4, r7, #0\n\t"
        "add r4, #8\n\t"
        "str r4, [sp, #0x34]\n\t"
        "add r5, r7, #0\n\t"
        "add r5, #0x14\n\t"
        "str r5, [sp, #0x40]\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x18\n\t"
        "str r0, [sp, #0x44]\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0x1c\n\t"
        "str r2, [sp, #0x48]\n\t"
        "add r3, r7, #0\n\t"
        "add r3, #0x20\n\t"
        "str r3, [sp, #0x4c]\n\t"
        "add r4, #0x1c\n\t"
        "str r4, [sp, #0x50]\n\t"
        "mov r5, sp\n\t"
        "add r5, #0x10\n\t"
        "str r5, [sp, #0x30]\n\t"
        "mov r0, sp\n\t"
        "add r0, #0x14\n\t"
        "str r0, [sp, #0x38]\n\t"
        "mov r2, sp\n\t"
        "add r2, #0x18\n\t"
        "str r2, [sp, #0x3c]\n\t"
        "ldr r3, [sp, #0x2c]\n\t"
        "cmp r3, r1\n\t"
        "bge 13f\n\t"
        "add r4, #0x24\n\t"
        "str r4, [sp, #0x54]\n\t"
        "mov r5, #0x30\n\t"
        "add r5, r5, r7\n\t"
        "mov r8, r5\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x44\n\t"
        "str r0, [sp, #0x58]\n\t"
        "add r1, r7, #0\n\t"
        "add r1, #0x40\n\t"
        "str r1, [sp, #0x5c]\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0x3c\n\t"
        "str r2, [sp, #0x60]\n\t"
        "mov r3, #0x38\n\t"
        "add r3, r3, r7\n\t"
        "mov sl, r3\n\t"
        "sub r4, #0x1c\n\t"
        "str r4, [sp, #0x64]\n\t"
    "4:\n\t"
        "ldr r5, [sp, #0x64]\n\t"
        "ldr r6, [r5]\n\t"
        "ldr r2, [r6]\n\t"
        "ldr r1, [r6, #4]\n\t"
        "ldr r0, [sp, #0x24]\n\t"
        "sub r2, r2, r0\n\t"
        "cmp r2, #0\n\t"
        "bge 5f\n\t"
        "neg r2, r2\n\t"
    "5:\n\t"
        "ldr r3, [sp, #0x28]\n\t"
        "sub r1, r1, r3\n\t"
        "cmp r1, #0\n\t"
        "bge 6f\n\t"
        "neg r1, r1\n\t"
    "6:\n\t"
        "mov r4, sb\n\t"
        "sub r0, r1, r4\n\t"
        "cmp r0, #0\n\t"
        "bge 7f\n\t"
        "neg r0, r0\n\t"
    "7:\n\t"
        "cmp r0, #8\n\t"
        "bgt 8f\n\t"
        "mov r5, sl\n\t"
        "ldr r0, [r5]\n\t"
        "cmp r0, #4\n\t"
        "bne 10f\n\t"
    "8:\n\t"
        "mov r0, sl\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r3, [sp, #0x60]\n\t"
        "ldr r2, [r3]\n\t"
        "ldr r4, [sp, #0x5c]\n\t"
        "ldr r3, [r4]\n\t"
        "ldr r5, [sp, #0x58]\n\t"
        "ldr r0, [r5]\n\t"
        "str r0, [sp]\n\t"
        "mov r0, r8\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r5, [r0, #4]\n\t"
        "str r4, [sp, #4]\n\t"
        "str r5, [sp, #8]\n\t"
        "ldr r4, [sp, #0x54]\n\t"
        "ldr r0, [r4]\n\t"
        "str r0, [sp, #0xc]\n\t"
        "mov r5, r8\n\t"
        "ldrb r0, [r5, #0x1c]\n\t"
        "ldr r4, [sp, #0x30]\n\t"
        "strb r0, [r4]\n\t"
        "ldrb r0, [r5, #0x1d]\n\t"
        "ldr r5, [sp, #0x38]\n\t"
        "strb r0, [r5]\n\t"
        "mov r0, #0\n\t"
        "ldr r4, [sp, #0x3c]\n\t"
        "strb r0, [r4]\n\t"
        "add r0, r6, #0\n\t"
        "bl sub_800E08C\n\t"
        "mov r5, #1\n\t"
        "mov ip, r5\n\t"
        "b 12f\n\t"
        ".align 2, 0\n"
    "9: .4byte gUnknown_030012D8\n"
    "10:\n\t"
        "cmp r1, sb\n\t"
        "blt 11f\n\t"
        "cmp r1, sb\n\t"
        "bne 12f\n\t"
        "ldr r0, [sp, #0x20]\n\t"
        "cmp r2, r0\n\t"
        "bge 12f\n\t"
    "11:\n\t"
        "str r2, [sp, #0x20]\n\t"
        "mov sb, r1\n\t"
        "ldr r1, [sp, #0x2c]\n\t"
        "str r1, [sp, #0x1c]\n\t"
    "12:\n\t"
        "ldr r2, [sp, #0x54]\n\t"
        "add r2, #0x24\n\t"
        "str r2, [sp, #0x54]\n\t"
        "mov r3, #0x24\n\t"
        "add r8, r3\n\t"
        "ldr r4, [sp, #0x58]\n\t"
        "add r4, #0x24\n\t"
        "str r4, [sp, #0x58]\n\t"
        "ldr r5, [sp, #0x5c]\n\t"
        "add r5, #0x24\n\t"
        "str r5, [sp, #0x5c]\n\t"
        "ldr r0, [sp, #0x60]\n\t"
        "add r0, #0x24\n\t"
        "str r0, [sp, #0x60]\n\t"
        "add sl, r3\n\t"
        "ldr r1, [sp, #0x64]\n\t"
        "add r1, #0x24\n\t"
        "str r1, [sp, #0x64]\n\t"
        "ldr r2, [sp, #0x2c]\n\t"
        "add r2, #1\n\t"
        "str r2, [sp, #0x2c]\n\t"
        "ldr r0, [r7]\n\t"
        "cmp r2, r0\n\t"
        "blt 4b\n\t"
    "13:\n\t"
        "ldr r3, [sp, #0x1c]\n\t"
        "lsl r6, r3, #3\n\t"
        "add r6, r6, r3\n\t"
        "lsl r6, r6, #2\n\t"
        "ldr r4, [sp, #0x34]\n\t"
        "add r0, r4, r6\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r5, [sp, #0x40]\n\t"
        "add r1, r5, r6\n\t"
        "ldr r1, [r1]\n\t"
        "ldr r3, [sp, #0x44]\n\t"
        "add r2, r3, r6\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r4, [sp, #0x48]\n\t"
        "add r3, r4, r6\n\t"
        "ldr r3, [r3]\n\t"
        "ldr r5, [sp, #0x4c]\n\t"
        "add r4, r5, r6\n\t"
        "ldr r4, [r4]\n\t"
        "str r4, [sp]\n\t"
        "add r4, r6, r7\n\t"
        "mov r8, r4\n\t"
        "mov r5, r8\n\t"
        "ldr r4, [r5, #0xc]\n\t"
        "ldr r5, [r5, #0x10]\n\t"
        "str r4, [sp, #4]\n\t"
        "str r5, [sp, #8]\n\t"
        "ldr r4, [sp, #0x50]\n\t"
        "add r6, r4, r6\n\t"
        "ldr r4, [r6]\n\t"
        "str r4, [sp, #0xc]\n\t"
        "mov r4, r8\n\t"
        "add r4, #0x28\n\t"
        "ldrb r4, [r4]\n\t"
        "ldr r5, [sp, #0x30]\n\t"
        "strb r4, [r5]\n\t"
        "mov r4, #0x29\n\t"
        "add r8, r4\n\t"
        "mov r5, r8\n\t"
        "ldrb r4, [r5]\n\t"
        "ldr r5, [sp, #0x38]\n\t"
        "strb r4, [r5]\n\t"
        "mov r5, ip\n\t"
        "ldr r4, [sp, #0x3c]\n\t"
        "strb r5, [r4]\n\t"
        "bl sub_800E08C\n\t"
        "mov r0, #0\n\t"
        "str r0, [r7]\n\t"
        "strb r0, [r7, #4]\n\t"
    "14:\n\t"
        "add sp, #0x68\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
/* Trailing byte-padding mismatch fix: the ROM pads the 2-byte gap
 * before the next function (sub_8010D54) with a zero halfword rather
 * than the assembler's default `nop` (`mov r8, r8`) - see
 * matching_decomp_alignment_fix memory. */
asm(".align 2, 0");
