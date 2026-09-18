#include "core.h"

/* Eases `self`'s cached position (`self+0x1c`/`0x20`/`0x24`, the same
 * fields `InitActorPart` caches its `b`/`c`/`d` constructor arguments
 * into, per actor_part50.c) toward a caller-supplied target, with the
 * exact target/mode selected by `self+0x28` ("state"):
 *   - state 0: eases toward `posX`/`posY` offset by a per-frame-counter
 *     (`self+0x44`) lookup into `gStaticData_0816A820` (two different
 *     index strides for the X/Y offsets, producing a scatter/orbit-style
 *     curve), and toward `posZ-0x200`.
 *   - state 1: snaps (no easing) directly to `posX` for the X axis, to
 *     `posY` plus a different table-driven offset for Y, and to
 *     `posZ+0x200` for Z.
 *   - any other state: eases toward `posX`/`posY` directly (no table
 *     offset), and toward `posZ+0x200`.
 * "Easing" is a round-toward-zero divide (by 16 for X/Y, by 4 for Z) of
 * the remaining delta, added back onto the cached position - the ROM's
 * own rsb/lsr/add/asr rounding idiom, same shape as `sub_80070EC` in
 * docs/matching.md.
 *
 * Written as NAKED asm, not plain C: every individual instruction body
 * was already confirmed correct in an earlier plain-C reconstruction
 * (see docs/matching/issue-54-actor-d3a8.md's original parked-pass
 * writeup), but the function hit this project's confirmed, categorical
 * gcc-2.9 r7-pin bug - any explicit `register T x asm("r7")` pin
 * compiles with r7 silently dropped from the prologue/epilogue
 * push/pop list (see docs/matching.md's `sub_8007DBC`/`sub_8006600`
 * entries, and matching_decomp_register_pinning memory point 10),
 * confirmed on this exact function 3+ independent ways. This is a
 * mechanical, byte-verified transcription of the ROM's own instructions
 * (checked instruction-by-instruction against the ROM disassembly,
 * `expected/code_3.s`, before being counted as matched), not an
 * inferred C control-flow guess - the semantics above were already
 * fully understood before this transcription, so none of that risk
 * applies here. */
NAKED void sub_802D3A8(void *selfArg, s32 posX, s32 posY, s32 posZ)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r5, r0, #0\n\t"
        "add r6, r1, #0\n\t"
        "mov ip, r2\n\t"
        "add r7, r3, #0\n\t"
        "ldr r0, [r5, #0x28]\n\t"
        "cmp r0, #0\n\t"
        "bne 7f\n\t"
        "ldr r4, 1f\n\t"
        "ldr r1, [r5, #0x44]\n\t"
        "lsl r0, r1, #2\n\t"
        "mov r3, #0xff\n\t"
        "and r0, r3\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r4\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r0, r2]\n\t"
        "lsl r2, r0, #1\n\t"
        "add r2, r2, r0\n\t"
        "lsl r2, r2, #3\n\t"
        "ldr r0, 2f\n\t"
        "add r2, r2, r0\n\t"
        "add r2, r6, r2\n\t"
        "lsl r1, r1, #1\n\t"
        "and r1, r3\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r4\n\t"
        "mov r0, #0\n\t"
        "ldrsh r1, [r1, r0]\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #1\n\t"
        "ldr r1, 3f\n\t"
        "add r0, r0, r1\n\t"
        "mov r1, ip\n\t"
        "add r4, r1, r0\n\t"
        "ldr r0, 4f\n\t"
        "add r3, r7, r0\n\t"
        "ldr r1, [r5, #0x1c]\n\t"
        "sub r0, r2, r1\n\t"
        "cmp r0, #0\n\t"
        "bge 5f\n\t"
        "add r0, #0xf\n\t"
    "5:\n\t"
        "asr r0, r0, #4\n\t"
        "add r0, r1, r0\n\t"
        "str r0, [r5, #0x1c]\n\t"
        "ldr r1, [r5, #0x20]\n\t"
        "sub r0, r4, r1\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_0816A820\n"
    "2: .4byte 0xFFFFF000\n"
    "3: .4byte 0xFFFFE200\n"
    "4: .4byte 0xFFFFFE00\n"
    "7:\n\t"
        "cmp r0, #1\n\t"
        "bne 14f\n\t"
        "str r6, [r5, #0x1c]\n\t"
        "ldr r2, 8f\n\t"
        "ldr r1, [r5, #0x44]\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "mov r1, #0xff\n\t"
        "and r0, r1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r2\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r2, 9f\n\t"
        "add r0, r0, r2\n\t"
        "add r0, ip\n\t"
        "str r0, [r5, #0x20]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r7, r1\n\t"
        "b 10f\n\t"
        ".align 2, 0\n"
    "8: .4byte gStaticData_0816A820\n"
    "9: .4byte 0xFFFFF600\n"
    "14:\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #2\n\t"
        "add r3, r7, r2\n\t"
        "ldr r1, [r5, #0x1c]\n\t"
        "sub r0, r6, r1\n\t"
        "cmp r0, #0\n\t"
        "bge 11f\n\t"
        "add r0, #0xf\n\t"
    "11:\n\t"
        "asr r0, r0, #4\n\t"
        "add r0, r1, r0\n\t"
        "str r0, [r5, #0x1c]\n\t"
        "ldr r1, [r5, #0x20]\n\t"
        "mov r2, ip\n\t"
        "sub r0, r2, r1\n\t"
    "6:\n\t"
        "cmp r0, #0\n\t"
        "bge 12f\n\t"
        "add r0, #0xf\n\t"
    "12:\n\t"
        "asr r0, r0, #4\n\t"
        "add r0, r1, r0\n\t"
        "str r0, [r5, #0x20]\n\t"
        "ldr r1, [r5, #0x24]\n\t"
        "sub r0, r3, r1\n\t"
        "cmp r0, #0\n\t"
        "bge 13f\n\t"
        "add r0, #3\n\t"
    "13:\n\t"
        "asr r0, r0, #2\n\t"
        "add r0, r1, r0\n\t"
    "10:\n\t"
        "str r0, [r5, #0x24]\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}
