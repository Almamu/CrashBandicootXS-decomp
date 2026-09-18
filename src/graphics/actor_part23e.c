#include "core.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * A large "spawn/arm this weapon-kind instance" setup routine: resets
 * the ramp/velocity globals, fires the tracker object's state-1/
 * table-index-0 transition, seeds the position accumulators
 * (`gUnknown_03001540`/`gUnknown_03001544`/`gUnknown_03001548`) from
 * its own three arguments, looks up a per-kind keyframe-table record
 * (`gStaticData_0817C2D0`, indexed by both `gUnknown_03001564` - the
 * caller object `sub_8030F88` stashed - and this function's own first
 * argument) and copies several of its fields into
 * `gUnknown_03001570`/`gUnknown_0300156C`, resets the DMA-refresh/
 * palette-strip counters, recomputes the BG2 zoom scale/offset via
 * `sub_8029B2C`/`sub_803ADB4`/`sub_8029E34`, blits the tracker's
 * current keyframe-table box via `sub_8030D48`, sets DISPCNT's bit10,
 * recomputes the BG2 affine matrix (`sub_80312C4`), and finally queues
 * a palette-strip DMA transfer (`QueueVramDmaTransfer`).
 *
 * Semantics are understood at the level above, but this is transcribed
 * as NAKED asm: the three incoming position arguments and the
 * per-kind table pointer all stay live in `r8`/`sb`/`sl` across
 * several real function calls (`sub_8029B2C`, `sub_803ADB4`,
 * `sub_8029E34`), the same many-high-register allocation gcc-2.9
 * difficulty documented throughout this project. Mechanical,
 * byte-verified transcription. */
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern void *gUnknown_03001534;
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001544;
extern s32 gUnknown_03001548;
extern void *gUnknown_03001568;
extern s32 gUnknown_03001564;
extern u8 gStaticData_0817C2D0[];
extern s32 gUnknown_03001570;
extern s32 gUnknown_0300156C;
extern s32 gUnknown_03001574;
extern u8 gUnknown_03001524;
extern s32 gUnknown_03001520;
extern s32 gUnknown_03001554;
extern s32 sub_8029B2C(void);
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 gUnknown_0300154C;
extern s32 gUnknown_03001550;
extern void sub_8029E34(s32 arg0);
extern void sub_8030D48(void *arg0);
extern void sub_80312C4(void);
extern s32 gUnknown_03001578;
extern u8 gStaticData_0817C378[];
extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);

