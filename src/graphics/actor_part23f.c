#include "core.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * A large per-frame "advance this weapon-kind instance" driver: fires
 * a stride-4 trampoline (`gStaticData_0817C3FC`, indexed by the
 * tracker's own state global `gUnknown_03001538`) via `sub_803AD78`,
 * refreshes the palette-strip animation (`sub_8031744`), and advances
 * `gUnknown_0300153C`'s frame counter. While the tracker's state is
 * nonzero: advances its own anim-frame accumulator (`+8`, by its part-
 * table's `+0x10` halfword) and, once `GetAnimFrameBaseOffset` crosses
 * the current keyframe-table entry's threshold, both re-arms the
 * accumulator against the *next* entry's own delta and sets the "loop"
 * flag (`+0x12`). Always recomputes the BG2 zoom scale/offset the same
 * way `sub_8031040` (actor_part23e.c) does (`sub_8029B2C`/
 * `sub_803ADB4`/`sub_8029E34`), and - only when the tracker's
 * accumulator (`+8`, `>>8`) actually crossed to a new keyframe-table
 * index this frame - re-blits its box via `sub_8030D48` and re-arms
 * the "apply now" latch (`gUnknown_03001524`).
 *
 * Semantics are understood at the level above (every load/store,
 * branch and call accounted for), but this is transcribed as NAKED
 * asm: it shares the exact same accumulator-recompute tail as
 * `sub_8031040`'s NAKED transcription, and the intervening
 * keyframe-table re-arm block hits the same "which operand's address
 * loads first" gcc-2.9 scheduling gap already documented on
 * `sub_8030734` (actor_part21d.c) for a chain of dependent global
 * reads/writes - not pursued further given this file's size.
 * Mechanical, byte-verified transcription. */
extern void *gUnknown_03001534;
extern u8 gStaticData_0817C3FC[];
extern s32 gUnknown_03001538;
extern s32 sub_803AD78(void *fn);
extern void sub_8031744(void);
extern s32 gUnknown_0300153C;
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 gUnknown_03001554;
extern s32 sub_8029B2C(void);
extern s32 gUnknown_03001548;
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 gUnknown_0300154C;
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001550;
extern s32 gUnknown_03001544;
extern void sub_8029E34(s32 arg0);
extern void sub_8030D48(void *arg0);
extern u8 gUnknown_03001524;

NAKED void sub_80311C4(void)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "ldr r5, 1f\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r0, [r0, #8]\n\t"
        "asr r6, r0, #8\n\t"
        "ldr r1, 2f\n\t"
        "ldr r4, 3f\n\t"
        "ldr r0, [r4]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_803AD78\n\t"
        "bl sub_8031744\n\t"
        "ldr r1, 4f\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #1\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, [r4]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "ldr r4, [r5]\n\t"
        "mov r0, #0x10\n\t"
        "ldrsh r1, [r4, r0]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #8]\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r4, #0x12]\n\t"
        "add r0, r4, #0\n\t"
        "bl GetAnimFrameBaseOffset\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "ldr r3, [r4]\n\t"
        "lsl r1, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r1, r3\n\t"
        "mov r3, #4\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "cmp r0, r2\n\t"
        "blt 6f\n\t"
        "mov r3, #6\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "sub r0, r2, r0\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r4, #8]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r4, #8]\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r4, #0x12]\n\t"
    "6:\n\t"
        "ldr r4, 7f\n\t"
        "bl sub_8029B2C\n\t"
        "ldr r1, 8f\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r1]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r4]\n\t"
        "mov r0, #0xe0\n\t"
        "lsl r0, r0, #0x11\n\t"
        "bl sub_803ADB4\n\t"
        "ldr r2, 9f\n\t"
        "ldr r1, 10f\n\t"
        "ldr r1, [r1]\n\t"
        "mul r1, r0\n\t"
        "asr r1, r1, #0xc\n\t"
        "str r1, [r2]\n\t"
        "ldr r2, 11f\n\t"
        "ldr r1, 12f\n\t"
        "ldr r1, [r1]\n\t"
        "mul r0, r1\n\t"
        "asr r0, r0, #0xc\n\t"
        "str r0, [r2]\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8029E34\n\t"
        "ldr r3, [r5]\n\t"
        "ldr r0, [r3, #8]\n\t"
        "asr r4, r0, #8\n\t"
        "cmp r6, r4\n\t"
        "beq 5f\n\t"
        "ldr r1, [r3, #0xc]\n\t"
        "ldr r2, [r3]\n\t"
        "lsl r0, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r2\n\t"
        "mov r1, #2\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r0, r4\n\t"
        "ldr r1, [r3, #4]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8030D48\n\t"
        "ldr r1, 13f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
    "5:\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001534\n"
    "2: .4byte gStaticData_0817C3FC\n"
    "3: .4byte gUnknown_03001538\n"
    "4: .4byte gUnknown_0300153C\n"
    "7: .4byte gUnknown_03001554\n"
    "8: .4byte gUnknown_03001548\n"
    "9: .4byte gUnknown_0300154C\n"
    "10: .4byte gUnknown_03001540\n"
    "11: .4byte gUnknown_03001550\n"
    "12: .4byte gUnknown_03001544\n"
    "13: .4byte gUnknown_03001524\n"
    );
}
