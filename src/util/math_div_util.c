#include "core.h"
#include "gba/defines.h"

extern void nullsub_8(void);

/* Signed integer division (a / b), truncating toward zero - the
 * generic "atan2-style" math primitive referenced throughout
 * docs/rom_map.md/matching.md (see math_util.c's sub_800090C/
 * sub_8000924/sub_80008F0 wrappers, and sub_8027940's digit-splitting
 * use). Classic shift-and-subtract binary long division, 4 bits at a
 * time: normalizes `divisor`/`bit` up to the dividend's magnitude, then
 * repeatedly tests the top 4 candidate bit positions before shrinking
 * by another nibble.
 *
 * Written as NAKED asm, not plain C: a prior pass's plain-C
 * reconstruction (register-pinned dividend/divisor/quotient/bit/mask/
 * sign to r0/r1/r2/r3/r4/ip, matching the ROM's own choices exactly)
 * got every single instruction in the function BODY byte-identical to
 * the ROM - the only gap was the entry/exit shape: the ROM has two
 * genuinely different prologue/epilogue sequences depending on path
 * (`push {r4}` only, ending in a bare `mov pc, lr`, on the fallthrough
 * `b != 0` path that never calls anything; a separate, minimal
 * `push {lr} ... bl nullsub_8 ... pop {pc}` only on the `b == 0` path)
 * - real per-path register-save minimization (shrink-wrapping) that
 * agbcc, built on gcc 2.9 long before shrink-wrapping existed in
 * mainline gcc, cannot produce from plain C no matter how the `if` is
 * phrased (see docs/matching.md's issue #70 entry for the earlier
 * pass's full account). Since every instruction was already confirmed
 * correct, this is a mechanical, byte-verified transcription of the
 * ROM's own instructions (translated from the disassembler's unified
 * syntax to this project's established NAKED plain/divided syntax,
 * `adds`->`add`/`movs`->`mov`/`rsbs rX,rX,#0`->`neg rX,rX`/etc, local
 * labels renumbered per docs/matching/issue-4-sio-settings-sync.md's
 * convention), not an inferred control-flow guess. */
NAKED s32 sub_803ADB4(s32 a, s32 b)
{
    asm(
        "cmp r1, #0\n\t"
        "beq 13f\n\t"
        "push {r4}\n\t"
        "add r4, r0, #0\n\t"
        "eor r4, r1\n\t"
        "mov ip, r4\n\t"
        "mov r3, #1\n\t"
        "mov r2, #0\n\t"
        "cmp r1, #0\n\t"
        "bpl 1f\n\t"
        "neg r1, r1\n\t"
    "1:\n\t"
        "cmp r0, #0\n\t"
        "bpl 2f\n\t"
        "neg r0, r0\n\t"
    "2:\n\t"
        "cmp r0, r1\n\t"
        "blo 11f\n\t"
        "mov r4, #1\n\t"
        "lsl r4, r4, #0x1c\n\t"
    "3:\n\t"
        "cmp r1, r4\n\t"
        "bhs 4f\n\t"
        "cmp r1, r0\n\t"
        "bhs 4f\n\t"
        "lsl r1, r1, #4\n\t"
        "lsl r3, r3, #4\n\t"
        "b 3b\n\t"
    "4:\n\t"
        "lsl r4, r4, #3\n\t"
    "5:\n\t"
        "cmp r1, r4\n\t"
        "bhs 6f\n\t"
        "cmp r1, r0\n\t"
        "bhs 6f\n\t"
        "lsl r1, r1, #1\n\t"
        "lsl r3, r3, #1\n\t"
        "b 5b\n\t"
    "6:\n\t"
        "cmp r0, r1\n\t"
        "blo 7f\n\t"
        "sub r0, r0, r1\n\t"
        "orr r2, r3\n\t"
    "7:\n\t"
        "lsr r4, r1, #1\n\t"
        "cmp r0, r4\n\t"
        "blo 8f\n\t"
        "sub r0, r0, r4\n\t"
        "lsr r4, r3, #1\n\t"
        "orr r2, r4\n\t"
    "8:\n\t"
        "lsr r4, r1, #2\n\t"
        "cmp r0, r4\n\t"
        "blo 9f\n\t"
        "sub r0, r0, r4\n\t"
        "lsr r4, r3, #2\n\t"
        "orr r2, r4\n\t"
    "9:\n\t"
        "lsr r4, r1, #3\n\t"
        "cmp r0, r4\n\t"
        "blo 10f\n\t"
        "sub r0, r0, r4\n\t"
        "lsr r4, r3, #3\n\t"
        "orr r2, r4\n\t"
    "10:\n\t"
        "cmp r0, #0\n\t"
        "beq 11f\n\t"
        "lsr r3, r3, #4\n\t"
        "beq 11f\n\t"
        "lsr r1, r1, #4\n\t"
        "b 6b\n\t"
    "11:\n\t"
        "add r0, r2, #0\n\t"
        "mov r4, ip\n\t"
        "cmp r4, #0\n\t"
        "bpl 12f\n\t"
        "neg r0, r0\n\t"
    "12:\n\t"
        "pop {r4}\n\t"
        "mov pc, lr\n\t"
    "13:\n\t"
        "push {lr}\n\t"
        "bl nullsub_8\n\t"
        "mov r0, #0\n\t"
        "pop {pc}\n\t"
    );
}
asm(".align 2, 0");

