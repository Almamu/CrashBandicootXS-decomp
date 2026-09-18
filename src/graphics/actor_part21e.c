#include "core.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part21c.c/actor_part21d.c - see actor_part20.c's header
 * comment and docs/matching/issue-58-0x08030334-actor.md.
 *
 * `sub_80306AC`/`sub_8030734`'s third sibling: advances
 * `gUnknown_03001548` by its per-frame delta and ramps
 * `gUnknown_03001560` toward `0xb2` the same +-1/frame way. While the
 * phase counter (`gUnknown_03001570`) is armed (0), computes the
 * player's (`gUnknown_03000884`) distance from a target point
 * (`+0x24` axis, offset `+0xa` minus the accumulated position) via
 * `sub_803ADB4`, and - only once that "speed" term is positive -
 * computes a signed Manhattan-style distance in X/Y (`+0x1c`/`+0x20`
 * against `gUnknown_03001540`/`gUnknown_03001544`, scaled by the speed
 * term, `abs`-combined) and, while under a `0x7FF` threshold, spawns an
 * effect via `sub_802E674` (the 4-argument sibling of `sub_8030734`'s
 * `sub_802E62C`) and advances the same `gUnknown_03001574` counter
 * through the weapon table's next threshold slot (`+0x14`/`+0x18`/
 * `+0x10`). Otherwise the phase just decrements. Always re-runs
 * `sub_8030E08` and, past a higher position ceiling (`0x4300`),
 * re-arms the phase from the weapon table (`+4`) and fires the
 * state-2/table-index-0 transition on the tracker object, then always
 * finishes with `sub_803171C`.
 *
 * Semantics are fully understood, but transcribed as NAKED asm: the
 * distance/speed computation keeps the weapon table's phase pointer in
 * `sb` and the raw phase value in `r8` alive across a `sub_803ADB4`
 * call (a real function call, not a leaf helper), the same many-high-
 * register allocation gcc-2.9 difficulty documented throughout this
 * project - not reproducible register-for-register from plain C.
 * Mechanical, byte-verified transcription. */
extern s32 gUnknown_03001548;
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001570;
extern void *gUnknown_03000884;
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001544;
extern s32 sub_802E674(s32 x, s32 y, s32 z, s32 w);
extern s32 gUnknown_03001574;
extern void *gUnknown_03001568;
extern s32 gUnknown_03001554;
extern void *gUnknown_03001534;
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern void sub_8030E08(void);
extern void sub_803171C(void);

