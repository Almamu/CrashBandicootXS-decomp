#include "core.h"

/* Sits between FormatCentiseconds (ROM 0x0800106C, in src/util/time_util.c) and
 * LoadTaggedAsset (still raw in asm/code_3_1_5.s). */

extern void sub_80006A8(void);
extern s32 sub_80007AC(void *arg0);
extern void *gUnknown_03001304;
extern u16 gUnknown_030007E0;

/* Polls input (via sub_80006A8/sub_80007AC, the same VBlank-wait-then-
 * update-keys pair used elsewhere) until a button matching `mask`'s bit
 * 0 (confirm) or bit 3 (cancel) is newly pressed, or (if `count != 0`)
 * until `count` polls have elapsed. Returns 0 only if a cancel (bit 3)
 * press was seen; every other exit path (confirm press, or hitting the
 * poll-count limit) returns 1. `checkButtons` (nonzero) enables the
 * per-poll button checks at all - with it clear and a nonzero `count`,
 * this is just a `count`-poll delay that always returns 1; with it
 * clear and `count == 0`, it returns 1 immediately without polling at
 * all. Reads the "newly pressed this frame" keys (`gUnknown_030007E0`'s
 * companion u16 at +2, see `sub_80007AC`'s own notes in
 * `src/system/irq.c`) fresh each poll (no caching across polls, since
 * `sub_80007AC`'s call in between could change it). The
 * `sub_80007AC(gUnknown_03001304)` calls pass an argument the real,
 * already-matched `sub_80007AC(void)` (in `src/system/irq.c`) never
 * reads - same "ROM sets up an arg the callee ignores" shape as
 * `sub_80006A8` itself.
 *
 * Written as NAKED asm, not plain C: a full C reconstruction (kept in
 * git history) matched the ROM instruction-for-instruction except one
 * 4-byte residual - the count-limited loop's `if (keys & 1)` bit-test
 * compiled as `bne done; b continue`, where the ROM has the opposite
 * sense, `beq continue; b done` (same two instructions, same size, just
 * inverted) - every rephrasing tried (positive/negative sense, explicit
 * goto-only chains, reordering this block relative to the unlimited-loop
 * variant) produced identical output, pointing at a fixed gcc-2.9
 * canonicalization for this exact shape rather than anything reachable
 * from C source here. Every instruction below is confirmed
 * byte-identical to the ROM (the paragraph above is that derivation) -
 * full NAKED transcription, like this project's other
 * hard-compiler-limitation cases (`src/util/math_div_util.c`'s
 * `nullsub_8`, `src/system/link_cable.c`'s `sub_8001CB8`/`sub_8001DB4`,
 * `src/util/printf_util.c`'s `sub_8000CBC`,
 * `src/graphics/text_layout.c`'s `sub_8000EE4`), is more honest than
 * continuing to chase this one branch sense through plain C. */
NAKED s32 sub_80010E0(s32 count, u8 checkButtons, s32 mask)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "add r6, r0, #0\n\t"
        "add r7, r2, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r4, r1, #0x18\n\t"
        "mov r0, #1\n\t"
        "mov r8, r0\n\t"
        "cmp r6, #0\n\t"
        "beq 6f\n\t"
        "mov r5, #0\n\t"
        "b 3f\n\t"
    "1:\n\t"
        "mov r0, #8\n\t"
        "and r1, r0\n\t"
        "cmp r1, #0\n\t"
        "bne 8f\n\t"
    "2:\n\t"
        "add r5, #1\n\t"
    "3:\n\t"
        "cmp r5, r6\n\t"
        "bge 9f\n\t"
        "bl sub_80006A8\n\t"
        "ldr r0, 4f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80007AC\n\t"
        "ldr r0, 5f\n\t"
        "add r1, r7, #0\n\t"
        "ldrh r0, [r0, #2]\n\t"
        "and r1, r0\n\t"
        "cmp r4, #0\n\t"
        "beq 2b\n\t"
        "mov r0, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 1b\n\t"
        "b 9f\n\t"
        ".align 2, 0\n\t"
    "4: .4byte gUnknown_03001304\n\t"
    "5: .4byte gUnknown_030007E0\n\t"
    "6:\n\t"
        "cmp r4, #0\n\t"
        "beq 9f\n\t"
    "7:\n\t"
        "bl sub_80006A8\n\t"
        "ldr r0, 10f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80007AC\n\t"
        "ldr r0, 11f\n\t"
        "add r1, r7, #0\n\t"
        "ldrh r0, [r0, #2]\n\t"
        "and r1, r0\n\t"
        "mov r0, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 9f\n\t"
        "mov r0, #8\n\t"
        "and r1, r0\n\t"
        "cmp r1, #0\n\t"
        "beq 7b\n\t"
    "8:\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
    "9:\n\t"
        "mov r0, r8\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n\t"
    "10: .4byte gUnknown_03001304\n\t"
    "11: .4byte gUnknown_030007E0\n\t"
    );
}