/* Shared divide-by-zero handler for all three division/modulo routines
 * below. ROM bytes are `mov pc, lr` (not `bx lr`, which is what
 * agbcc's plain-C codegen picks for a genuinely empty function body) -
 * written via NAKED + `asm("mov pc, lr")` instead, the same technique
 * `sub_803ADB4` above now uses for its own body. */
NAKED void nullsub_8(void)
{
    asm("mov pc, lr");
}
asm(".align 2, 0");

/* Signed modulo (a % b), result takes the sign of the dividend (C's
 * '%'). Same nibble-at-a-time shift-and-subtract shape as sub_803ADB4,
 * but tracks a *rotated* copy of the bit weight (`ror`, not a plain
 * shift) in a correction mask, so a quotient-bit weight too small to
 * shift meaningfully still leaves a nonzero marker up near bit 31 -
 * used afterwards to add back exactly the fractional divisor amounts
 * that were subtracted from the running remainder but shouldn't have
 * survived, since only the top-level (whole-`divisor`) subtraction is
 * a genuine remainder step.
 *
 * Written as NAKED asm for the same reason as sub_803ADB4: a prior
 * pass's plain-C reconstruction was verified semantically correct
 * (7 % 3 == 1 traced by hand) and structurally mirrored the ROM's
 * register roles, but a plain-C `(v >> n) | (v << (32 - n))` rotate
 * compiles to a shift/shift/or triple instead of the ROM's single
 * `ror` instruction, on top of the same shrink-wrapping-shaped
 * prologue/epilogue gap sub_803ADB4 had. Mechanical, byte-verified
 * transcription of the ROM's own instructions - see sub_803ADB4's
 * comment above for the syntax-translation/label-renumbering
 * convention used. */
