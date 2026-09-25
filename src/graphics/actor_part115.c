#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `sub_800C314`,
 * `sub_800B8DC`'s state-7 callee. Early-outs unless `owner+0x38`
 * (self+0x70, the "owner" object) is set, then dispatches on
 * `self+0x68` (values 0, 1, 6 handled; anything else no-ops):
 *
 * - Case 0: if `self+0x80` bit 0 is set, toggles `owner+0x28` bit 4
 *   (the established mirror-flag convention, see
 *   `src/graphics/actor_part17.c`'s `(s32)(part[0x28] << 27) < 0`
 *   idiom) and triggers `sub_800C8CC(self, 1)`; otherwise just
 *   `sub_800C8CC(self, 6)`. Either way, tails into
 *   `sub_800C8BC(self, 0)` + `sub_800C8AC(self, 0)`.
 * - Case 1: advances `self+0x80` as a wrapping 0-3 counter
 *   (`(self->0x80 + 1) % 4`, a classic gcc truncating-division-by-4
 *   expansion in the ROM), then triggers `sub_800C8BC(self, 2)` +
 *   `sub_800C8AC(self, 2)` + `sub_800C8CC(self, 0)`.
 * - Case 6: same as case 0's toggle but on `owner+0x28` bit 5
 *   instead of bit 4, *plus* case 1's own `self+0x80` counter
 *   advance, then the same `sub_800C8BC`/`sub_800C8AC`/`sub_800C8CC`
 *   trigger triple as case 1.
 *
 * NAKED transcription, not real C: an isolated real-C attempt
 * reproduced the overall dispatch/branch structure exactly (matched
 * byte-for-byte outside the two mirror-flag-toggle blocks), and even
 * reproduced the *established* `(s32)(x << 27) < 0` bit-test idiom
 * from `actor_part17.c` correctly - but could not reproduce the
 * ROM's own instruction *sequencing* for combining the toggled bit
 * back into the byte (ROM computes a 0/1 flag via the shift-test,
 * left-shifts it into bit position, materializes the "clear that bit"
 * mask via a `movs #imm; rsbs r,r,#0` negate-trick, ANDs, then ORs -
 * all as one shared tail). Every C rephrasing tried (nested ternary,
 * a separate `bit = cond ? 0 : 1;` statement, full if/else with
 * separate per-branch stores) got a *different* but still-mismatched
 * instruction order/selection: either the compiler fused the
 * conditional-OR into a single "branch, then conditionally OR a
 * pre-shifted constant" (skipping the explicit 0/1-then-shift
 * ROM does), computed the AND-mask by algebraically reusing an
 * already-live register from an earlier unrelated computation
 * (`self->0x80 & 1`'s leftover `1`) instead of ROM's fresh literal
 * load, or (when forced via a boolean ternary) went fully branchless
 * via an `mvn`/`lsr`-based bit trick. Same underlying gcc-2.9
 * instruction-selection-for-a-single-bit-toggle gap
 * `actor_part27c.c`'s `sub_8018884` doc comment already documents
 * needing heavy register pinning for a related (not identical) bit
 * test/bitmap-set idiom in this same ROM region - not chased further
 * given this function's modest size. Transcribed instruction-for-
 * instruction from the ROM disassembly instead, translating unified-
 * syntax mnemonics to this project's divided/suffix-less convention.
 * Confirmed byte-identical to `baserom.gba` at
 * `0x0800C314`-`0x0800C40C` (248 bytes) via the isolated cpp/agbcc/as
 * + objcopy/cmp pipeline (only the three `bl` relocation sites
 * differ) plus a full clean `rm -rf build && make NON_MATCHING=1
 * report` (no warnings) and `rm -rf build crashbandicootxs.elf
 * crashbandicootxs.gba crashbandicootxs.map && make compare`
 * (`crashbandicootxs.gba: La suma coincide`). */
NAKED void sub_800C314(void *self)
{
    asm(
        "push {r4, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r2, [r4, #0x70]\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "ldr r0, [r4, #0x68]\n\t"
        "cmp r0, #1\n\t"
        "beq 2f\n\t"
        "cmp r0, #1\n\t"
        "bgt 3f\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "b 1f\n\t"
    "3:\n\t"
        "cmp r0, #6\n\t"
        "beq 5f\n\t"
        "b 1f\n\t"
    "4:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x80\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "add r3, r2, #0\n\t"
        "add r3, #0x28\n\t"
        "ldrb r2, [r3]\n\t"
        "lsl r0, r2, #0x1b\n\t"
        "mov r1, #0\n\t"
        "cmp r0, #0\n\t"
        "blt 7f\n\t"
        "mov r1, #1\n\t"
    "7:\n\t"
        "lsl r1, r1, #4\n\t"
        "mov r0, #0x11\n\t"
        "neg r0, r0\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r3]\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #1\n\t"
        "bl sub_800C8CC\n\t"
        "b 8f\n\t"
    "6:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #6\n\t"
        "bl sub_800C8CC\n\t"
    "8:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8BC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8AC\n\t"
        "b 1f\n\t"
    "2:\n\t"
        "add r3, r4, #0\n\t"
        "add r3, #0x80\n\t"
        "ldr r2, [r3]\n\t"
        "add r1, r2, #1\n\t"
        "add r0, r1, #0\n\t"
        "cmp r1, #0\n\t"
        "bge 9f\n\t"
        "add r0, r2, #4\n\t"
    "9:\n\t"
        "asr r0, r0, #2\n\t"
        "lsl r0, r0, #2\n\t"
        "sub r0, r1, r0\n\t"
        "str r0, [r3]\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #2\n\t"
        "bl sub_800C8BC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #2\n\t"
        "bl sub_800C8AC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8CC\n\t"
        "b 1f\n\t"
    "5:\n\t"
        "add r3, r2, #0\n\t"
        "add r3, #0x28\n\t"
        "ldrb r2, [r3]\n\t"
        "lsl r0, r2, #0x1a\n\t"
        "mov r1, #0\n\t"
        "cmp r0, #0\n\t"
        "blt 10f\n\t"
        "mov r1, #1\n\t"
    "10:\n\t"
        "lsl r1, r1, #5\n\t"
        "mov r0, #0x21\n\t"
        "neg r0, r0\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r3]\n\t"
        "add r3, r4, #0\n\t"
        "add r3, #0x80\n\t"
        "ldr r2, [r3]\n\t"
        "add r1, r2, #1\n\t"
        "add r0, r1, #0\n\t"
        "cmp r1, #0\n\t"
        "bge 11f\n\t"
        "add r0, r2, #4\n\t"
    "11:\n\t"
        "asr r0, r0, #2\n\t"
        "lsl r0, r0, #2\n\t"
        "sub r0, r1, r0\n\t"
        "str r0, [r3]\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #2\n\t"
        "bl sub_800C8BC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #2\n\t"
        "bl sub_800C8AC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8CC\n\t"
    "1:\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