NAKED void sub_8031040(s32 a, s32 b, s32 c, s32 d)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "add r6, r0, #0\n\t"
        "add r5, r1, #0\n\t"
        "mov sb, r2\n\t"
        "mov sl, r3\n\t"
        "ldr r1, 1f\n\t"
        "mov r0, #0x66\n\t"
        "str r0, [r1]\n\t"
        "mov r7, #0\n\t"
        "ldr r0, 2f\n\t"
        "mov r1, #1\n\t"
        "str r1, [r0]\n\t"
        "ldr r0, 3f\n\t"
        "str r7, [r0]\n\t"
        "ldr r2, 4f\n\t"
        "ldr r4, [r2]\n\t"
        "str r7, [r4, #0xc]\n\t"
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
        "mov r3, #4\n\t"
        "ldrsh r1, [r1, r3]\n\t"
        "cmp r0, r1\n\t"
        "blt 5f\n\t"
        "str r7, [r4, #8]\n\t"
    "5:\n\t"
        "ldr r0, 6f\n\t"
        "mov r8, r0\n\t"
        "lsl r0, r5, #2\n\t"
        "add r0, r0, r5\n\t"
        "mov r1, r8\n\t"
        "str r0, [r1]\n\t"
        "mov r2, sb\n\t"
        "lsl r0, r2, #1\n\t"
        "ldr r3, 7f\n\t"
        "str r0, [r3]\n\t"
        "ldr r5, 8f\n\t"
        "mov r0, #0xa0\n\t"
        "lsl r0, r0, #8\n\t"
        "add r0, sl\n\t"
        "str r0, [r5]\n\t"
        "ldr r3, 9f\n\t"
        "ldr r0, 10f\n\t"
        "ldr r0, [r0]\n\t"
        "lsl r1, r0, #3\n\t"
        "sub r1, r1, r0\n\t"
        "lsl r1, r1, #2\n\t"
        "lsl r0, r6, #3\n\t"
        "sub r0, r0, r6\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r2, 11f\n\t"
        "add r0, r0, r2\n\t"
        "add r1, r1, r0\n\t"
        "str r1, [r3]\n\t"
        "ldr r2, 12f\n\t"
        "ldr r0, [r1, #0xc]\n\t"
        "str r0, [r2]\n\t"
        "ldr r2, 13f\n\t"
        "ldr r0, [r1]\n\t"
        "str r0, [r2]\n\t"
        "ldr r0, 14f\n\t"
        "str r7, [r0]\n\t"
        "ldr r0, 15f\n\t"
        "mov r1, #1\n\t"
        "strb r1, [r0]\n\t"
        "ldr r0, 16f\n\t"
        "str r7, [r0]\n\t"
        "ldr r4, 17f\n\t"
        "bl sub_8029B2C\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r5]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r4]\n\t"
        "mov r0, #0xe0\n\t"
        "lsl r0, r0, #0x11\n\t"
        "bl sub_803ADB4\n\t"
        "ldr r2, 18f\n\t"
        "mov r3, r8\n\t"
        "ldr r1, [r3]\n\t"
        "mul r1, r0\n\t"
        "asr r1, r1, #0xc\n\t"
        "str r1, [r2]\n\t"
        "ldr r2, 19f\n\t"
        "ldr r3, 7f\n\t"
        "ldr r1, [r3]\n\t"
        "mul r0, r1\n\t"
        "asr r0, r0, #0xc\n\t"
        "str r0, [r2]\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8029E34\n\t"
        "ldr r0, 4f\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r3, [r2, #8]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r1, [r2, #0xc]\n\t"
        "ldr r4, [r2]\n\t"
        "lsl r0, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r4\n\t"
        "mov r1, #2\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r2, #4]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8030D48\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #0x13\n\t"
        "ldrh r0, [r2]\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #3\n\t"
        "add r1, r3, #0\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r2]\n\t"
        "bl sub_80312C4\n\t"
        "ldr r0, 20f\n\t"
        "str r7, [r0]\n\t"
        "ldr r0, 21f\n\t"
        "ldr r1, 22f\n\t"
        "mov r2, #0x20\n\t"
        "mov r3, #0x10\n\t"
        "bl QueueVramDmaTransfer\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001560\n"
    "2: .4byte gUnknown_03001538\n"
    "3: .4byte gUnknown_0300153C\n"
    "4: .4byte gUnknown_03001534\n"
    "6: .4byte gUnknown_03001540\n"
    "7: .4byte gUnknown_03001544\n"
    "8: .4byte gUnknown_03001548\n"
    "9: .4byte gUnknown_03001568\n"
    "10: .4byte gUnknown_03001564\n"
    "11: .4byte gStaticData_0817C2D0\n"
    "12: .4byte gUnknown_03001570\n"
    "13: .4byte gUnknown_0300156C\n"
    "14: .4byte gUnknown_03001574\n"
    "15: .4byte gUnknown_03001524\n"
    "16: .4byte gUnknown_03001520\n"
    "17: .4byte gUnknown_03001554\n"
    "18: .4byte gUnknown_0300154C\n"
    "19: .4byte gUnknown_03001550\n"
    "20: .4byte gUnknown_03001578\n"
    "21: .4byte gStaticData_0817C378\n"
    "22: .4byte 0x05000020\n"
    );
}