NAKED s32 sub_803AE4C(s32 a, s32 b)
{
    asm(
        "mov r3, #1\n\t"
        "cmp r1, #0\n\t"
        "beq 16f\n\t"
        "bpl 1f\n\t"
        "neg r1, r1\n\t"
    "1:\n\t"
        "push {r4}\n\t"
        "push {r0}\n\t"
        "cmp r0, #0\n\t"
        "bpl 2f\n\t"
        "neg r0, r0\n\t"
    "2:\n\t"
        "cmp r0, r1\n\t"
        "blo 14f\n\t"
        "mov r4, #1\n\t"
        "lsl r4, r4, #0x1c\n\t"
    "3:\n\t"
        "cmp r1, r4\n\t"
        "bhs 4f\n\t"
        "cmp r1, r0\n\t"
        "bhs 4f\n\t"
        "lsl r1, r1, #4\n\t"
        "lsl r3, r3, #4\n\t"
        "b 3b\n\t"
    "4:\n\t"
        "lsl r4, r4, #3\n\t"
    "5:\n\t"
        "cmp r1, r4\n\t"
        "bhs 6f\n\t"
        "cmp r1, r0\n\t"
        "bhs 6f\n\t"
        "lsl r1, r1, #1\n\t"
        "lsl r3, r3, #1\n\t"
        "b 5b\n\t"
    "6:\n\t"
        "mov r2, #0\n\t"
        "cmp r0, r1\n\t"
        "blo 7f\n\t"
        "sub r0, r0, r1\n\t"
    "7:\n\t"
        "lsr r4, r1, #1\n\t"
        "cmp r0, r4\n\t"
        "blo 8f\n\t"
        "sub r0, r0, r4\n\t"
        "mov ip, r3\n\t"
        "mov r4, #1\n\t"
        "ror r3, r4\n\t"
        "orr r2, r3\n\t"
        "mov r3, ip\n\t"
    "8:\n\t"
        "lsr r4, r1, #2\n\t"
        "cmp r0, r4\n\t"
        "blo 9f\n\t"
        "sub r0, r0, r4\n\t"
        "mov ip, r3\n\t"
        "mov r4, #2\n\t"
        "ror r3, r4\n\t"
        "orr r2, r3\n\t"
        "mov r3, ip\n\t"
    "9:\n\t"
        "lsr r4, r1, #3\n\t"
        "cmp r0, r4\n\t"
        "blo 10f\n\t"
        "sub r0, r0, r4\n\t"
        "mov ip, r3\n\t"
        "mov r4, #3\n\t"
        "ror r3, r4\n\t"
        "orr r2, r3\n\t"
        "mov r3, ip\n\t"
    "10:\n\t"
        "mov ip, r3\n\t"
        "cmp r0, #0\n\t"
        "beq 11f\n\t"
        "lsr r3, r3, #4\n\t"
        "beq 11f\n\t"
        "lsr r1, r1, #4\n\t"
        "b 6b\n\t"
    "11:\n\t"
        "mov r4, #0xe\n\t"
        "lsl r4, r4, #0x1c\n\t"
        "and r2, r4\n\t"
        "beq 14f\n\t"
        "mov r3, ip\n\t"
        "mov r4, #3\n\t"
        "ror r3, r4\n\t"
        "tst r2, r3\n\t"
        "beq 12f\n\t"
        "lsr r4, r1, #3\n\t"
        "add r0, r0, r4\n\t"
    "12:\n\t"
        "mov r3, ip\n\t"
        "mov r4, #2\n\t"
        "ror r3, r4\n\t"
        "tst r2, r3\n\t"
        "beq 13f\n\t"
        "lsr r4, r1, #2\n\t"
        "add r0, r0, r4\n\t"
    "13:\n\t"
        "mov r3, ip\n\t"
        "mov r4, #1\n\t"
        "ror r3, r4\n\t"
        "tst r2, r3\n\t"
        "beq 14f\n\t"
        "lsr r4, r1, #1\n\t"
        "add r0, r0, r4\n\t"
    "14:\n\t"
        "pop {r4}\n\t"
        "cmp r4, #0\n\t"
        "bpl 15f\n\t"
        "neg r0, r0\n\t"
    "15:\n\t"
        "pop {r4}\n\t"
        "mov pc, lr\n\t"
    "16:\n\t"
        "push {lr}\n\t"
        "bl nullsub_8\n\t"
        "mov r0, #0\n\t"
        "pop {pc}\n\t"
        ".align 2, 0\n"
    );
}

/* Unsigned modulo (a % b) - same shape as sub_803AE4C but with no sign
 * handling at all (used where the caller already knows both operands
 * are non-negative), plus a `dividend < divisor` fast-return path (no
 * `push` at all - the ROM's own per-path register-save minimization,
 * same class of gap that forced sub_803ADB4/sub_803AE4C to NAKED too)
 * the signed version doesn't have. See sub_803AE4C for the correction-
 * mask/rotate technique and the NAKED-transcription rationale. */
