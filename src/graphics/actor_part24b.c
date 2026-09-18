#include "core.h"

/* Same "boss-weapon self" object family as actor_part20.c (see that
 * file's header comment and docs/matching/issue-58-0x08030334-actor.md),
 * and the same 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` AABB-overlap
 * shape as `sub_802DD9C`/`sub_802D7B0` (actor_part75.c/actor_part74.c,
 * see docs/matching/issue-54-actor-d3a8.md) - only runs while the small
 * tracker object's state global (`gUnknown_03001538`) is 2 or 3. Box A:
 * `gStaticData_0817C3D8` (a fixed keyframe-table box) with the boss-
 * weapon's own screen-space accumulators (`gUnknown_03001540`/`0x1544`/
 * `0x1548`, all `>>8`) added into its `x`/`y`/`z`. Box B: `self+0x38`'s
 * own 12-byte vector, with `self`'s own `+0x1c`/`0x20`/`0x24` position
 * (all `>>8`) added into all three of `x`/`y`/`z`, then copied through
 * `sub_800014C`'s self-copy idiom before the 3-axis overlap test.
 * Returns 1 only when all three axes overlap.
 *
 * Written as NAKED asm for the identical register-pressure reasons as
 * `sub_802DD9C`/`sub_802D7B0` - mechanical, byte-verified transcription
 * of the exact same instruction shape, not an inferred guess. */
extern u8 gStaticData_0817C3D8[];
extern s32 gUnknown_03001538;
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001544;
extern s32 gUnknown_03001548;
extern void *sub_800014C(void *dest, void *src, s32 size);

NAKED u8 sub_8031378(void *self)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "sub sp, #0x24\n\t"
        "add r5, r0, #0\n\t"
        "ldr r0, 1f\n\t"
        "ldr r0, [r0]\n\t"
        "sub r0, #2\n\t"
        "cmp r0, #1\n\t"
        "bhi 8f\n\t"
        "mov r1, sp\n\t"
        "ldr r0, 2f\n\t"
        "ldm r0!, {r2, r3, r4}\n\t"
        "stm r1!, {r2, r3, r4}\n\t"
        "ldr r0, 3f\n\t"
        "ldr r3, [r0]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r0, 4f\n\t"
        "ldr r4, [r0]\n\t"
        "asr r4, r4, #8\n\t"
        "ldr r0, 5f\n\t"
        "ldr r2, [r0]\n\t"
        "asr r2, r2, #8\n\t"
        "mov r1, sp\n\t"
        "ldrh r0, [r1]\n\t"
        "add r3, r0, r3\n\t"
        "strh r3, [r1]\n\t"
        "ldrh r0, [r1, #2]\n\t"
        "add r0, r0, r4\n\t"
        "strh r0, [r1, #2]\n\t"
        "ldrh r3, [r1, #4]\n\t"
        "add r2, r3, r2\n\t"
        "strh r2, [r1, #4]\n\t"
        "add r1, sp, #0x18\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x38\n\t"
        "ldm r0!, {r2, r3, r4}\n\t"
        "stm r1!, {r2, r3, r4}\n\t"
        "ldr r0, [r5, #0x1c]\n\t"
        "asr r0, r0, #8\n\t"
        "ldr r3, [r5, #0x20]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r2, [r5, #0x24]\n\t"
        "asr r2, r2, #8\n\t"
        "add r1, sp, #0x18\n\t"
        "ldrh r4, [r1]\n\t"
        "add r0, r4, r0\n\t"
        "strh r0, [r1]\n\t"
        "ldrh r0, [r1, #2]\n\t"
        "add r0, r0, r3\n\t"
        "strh r0, [r1, #2]\n\t"
        "ldrh r5, [r1, #4]\n\t"
        "add r2, r5, r2\n\t"
        "strh r2, [r1, #4]\n\t"
        "add r0, sp, #0xc\n\t"
        "ldm r1!, {r2, r3, r4}\n\t"
        "stm r0!, {r2, r3, r4}\n\t"
        "add r4, sp, #0xc\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r4, #0\n\t"
        "mov r2, #0xc\n\t"
        "bl sub_800014C\n\t"
        "mov r1, sp\n\t"
        "mov r5, #4\n\t"
        "ldrsh r2, [r1, r5]\n\t"
        "mov r0, #4\n\t"
        "ldrsh r3, [r4, r0]\n\t"
        "mov r5, #0xa\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 8f\n\t"
        "mov r5, #0xa\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 8f\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r1, r0]\n\t"
        "mov r5, #2\n\t"
        "ldrsh r3, [r4, r5]\n\t"
        "mov r5, #8\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 8f\n\t"
        "mov r5, #8\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 8f\n\t"
        "mov r0, #0\n\t"
        "ldrsh r2, [r1, r0]\n\t"
        "mov r5, #0\n\t"
        "ldrsh r3, [r4, r5]\n\t"
        "mov r5, #6\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 8f\n\t"
        "mov r4, #6\n\t"
        "ldrsh r0, [r1, r4]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "bgt 9f\n\t"
    "8:\n\t"
        "mov r0, #0\n\t"
        "b 10f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001538\n"
    "2: .4byte gStaticData_0817C3D8\n"
    "3: .4byte gUnknown_03001540\n"
    "4: .4byte gUnknown_03001544\n"
    "5: .4byte gUnknown_03001548\n"
    "9:\n\t"
        "mov r0, #1\n\t"
    "10:\n\t"
        "add sp, #0x24\n\t"
        "pop {r4, r5}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    );
}
