#include "core.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part21c.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * `sub_80306AC`'s (actor_part21c.c) companion: advances
 * `gUnknown_03001548` by its per-frame delta the same way, but also
 * ramps `gUnknown_03001560` itself toward a fixed target (`0x98`,
 * +-1/frame). Drives a small phase counter (`gUnknown_03001570`) that,
 * on its "armed" phase (0), spawns an effect via `sub_802E62C` centered
 * on a fixed camera offset and advances a per-effect counter
 * (`gUnknown_03001574`) through a small weapon-kind table
 * (`gUnknown_03001568`)'s thresholds, otherwise just decrements the
 * phase. Always re-runs the position-easing helper `sub_8030E08`, and -
 * while `gUnknown_03001554` hasn't crossed its (lower) ceiling
 * `0x31FF` - re-arms the phase from the weapon table and fires the
 * state-3/table-index-0 transition on the tracker object
 * (`gUnknown_03001534`), same shape as `sub_80306AC`. Always finishes
 * with `sub_803171C` (the palette bank-1 flash-color select).
 *
 * Semantics are fully understood (every load/store, branch and call
 * confirmed correct against a plain-C reconstruction that got
 * everything but two statements' evaluation order byte-identical -
 * see `sub_80306AC` in actor_part21c.c for the same idiom applied
 * successfully to this function's simpler sibling). Transcribed as
 * NAKED asm because the remaining gap is a "which operand's address
 * gets materialized first" choice this compiler makes for a plain
 * `*destPtr = *(sourceExpr);` assignment (`gUnknown_03001570 =
 * *(s32*)(table + 0x10);`) that no C-level reordering (pre-declaring
 * the destination pointer earlier, splitting into a temporary) moved -
 * this compiler always computes the RHS's own address load before the
 * LHS's, opposite of the ROM's own build. Mechanical, byte-verified
 * transcription. */
extern s32 GetAnimFrameBaseOffset(void *self);
extern void sub_803171C(void);
extern void sub_8030E08(void);
extern s32 sub_802E62C(s32 x, s32 y, s32 z);

extern s32 gUnknown_03001548;
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001554;
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern void *gUnknown_03001534;
extern s32 gUnknown_03001570;
extern s32 gUnknown_03001574;
extern void *gUnknown_03001568;
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001544;

NAKED void sub_8030734(void)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "ldr r1, 1f\n\t"
        "ldr r4, 2f\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r3, [r4]\n\t"
        "add r0, r0, r3\n\t"
        "str r0, [r1]\n\t"
        "add r2, r1, #0\n\t"
        "cmp r3, #0x98\n\t"
        "bgt 3f\n\t"
        "add r0, r3, #1\n\t"
        "b 4f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001548\n"
    "2: .4byte gUnknown_03001560\n"
    "3:\n\t"
        "sub r0, r3, #1\n\t"
    "4:\n\t"
        "str r0, [r4]\n\t"
        "ldr r5, 5f\n\t"
        "ldr r4, [r5]\n\t"
        "cmp r4, #0\n\t"
        "bne 6f\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, 8f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, 9f\n\t"
        "ldr r1, [r1]\n\t"
        "ldr r3, 10f\n\t"
        "add r1, r1, r3\n\t"
        "ldr r2, [r2]\n\t"
        "sub r2, #0xa\n\t"
        "bl sub_802E62C\n\t"
        "ldr r3, 11f\n\t"
        "ldr r1, [r3]\n\t"
        "add r1, #1\n\t"
        "str r1, [r3]\n\t"
        "ldr r0, 12f\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r2, #8]\n\t"
        "cmp r1, r0\n\t"
        "bne 13f\n\t"
        "str r4, [r3]\n\t"
        "ldr r0, [r2, #0xc]\n\t"
        "b 14f\n\t"
        ".align 2, 0\n"
    "5: .4byte gUnknown_03001570\n"
    "7: .4byte gUnknown_03001540\n"
    "8: .4byte 0xFFFFF325\n"
    "9: .4byte gUnknown_03001544\n"
    "10: .4byte 0x0000516D\n"
    "11: .4byte gUnknown_03001574\n"
    "12: .4byte gUnknown_03001568\n"
    "13:\n\t"
        "ldr r0, [r2, #4]\n\t"
        "b 14f\n\t"
    "6:\n\t"
        "sub r0, r4, #1\n\t"
    "14:\n\t"
        "str r0, [r5]\n\t"
    "15:\n\t"
        "bl sub_8030E08\n\t"
        "ldr r0, 16f\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, 17f\n\t"
        "cmp r1, r0\n\t"
        "bgt 18f\n\t"
        "ldr r1, 19f\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x10]\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, 21f\n\t"
        "mov r5, #0\n\t"
        "str r5, [r0]\n\t"
        "mov r1, #3\n\t"
        "ldr r0, 22f\n\t"
        "str r1, [r0]\n\t"
        "ldr r0, 23f\n\t"
        "str r5, [r0]\n\t"
        "ldr r0, 24f\n\t"
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
        "blt 18f\n\t"
        "str r5, [r4, #8]\n\t"
    "18:\n\t"
        "bl sub_803171C\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "16: .4byte gUnknown_03001554\n"
    "17: .4byte 0x000031FF\n"
    "19: .4byte gUnknown_03001570\n"
    "20: .4byte gUnknown_03001568\n"
    "21: .4byte gUnknown_03001574\n"
    "22: .4byte gUnknown_03001538\n"
    "23: .4byte gUnknown_0300153C\n"
    "24: .4byte gUnknown_03001534\n"
    );
}