NAKED u32 sub_803AF1C(u32 a, u32 b)
{
    asm(
        "cmp r1, #0\n\t"
        "beq 15f\n\t"
        "mov r3, #1\n\t"
        "cmp r0, r1\n\t"
        "bhs 1f\n\t"
        "mov pc, lr\n\t"
    "1:\n\t"
        "push {r4}\n\t"
        "mov r4, #1\n\t"
        "lsl r4, r4, #0x1c\n\t"
    "2:\n\t"
        "cmp r1, r4\n\t"
        "bhs 3f\n\t"
        "cmp r1, r0\n\t"
        "bhs 3f\n\t"
        "lsl r1, r1, #4\n\t"
        "lsl r3, r3, #4\n\t"
        "b 2b\n\t"
    "3:\n\t"
        "lsl r4, r4, #3\n\t"
    "4:\n\t"
        "cmp r1, r4\n\t"
        "bhs 5f\n\t"
        "cmp r1, r0\n\t"
        "bhs 5f\n\t"
        "lsl r1, r1, #1\n\t"
        "lsl r3, r3, #1\n\t"
        "b 4b\n\t"
    "5:\n\t"
        "mov r2, #0\n\t"
        "cmp r0, r1\n\t"
        "blo 6f\n\t"
        "sub r0, r0, r1\n\t"
    "6:\n\t"
        "lsr r4, r1, #1\n\t"
        "cmp r0, r4\n\t"
        "blo 7f\n\t"
        "sub r0, r0, r4\n\t"
        "mov ip, r3\n\t"
        "mov r4, #1\n\t"
        "ror r3, r4\n\t"
        "orr r2, r3\n\t"
        "mov r3, ip\n\t"
    "7:\n\t"
        "lsr r4, r1, #2\n\t"
        "cmp r0, r4\n\t"
        "blo 8f\n\t"
        "sub r0, r0, r4\n\t"
        "mov ip, r3\n\t"
        "mov r4, #2\n\t"
        "ror r3, r4\n\t"
        "orr r2, r3\n\t"
        "mov r3, ip\n\t"
    "8:\n\t"
        "lsr r4, r1, #3\n\t"
        "cmp r0, r4\n\t"
        "blo 9f\n\t"
        "sub r0, r0, r4\n\t"
        "mov ip, r3\n\t"
        "mov r4, #3\n\t"
        "ror r3, r4\n\t"
        "orr r2, r3\n\t"
        "mov r3, ip\n\t"
    "9:\n\t"
        "mov ip, r3\n\t"
        "cmp r0, #0\n\t"
        "beq 10f\n\t"
        "lsr r3, r3, #4\n\t"
        "beq 10f\n\t"
        "lsr r1, r1, #4\n\t"
        "b 5b\n\t"
    "10:\n\t"
        "mov r4, #0xe\n\t"
        "lsl r4, r4, #0x1c\n\t"
        "and r2, r4\n\t"
        "bne 11f\n\t"
        "pop {r4}\n\t"
        "mov pc, lr\n\t"
    "11:\n\t"
        "mov r3, ip\n\t"
        "mov r4, #3\n\t"
        "ror r3, r4\n\t"
        "tst r2, r3\n\t"
        "beq 12f\n\t"
        "lsr r4, r1, #3\n\t"
        "add r0, r0, r4\n\t"
    "12:\n\t"
        "mov r3, ip\n\t"
        "mov r4, #2\n\t"
        "ror r3, r4\n\t"
        "tst r2, r3\n\t"
        "beq 13f\n\t"
        "lsr r4, r1, #2\n\t"
        "add r0, r0, r4\n\t"
    "13:\n\t"
        "mov r3, ip\n\t"
        "mov r4, #1\n\t"
        "ror r3, r4\n\t"
        "tst r2, r3\n\t"
        "beq 14f\n\t"
        "lsr r4, r1, #1\n\t"
        "add r0, r0, r4\n\t"
    "14:\n\t"
        "pop {r4}\n\t"
        "mov pc, lr\n\t"
    "15:\n\t"
        "push {lr}\n\t"
        "bl nullsub_8\n\t"
        "mov r0, #0\n\t"
        "pop {pc}\n\t"
    );
}
