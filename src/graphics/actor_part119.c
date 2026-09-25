#include "core.h"

/* GitHub issue #9/#10: `sub_800C244`, another of the four
 * `self+0x68`-dispatching siblings (docs/matching/issue-9-10-0x0800b8dc-graphics.md)
 * - called from sub_800B8DC's own state 8.
 *
 * Unconditional prelude: if `owner->4` (Y position) is still less than
 * `self->0x64`, the function is a no-op (early return). Otherwise it
 * always fires `sub_800C8BC(self, 0)` + `sub_800C8AC(self, 0)` and
 * re-syncs `owner->4` from `self->0x64` before dispatching further -
 * reads as "once the tracked Y target is reached, latch it and re-fire
 * the anchor triggers once". Two further `self+0x68`-keyed sub-cases
 * follow, gated by whether `owner->0x38` is set:
 *  - `owner->0x38 == 0`: modes 0/1 trigger `sub_800C8CC` with a
 *    constant (1) or toggle `owner->0x28` bit 4 (the same mask-and-or
 *    idiom `sub_800C074` uses) then trigger mode 0; anything else is a
 *    no-op.
 *  - `owner->0x38 != 0`, gated further by `owner->0x30 == 8` and
 *    `owner->0x34 == 0` (the same "blocking condition" pair the Phase 1
 *    doc's field table documents): modes 0/1 both trigger
 *    `sub_800C8BC`/`sub_800C8AC` with mode 3 then play SFX `0x14` -
 *    mode 0 keeps `owner->0x28`'s existing mirror bit (`sub_800C8BC(self,3)`),
 *    mode 1 forces it clear (`sub_800C8BC(self,0)`) before the same
 *    `sub_800C8AC(self,3)` + SFX tail.
 *
 * NAKED transcription, not real C: the same `self`/`owner`
 * multi-field-liveness shape already established as resistant
 * throughout this ROM neighborhood (docs/matching/issue-9-10-0x0800b8dc-graphics.md's
 * "Matching" section) - transcribed directly from the confirmed-correct
 * ROM disassembly. Confirmed byte-exact via the isolated
 * cpp/agbcc/as + objcopy pipeline (the only differences from a direct
 * ROM slice are the `bl` and `.word` relocation sites) and a full clean
 * `make compare`. */

extern void sub_800C8AC(void *self, s32 mode);
extern void sub_800C8BC(void *self, s32 mode);
extern void sub_800C8CC(void *self, s32 mode);
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void *gUnknown_030012BC;

NAKED void sub_800C244(void *selfArg)
{
    asm(
        "push {r4, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r4, #0x70]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "ldr r0, [r4, #0x64]\n\t"
        "cmp r1, r0\n\t"
        "blt 1f\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8BC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8AC\n\t"
        "ldr r2, [r4, #0x70]\n\t"
        "ldr r1, [r4, #0x64]\n\t"
        "str r1, [r2, #4]\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "ldr r0, [r4, #0x68]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "cmp r0, #1\n\t"
        "beq 4f\n\t"
        "b 1f\n\t"
    "3:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #1\n\t"
        "bl sub_800C8CC\n\t"
        "b 1f\n\t"
    "4:\n\t"
        "add r3, r2, #0\n\t"
        "add r3, #0x28\n\t"
        "ldrb r2, [r3]\n\t"
        "lsl r0, r2, #0x1b\n\t"
        "mov r1, #0\n\t"
        "cmp r0, #0\n\t"
        "blt 5f\n\t"
        "mov r1, #1\n\t"
    "5:\n\t"
        "lsl r1, r1, #4\n\t"
        "mov r0, #0x11\n\t"
        "neg r0, r0\n\t"
        "and r0, r0, r2\n\t"
        "orr r0, r0, r1\n\t"
        "strb r0, [r3]\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8CC\n\t"
        "b 1f\n\t"
    "2:\n\t"
        "ldr r0, [r2, #0x30]\n\t"
        "cmp r0, #8\n\t"
        "bne 1f\n\t"
        "ldr r0, [r2, #0x34]\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "ldr r0, [r4, #0x68]\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "cmp r0, #1\n\t"
        "beq 7f\n\t"
        "b 1f\n\t"
    "6:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #3\n\t"
        "bl sub_800C8BC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #3\n\t"
        "bl sub_800C8AC\n\t"
        "ldr r0, =gUnknown_030012BC\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x14\n\t"
        "bl PlaySfx\n\t"
        "b 1f\n\t"
        ".pool\n\t"
    "7:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8BC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #3\n\t"
        "bl sub_800C8AC\n\t"
        "ldr r0, =gUnknown_030012BC\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x14\n\t"
        "bl PlaySfx\n\t"
    "1:\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".pool\n\t"
    );
}
