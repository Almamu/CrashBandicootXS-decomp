#include "core.h"

/* The fade overlay's (`sub_803472C`/`sub_803487C`, actor_part87.c/
 * actor_part88.c) per-frame driver, called once per frame while the
 * effect is running (caller not yet identified in this pass - out of
 * scope, see docs/matching/issue-63-0x08033ef4-actor.md). Busy-loops
 * (yielding via `sub_8034AA4`/`sub_8034C5C` each iteration - graphics-
 * loading/particle-update helpers just past this file's own raw-asm
 * boundary, `asm/..._34aa4.s`) polling input twice per outer iteration:
 * a confirm press (bit 0) or D-pad-down-with-L (bit 3) of
 * `gUnknown_030007E0.pressed` immediately exits with a "confirm" SFX
 * cue (0x49); otherwise L alone (bit 6, only once `self+0x20`'s one-shot
 * flag is already set) or R alone (bit 7, only once it's clear) plays a
 * "step" cue (0x46) and flips that flag. Every two inner iterations, a
 * 0-15 counter (`self+0x12`'s low 5 bits) ping-pongs a screen-space
 * blend-alpha value up/down by re-applying `self->blend` (already built
 * by `sub_803472C`) to REG_BLDCNT/BLDALPHA, driving the overlay's
 * flicker/pulse animation. Returns 1 if `self+0x20`'s flag is still
 * clear when the loop exits via the confirm branch, else 0.
 *
 * Written as NAKED asm, not plain C: an outer 2-inner-iteration loop
 * with a 16-step ping-pong counter, an alternating one-shot flag, and
 * the `gUnknown_030012BC`/`gUnknown_030007E0` pointers all held live
 * simultaneously across four different `bl` sites (`sub_80007AC`,
 * `PlaySfx` x2, `sub_8034AA4`, `sub_8034C5C`) needs six live values in
 * six different registers (`sb`, `sl`, `r8`, r4-r7) with no spare - the
 * same "many high registers held live across calls inside a loop"
 * shape already NAKED throughout this codebase (e.g.
 * `sub_80309B4`/`sub_8031040`/`sub_80311C4`, actor_part21f.c/23e.c/23f.c)
 * and explicitly flagged as this class of difficulty for this exact
 * function in docs/matching/issue-63-0x08033ef4-actor.md before this
 * pass. Every instruction below is transcribed directly from and
 * checked against the ROM's own disassembly. */
NAKED s32 sub_8034994(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "add r4, r0, #0\n\t"
        "mov r0, #1\n\t"
        "mov sb, r0\n\t"
        "mov r1, #0\n\t"
        "mov r8, r1\n\t"
        "ldrb r2, [r4, #0x12]\n\t"
        "lsl r0, r2, #0x1b\n\t"
        "lsr r6, r0, #0x1b\n\t"
        "ldr r0, 4f\n\t"
        "mov sl, r0\n\t"
        "ldr r7, 5f\n\t"
    "2:\n\t"
        "ldr r0, 6f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80007AC\n\t"
        "mov r1, sl\n\t"
        "ldr r2, [r1]\n\t"
        "lsr r1, r2, #0x10\n\t"
        "mov r0, #1\n\t"
        "and r0, r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "lsr r1, r2, #0x10\n\t"
        "mov r0, #8\n\t"
        "and r0, r0, r1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r5, r0, #0x10\n\t"
        "cmp r5, #0\n\t"
        "beq 7f\n\t"
    "3:\n\t"
        "ldr r0, [r7]\n\t"
        "mov r1, #0x49\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
    "4: .4byte gUnknown_030007E0\n"
    "5: .4byte gUnknown_030012BC\n"
    "6: .4byte gUnknown_03001304\n"
    "7:\n\t"
        "lsr r1, r2, #0x10\n\t"
        "mov r0, #0x40\n\t"
        "and r0, r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "ldr r0, [r4, #0x20]\n\t"
        "cmp r0, #1\n\t"
        "bne 8f\n\t"
        "ldr r0, [r7]\n\t"
        "mov r1, #0x46\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
        "str r5, [r4, #0x20]\n\t"
    "8:\n\t"
        "mov r0, #0x80\n\t"
        "mov r2, sl\n\t"
        "ldrh r2, [r2, #2]\n\t"
        "and r0, r0, r2\n\t"
        "cmp r0, #0\n\t"
        "beq 9f\n\t"
        "ldr r0, [r4, #0x20]\n\t"
        "cmp r0, #0\n\t"
        "bne 9f\n\t"
        "ldr r0, [r7]\n\t"
        "mov r1, #0x46\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
        "mov r0, #1\n\t"
        "str r0, [r4, #0x20]\n\t"
    "9:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8034AA4\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8034C5C\n\t"
        "mov r0, #1\n\t"
        "add r8, r8, r0\n\t"
        "mov r1, r8\n\t"
        "cmp r1, #1\n\t"
        "ble 2b\n\t"
        "mov r2, #0\n\t"
        "mov r8, r2\n\t"
        "mov r0, sb\n\t"
        "cmp r0, #0\n\t"
        "beq 10f\n\t"
        "sub r6, r6, #1\n\t"
        "cmp r6, #0\n\t"
        "bgt 11f\n\t"
        "mov sb, r2\n\t"
        "b 11f\n\t"
    "10:\n\t"
        "add r6, r6, #1\n\t"
        "cmp r6, #0xf\n\t"
        "ble 11f\n\t"
        "mov r1, #1\n\t"
        "mov sb, r1\n\t"
    "11:\n\t"
        "mov r0, #0x1f\n\t"
        "add r1, r6, #0\n\t"
        "and r1, r1, r0\n\t"
        "mov r2, #0x20\n\t"
        "neg r2, r2\n\t"
        "add r0, r2, #0\n\t"
        "ldrb r2, [r4, #0x12]\n\t"
        "and r0, r0, r2\n\t"
        "orr r0, r0, r1\n\t"
        "strb r0, [r4, #0x12]\n\t"
        "ldr r1, 12f\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "str r0, [r1]\n\t"
        "b 2b\n\t"
        ".align 2, 0\n"
    "12: .4byte 0x04000050\n"
    "13:\n\t"
        "mov r1, #0\n\t"
        "ldr r0, [r4, #0x20]\n\t"
        "cmp r0, #0\n\t"
        "bne 14f\n\t"
        "mov r1, #1\n\t"
    "14:\n\t"
        "add r0, r1, #0\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1"
    );
}

asm(".align 2, 0");
