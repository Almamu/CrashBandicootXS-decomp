#include "core.h"

/* GitHub issue #9/#10, tail of the 0x0800B8DC-0x0800D040 cluster (see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): the last raw file
 * in the cluster, `asm/code_3_2_17_cbf4.s` - `sub_800CBF4`,
 * `nullsub_15`, `nullsub_3`, `sub_800CCCC`, `sub_800CCE0`, ROM
 * 0x0800CBF4-0x0800CD00 (contiguous, no gap on either side -
 * `actor_part117.o`'s `sub_800CBD4` ends exactly where this file
 * starts, and `actor_part109.o`'s already-matched `sub_800CD00`
 * begins exactly where this file ends). Closes out the entire
 * 43-function cluster investigation that began with `sub_800B8DC`/
 * `sub_800BD48`. */

extern void *gUnknown_030012B4;
extern void *sub_803AD7C(void *arg0, void *fn);

/* `other` (the second argument - `self`, the first, is never read)
 * shares `struct actor`'s leading header layout (id @8, flags @0xc,
 * table @0x18, same `other->table`-relative `{s16 offset, void *fn}`
 * pair at +0x28/+0x2c that `src/system/game_loop8.c`'s `sub_802400C`
 * already reads via an identical `sub_803AD7C` hit-probe call), but
 * is read at +0x38 too - bigger than the 0x1c-byte `struct actor`, so
 * kept as raw offsets rather than that struct, same reasoning
 * `actor_part27c.c` already documents for its own `other`/`part`.
 *
 * Runs the "flag active + bitmap-set" idiom (`other->0xc |= 1`, then,
 * unless `other`'s id sentinel-checks as `0xFFFF`, sets bit
 * `other->8 & 0x1f` of word `other->8 >> 5` in the
 * `gUnknown_030012B4+0x108` bitmap) up to three times, independently
 * gated: once when the `sub_803AD7C` hit-probe against `other->table`'s
 * own +0x28/+0x2c pair reports *no* hit, once when `other->0xc` bit 3
 * is already set, and once when `other->0x38` is nonzero. This is the
 * exact idiom `actor_part27c.c`'s `sub_8018884` already matches as
 * real C (its own doc comment: "needs several `register asm` pins ...
 * without them this compiler ... folds the ROM's shift-setup pair ...
 * and CSEs away the ROM's second, seemingly redundant reload") - here
 * inlined three times over (the ROM has no `bl` to a shared helper,
 * so the C can't call one either) rather than the one occurrence that
 * already needed that much register-pinning care. NAKED per
 * docs/workflow.md's escape hatch for this documented, already-
 * expensive-once shape; every instruction below is transcribed
 * directly from the ROM disassembly and verified byte-for-byte
 * against `baserom.gba`. */
NAKED void sub_800CBF4(void *self, void *other)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "add r4, r1, #0\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "mov r2, #0x28\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #0x2c]\n\t"
        "bl sub_803AD7C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "mov r0, #1\n\t"
        "ldrb r5, [r4, #0xc]\n\t"
        "orr r0, r5\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "ldr r0, =0x0000FFFF\n\t"
        "ldrh r1, [r4, #8]\n\t"
        "cmp r1, r0\n\t"
        "beq 1f\n\t"
        "ldrh r3, [r4, #8]\n\t"
        "ldr r0, =gUnknown_030012B4\n\t"
        "ldr r2, [r0]\n\t"
        "add r0, r3, #0\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r1, r0, #2\n\t"
        "mov r5, #0x84\n\t"
        "lsl r5, r5, #1\n\t"
        "add r2, r2, r5\n\t"
        "add r2, r2, r1\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r0, r3, r0\n\t"
        "mov r1, #1\n\t"
        "lsl r1, r0\n\t"
        "ldr r0, [r2]\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2]\n\t"
        "1:\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "lsr r0, r1, #3\n\t"
        "mov r2, #1\n\t"
        "and r0, r2\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "add r0, r1, #0\n\t"
        "orr r0, r2\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "ldr r0, =0x0000FFFF\n\t"
        "ldrh r1, [r4, #8]\n\t"
        "cmp r1, r0\n\t"
        "beq 2f\n\t"
        "ldrh r3, [r4, #8]\n\t"
        "ldr r0, =gUnknown_030012B4\n\t"
        "ldr r2, [r0]\n\t"
        "add r0, r3, #0\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r1, r0, #2\n\t"
        "mov r5, #0x84\n\t"
        "lsl r5, r5, #1\n\t"
        "add r2, r2, r5\n\t"
        "add r2, r2, r1\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r0, r3, r0\n\t"
        "mov r1, #1\n\t"
        "lsl r1, r0\n\t"
        "ldr r0, [r2]\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2]\n\t"
        "2:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "mov r0, #1\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "ldr r0, =0x0000FFFF\n\t"
        "ldrh r2, [r4, #8]\n\t"
        "cmp r2, r0\n\t"
        "beq 3f\n\t"
        "ldrh r3, [r4, #8]\n\t"
        "ldr r0, =gUnknown_030012B4\n\t"
        "ldr r2, [r0]\n\t"
        "add r0, r3, #0\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r1, r0, #2\n\t"
        "mov r4, #0x84\n\t"
        "lsl r4, r4, #1\n\t"
        "add r2, r2, r4\n\t"
        "add r2, r2, r1\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r0, r3, r0\n\t"
        "mov r1, #1\n\t"
        "lsl r1, r0\n\t"
        "ldr r0, [r2]\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2]\n\t"
        "3:\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".pool"
    );
}

/* Genuine empty stubs (`bx lr`). */
void nullsub_15(void *self)
{
}

void nullsub_3(void *self)
{
}

extern u8 gStaticData_087E400C[];
extern void sub_800B8A8(void *self, s32 flags);

/* Sets `self+0xc`'s table pointer to `gStaticData_087E400C`, then
 * tail-calls `sub_800B8A8` - same double-set pattern as
 * `sub_8018858`/`sub_8017A78`/`sub_8017FD4`. */
void sub_800CCCC(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gStaticData_087E400C;
    sub_800B8A8(self, flags);
}

extern void sub_800B8C8(void *self);

/* Resets via `sub_800B8C8`, re-points `self+0xc`'s table pointer at
 * `gStaticData_087E400C`, and runs `nullsub_3(self)` - the same
 * "reset via `sub_800B8C8`, re-point `self+0xc`, return `self`"
 * constructor shape already matched for `sub_801886C`/`sub_8018858`/
 * `sub_800CBD4`. */
void *sub_800CCE0(void *selfArg)
{
    u8 *self = selfArg;

    sub_800B8C8(self);
    *(void **)(self + 0xc) = gStaticData_087E400C;
    nullsub_3(self);
    return self;
}
