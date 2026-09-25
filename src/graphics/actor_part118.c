#include "core.h"

/* GitHub issue #9/#10: one of the four `self+0x68`-dispatching siblings
 * the Phase 1/2 investigation (docs/matching/issue-9-10-0x0800b8dc-graphics.md)
 * flagged as the next highest-value target in the 0x0800B8DC-0x0800D040
 * cluster - independently called from several of sub_800B8DC's own
 * `self+0x74` dispatch states (2, 13, 15, 18).
 *
 * A small 4-case dispatcher keyed off `self+0x68` (the same sub-state
 * byte `sub_800B8DC`'s own state machine reads/writes, and the field
 * `sub_800C40C` also dispatches on): modes 0 and 4 share an identical
 * "mirror-aware position gate" (compares `owner`'s X position against
 * `self+0x10`/`self+0x14` bounds, direction picked by `owner+0x28` bit
 * 4, the established mirror-flag convention) before triggering
 * `sub_800C8CC` with a different constant (1 vs 6) and always calling
 * `sub_800C8BC(self, 0)`. Modes 1 and 6 both toggle `owner+0x28` bit 4
 * (the "flag active + bitmap-set" idiom's own bit, an unconditional
 * flip via the mask-and-or idiom, not the position gate) when
 * `owner+0x38` is set, then trigger `sub_800C8CC`/`sub_800C8BC` with
 * different constants; mode 1 additionally clamps `owner+0x30` against
 * a keyframe-record byte (`owner+0x20`-table[`owner+0x2d`]+0x16, the
 * `sub_800D040`-style 28-byte-stride record convention) when
 * `self+0x6c == 0xf`. Any other mode is a silent no-op.
 *
 * NAKED transcription, not real C: this exact `self`/`owner`
 * multi-field shape (several simultaneously-live fields across `bl`
 * calls, plus a shared-retest CFG diamond in the position-gate blocks
 * that a plain if/else or goto reconstruction couldn't reproduce
 * byte-for-byte without fighting gcc 2.9's own jump-threading) matches
 * this ROM neighborhood's already-documented resistant shape (see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md's own "Matching"
 * section for `sub_800B8DC`/`sub_800BD48`) - transcribed directly from
 * the confirmed-correct ROM disassembly instead. Confirmed byte-exact
 * via the isolated cpp/agbcc/as + objcopy pipeline (the only
 * differences from a direct ROM slice are the six `bl` relocation
 * sites) and a full clean `make compare`. */

extern void sub_800C8AC(void *self, s32 mode);
extern void sub_800C8BC(void *self, s32 mode);
extern void sub_800C8CC(void *self, s32 mode);

NAKED void sub_800C074(void *selfArg)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r4, #0x68]\n\t"
        "cmp r0, #1\n\t"
        "beq 1f\n\t"
        "cmp r0, #1\n\t"
        "bgt 2f\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "b 4f\n\t"
    "2:\n\t"
        "cmp r0, #4\n\t"
        "beq 5f\n\t"
        "cmp r0, #6\n\t"
        "beq 6f\n\t"
        "b 4f\n\t"
    "3:\n\t"
        "ldr r2, [r4, #0x70]\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r3, r0, #0x1b\n\t"
        "cmp r3, #0\n\t"
        "bge 7f\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "cmp r1, r0\n\t"
        "blt 8f\n\t"
    "7:\n\t"
        "cmp r3, #0\n\t"
        "blt 4f\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r0, [r4, #0x14]\n\t"
        "cmp r1, r0\n\t"
        "ble 4f\n\t"
    "8:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #1\n\t"
        "b 9f\n\t"
    "1:\n\t"
        "ldr r1, [r4, #0x70]\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "add r3, r1, #0\n\t"
        "add r3, #0x28\n\t"
        "ldrb r2, [r3]\n\t"
        "lsl r0, r2, #0x1b\n\t"
        "mov r1, #0\n\t"
        "cmp r0, #0\n\t"
        "blt 10f\n\t"
        "mov r1, #1\n\t"
    "10:\n\t"
        "lsl r0, r1, #4\n\t"
        "mov r1, #0x11\n\t"
        "neg r1, r1\n\t"
        "and r1, r1, r2\n\t"
        "orr r1, r1, r0\n\t"
        "strb r1, [r3]\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8CC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #1\n\t"
        "bl sub_800C8BC\n\t"
        "ldr r0, [r4, #0x6c]\n\t"
        "cmp r0, #0xf\n\t"
        "bne 4f\n\t"
        "ldr r3, [r4, #0x70]\n\t"
        "mov r4, #8\n\t"
        "ldr r0, [r3, #0x20]\n\t"
        "add r2, r3, #0\n\t"
        "add r2, #0x2d\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r5, [r2]\n\t"
        "lsl r0, r5, #3\n\t"
        "sub r0, r0, r5\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "cmp r4, r0\n\t"
        "blt 11f\n\t"
        "sub r4, r0, #1\n\t"
    "11:\n\t"
        "str r4, [r3, #0x30]\n\t"
        "b 4f\n\t"
    "5:\n\t"
        "ldr r2, [r4, #0x70]\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r3, r0, #0x1b\n\t"
        "cmp r3, #0\n\t"
        "bge 12f\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "cmp r1, r0\n\t"
        "blt 13f\n\t"
    "12:\n\t"
        "cmp r3, #0\n\t"
        "blt 4f\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r0, [r4, #0x14]\n\t"
        "cmp r1, r0\n\t"
        "ble 4f\n\t"
    "13:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #6\n\t"
    "9:\n\t"
        "bl sub_800C8CC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8BC\n\t"
        "b 4f\n\t"
    "6:\n\t"
        "ldr r1, [r4, #0x70]\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "add r3, r1, #0\n\t"
        "add r3, #0x28\n\t"
        "ldrb r2, [r3]\n\t"
        "lsl r0, r2, #0x1b\n\t"
        "mov r1, #0\n\t"
        "cmp r0, #0\n\t"
        "blt 14f\n\t"
        "mov r1, #1\n\t"
    "14:\n\t"
        "lsl r1, r1, #4\n\t"
        "mov r0, #0x11\n\t"
        "neg r0, r0\n\t"
        "and r0, r0, r2\n\t"
        "orr r0, r0, r1\n\t"
        "strb r0, [r3]\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #4\n\t"
        "bl sub_800C8CC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #1\n\t"
        "bl sub_800C8BC\n\t"
    "4:\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
