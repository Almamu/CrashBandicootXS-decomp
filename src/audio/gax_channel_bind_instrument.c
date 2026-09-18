#include "core.h"

/* Binds a new instrument/entry to a per-channel voice object (`self`) from
 * `table->0x10[cmd]` and resets most of the voice's envelope/state fields
 * to their defaults, then (if the freshly-bound entry's own first byte is
 * non-zero, meaning it's some kind of "invalid"/placeholder entry) clears
 * the binding back out; finally, if a binding is still in place, records
 * `cmd` into the current song's `+0x34` per-slot table at index
 * `self->0x53`. Channel/voice/song object shapes aren't modeled yet
 * (same situation as the neighboring GAX2 engine internals in this
 * directory) - kept as raw offsets throughout.
 *
 * Written as NAKED asm, not plain C: a real C reconstruction reproduced
 * every field write and the two-separate-zero-constants split (matching
 * `sub_803A104`'s "two named zero temps" idiom in gax_channel_init.c) but
 * the ROM's own register choreography for `self` - reloaded fresh from
 * `ip` (never spilled to a callee-saved register) into a rotating cast of
 * r0/r1/r3 exactly when each group of field writes needs it, with `r3`
 * itself later mutated in place (`adds r3, #0x23`) once its prior value is
 * no longer needed - never came out byte-identical from any C phrasing
 * tried (every version needed one extra callee-saved register, `r5`, that
 * the ROM's version simply doesn't spend). A prior pass had already set
 * this same function aside for the same reason ("several honest attempts,
 * each fix for one instruction's register choice regressed a different
 * one" - see docs/matching/issue-68-0x08039818-audio.md's `sub_803985C`
 * entry); this pass's fresh attempts, informed by the redundant-re-fetch
 * technique that closed `sub_8038FD0`'s cluster, still didn't close this
 * one, confirming it as a genuine gap rather than an easy miss. Mechanical,
 * byte-verified transcription of the ROM's own instructions. */
NAKED void sub_803985C(void *self, void *unused, s32 cmd, void *table)
{
    asm(
        "push {r4, lr}\n\t"
        "mov ip, r0\n\t"
        "add r4, r2, #0\n\t"
        "cmp r4, #0\n\t"
        "beq L985C_0\n\t"
        "ldr r1, [r3, #0x10]\n\t"
        "lsl r0, r4, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, ip\n\t"
        "str r0, [r1, #0x3c]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0\n\t"
        "mov r3, ip\n\t"
        "strh r2, [r3, #0x38]\n\t"
        "mov r0, ip\n\t"
        "add r0, #0x22\n\t"
        "strb r1, [r0]\n\t"
        "strh r2, [r3, #0x3a]\n\t"
        "ldr r0, [r3, #0x3c]\n\t"
        "ldrb r0, [r0, #8]\n\t"
        "add r3, #0x23\n\t"
        "strb r0, [r3]\n\t"
        "mov r0, ip\n\t"
        "strh r2, [r0, #0x36]\n\t"
        "strb r1, [r0, #0x1f]\n\t"
        "add r0, #0x20\n\t"
        "strb r1, [r0]\n\t"
        "mov r0, #0xff\n\t"
        "mov r1, ip\n\t"
        "strb r0, [r1, #0x15]\n\t"
        "ldr r1, [r1, #0x3c]\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x84\n\t"
        "ldrb r0, [r0]\n\t"
        "mov r3, ip\n\t"
        "strb r0, [r3, #0x1e]\n\t"
        "strh r2, [r3, #0x32]\n\t"
        "strh r2, [r3, #0x30]\n\t"
        "ldrb r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "beq L985C_1\n\t"
        "str r2, [r3, #0x3c]\n\t"
        "L985C_1:\n\t"
        "mov r1, ip\n\t"
        "ldr r0, [r1, #0x3c]\n\t"
        "cmp r0, #0\n\t"
        "beq L985C_0\n\t"
        "ldr r0, L985C_2\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "ldr r1, [r0, #0x34]\n\t"
        "cmp r1, #0\n\t"
        "beq L985C_0\n\t"
        "mov r0, ip\n\t"
        "add r0, #0x53\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "strb r4, [r0]\n\t"
        "L985C_0:\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n\t"
        "L985C_2: .4byte gUnknown_03001630\n\t"
    );
}