NAKED void sub_8030834(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #4\n\t"
        "ldr r1, 1f\n\t"
        "ldr r2, 2f\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r3, [r2]\n\t"
        "add r6, r0, r3\n\t"
        "str r6, [r1]\n\t"
        "cmp r3, #0xb2\n\t"
        "bgt 3f\n\t"
        "add r0, r3, #1\n\t"
        "str r0, [r2]\n\t"
    "3:\n\t"
        "ldr r0, 4f\n\t"
        "mov sb, r0\n\t"
        "ldr r1, [r0]\n\t"
        "mov r8, r1\n\t"
        "cmp r1, #0\n\t"
        "bne 5f\n\t"
        "ldr r0, 6f\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r0, [r4, #0x24]\n\t"
        "add r0, #0xa\n\t"
        "sub r0, r0, r6\n\t"
        "ldr r1, 7f\n\t"
        "bl sub_803ADB4\n\t"
        "add r1, r0, #0\n\t"
        "cmp r1, #0\n\t"
        "ble 12f\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #5\n\t"
        "bl sub_803ADB4\n\t"
        "ldr r1, [r4, #0x1c]\n\t"
        "ldr r2, 8f\n\t"
        "add r1, r1, r2\n\t"
        "ldr r2, 9f\n\t"
        "ldr r7, [r2]\n\t"
        "sub r1, r1, r7\n\t"
        "add r3, r1, #0\n\t"
        "mul r3, r0\n\t"
        "asr r1, r3, #0xc\n\t"
        "mov ip, r1\n\t"
        "ldr r1, [r4, #0x20]\n\t"
        "ldr r2, 10f\n\t"
        "add r1, r1, r2\n\t"
        "ldr r2, 11f\n\t"
        "ldr r5, [r2]\n\t"
        "sub r1, r1, r5\n\t"
        "add r2, r1, #0\n\t"
        "mul r2, r0\n\t"
        "asr r4, r2, #0xc\n\t"
        "asr r3, r3, #0x1f\n\t"
        "mov r1, ip\n\t"
        "eor r1, r3\n\t"
        "sub r1, r1, r3\n\t"
        "asr r2, r2, #0x1f\n\t"
        "add r0, r4, #0\n\t"
        "eor r0, r2\n\t"
        "sub r0, r0, r2\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, 13f\n\t"
        "cmp r1, r0\n\t"
        "bgt 12f\n\t"
        "ldr r1, 14f\n\t"
        "add r0, r7, r1\n\t"
        "ldr r2, 15f\n\t"
        "add r1, r5, r2\n\t"
        "add r2, r6, #0\n\t"
        "sub r2, #0xa\n\t"
        "str r4, [sp]\n\t"
        "mov r3, ip\n\t"
        "bl sub_802E674\n\t"
        "ldr r3, 16f\n\t"
        "ldr r1, [r3]\n\t"
        "add r1, #1\n\t"
        "str r1, [r3]\n\t"
        "ldr r0, 17f\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r2, #0x14]\n\t"
        "cmp r1, r0\n\t"
        "bne 18f\n\t"
        "mov r0, r8\n\t"
        "str r0, [r3]\n\t"
        "ldr r0, [r2, #0x18]\n\t"
        "b 19f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001548\n"
    "2: .4byte gUnknown_03001560\n"
    "4: .4byte gUnknown_03001570\n"
    "6: .4byte gUnknown_03000884\n"
    "7: .4byte 0xFFFFFE56\n"
    "8: .4byte 0x00000CDB\n"
    "9: .4byte gUnknown_03001540\n"
    "10: .4byte 0xFFFFAE93\n"
    "11: .4byte gUnknown_03001544\n"
    "13: .4byte 0x000007FF\n"
    "14: .4byte 0xFFFFF325\n"
    "15: .4byte 0x0000516D\n"
    "16: .4byte gUnknown_03001574\n"
    "17: .4byte gUnknown_03001568\n"
    "18:\n\t"
        "ldr r0, [r2, #0x10]\n\t"
        "mov r2, sb\n\t"
        "str r0, [r2]\n\t"
        "b 12f\n\t"
    "5:\n\t"
        "mov r0, r8\n\t"
        "sub r0, #1\n\t"
    "19:\n\t"
        "mov r1, sb\n\t"
        "str r0, [r1]\n\t"
    "12:\n\t"
        "bl sub_8030E08\n\t"
        "ldr r0, 20f\n\t"
        "ldr r1, [r0]\n\t"
        "mov r0, #0x86\n\t"
        "lsl r0, r0, #7\n\t"
        "cmp r1, r0\n\t"
        "ble 21f\n\t"
        "ldr r1, 22f\n\t"
        "ldr r0, 23f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, 24f\n\t"
        "mov r5, #0\n\t"
        "str r5, [r0]\n\t"
        "mov r1, #2\n\t"
        "ldr r0, 25f\n\t"
        "str r1, [r0]\n\t"
        "ldr r0, 26f\n\t"
        "str r5, [r0]\n\t"
        "ldr r0, 27f\n\t"
        "ldr r4, [r0]\n\t"
        "str r5, [r4, #0xc]\n\t"
        "ldr r0, [r4]\n\t"
        "ldrh r0, [r0]\n\t"
        "mov r1, #0\n\t"
        "strh r0, [r4, #0x10]\n\t"
        "strb r1, [r4, #0x12]\n\t"
        "add r0, r4, #0\n\t"
        "bl GetAnimFrameBaseOffset\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "ldr r3, [r4]\n\t"
        "lsl r1, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r1, r3\n\t"
        "mov r2, #4\n\t"
        "ldrsh r1, [r1, r2]\n\t"
        "cmp r0, r1\n\t"
        "blt 21f\n\t"
        "str r5, [r4, #8]\n\t"
    "21:\n\t"
        "bl sub_803171C\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_03001554\n"
    "22: .4byte gUnknown_03001570\n"
    "23: .4byte gUnknown_03001568\n"
    "24: .4byte gUnknown_03001574\n"
    "25: .4byte gUnknown_03001538\n"
    "26: .4byte gUnknown_0300153C\n"
    "27: .4byte gUnknown_03001534\n"
    );
}
