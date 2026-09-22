#include "core.h"
#include "actor_anim.h"

extern s32 gUnknown_03001384;
extern s32 gUnknown_03001388;
extern s32 gUnknown_0300138C;
extern s32 gUnknown_03001390;
extern s32 gUnknown_03000878;
extern void *gUnknown_030012C0;

extern s32 sub_802A4E0(void);
extern void sub_8022CA0(void *arg0);
extern void sub_802AB34(void);

/* Re-bases the category's secondary tick counter from `arg0` (net of
 * `sub_802A4E0`'s current Q8.8 offset), resets the active-instance
 * counters, and re-syncs the frame-tick snapshot for a freshly
 * (re)selected category. */
void sub_8029748(s32 arg0)
{
    gUnknown_03000878 = arg0 - sub_802A4E0();
    gUnknown_03001384 = 0;
    gUnknown_03001388 = 0;
    sub_8022CA0(gUnknown_030012C0);
    gUnknown_03001390 = gUnknown_0300138C;
    sub_802AB34();
}

extern s32 gUnknown_03001380;

/* True once the running active-instance count reaches the current
 * category's `unknown_20` threshold. The cast to `s32` matches the
 * ROM's own signed comparison (`blt`) - `unknown_20` is declared `u32`
 * in actor_anim.h (its sign isn't otherwise pinned down), and the
 * unsigned usual-arithmetic-conversion comparison that produces
 * compiles to the unsigned `bcc` instead (see docs/workflow.md
 * step 3). */
s32 sub_8029794(void)
{
    return gUnknown_03001384 >= (s32)gStaticData_08175558[gUnknown_03001380].unknown_20;
}

extern s32 gUnknown_030013B0;
extern s32 gUnknown_030013A4;
extern void *gUnknown_03001394;
extern u8 gUnknown_030013B8;
extern u8 gUnknown_030013B9;
extern s32 gUnknown_030013A0;
extern void *gUnknown_0300087C;
extern s32 gUnknown_03001398;
extern s32 gUnknown_0300139C;
extern u8 gUnknown_030013BA;

extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* NAKED - kicks off a DMA copy of `gUnknown_030013A0` bytes from the
 * current "console"/text-plane cursor cell into VRAM (one of three
 * fixed destination strategies depending on the `gUnknown_030013B8`/
 * `gUnknown_030013B9` mode bytes), then arms `gUnknown_030013BA` so a
 * caller can poll for completion. Fully understood; every plain-C
 * attempt (register-variable pins on the `addr`/`off`/`gUnknown_03001394`
 * triple, an explicit dead-read island for the `sub_803AD88` trampoline
 * target) reproduced the right instructions but never the ROM's exact
 * register-reuse pattern for the address-computation prologue (`off`
 * computed into a register distinct from `addr` until the final
 * combine, with `gUnknown_03001394`'s address kept live across the
 * whole computation) - see docs/workflow.md's NAKED-transcription
 * escape hatch. */
NAKED void sub_80297C8(void)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "ldr r2, 1f\n\t"
        "ldr r0, 2f\n\t"
        "ldr r0, [r0]\n\t"
        "asr r0, r0, #8\n\t"
        "ldr r1, 3f\n\t"
        "ldr r1, [r1]\n\t"
        "mul r0, r1, r0\n\t"
        "mov r1, #0x81\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r2]\n\t"
        "add r6, r1, r0\n\t"
        "ldr r0, 4f\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "ldr r1, 6f\n\t"
        "ldrb r0, [r1]\n\t"
        "ldr r5, 7f\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "mov r5, #0xc0\n\t"
        "lsl r5, r5, #0x13\n\t"
        "8:\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r4, 10f\n\t"
        "ldrb r1, [r1]\n\t"
        "ldr r2, 11f\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r3, 12f\n\t"
        "ldr r3, [r3]\n\t"
        "ldr r4, [r4]\n\t"
        "bl sub_803AD88\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
        "1: .4byte gUnknown_03001394\n"
        "2: .4byte gUnknown_030013B0\n"
        "3: .4byte gUnknown_030013A4\n"
        "4: .4byte gUnknown_030013B8\n"
        "6: .4byte gUnknown_030013B9\n"
        "7: .4byte 0x06002000\n"
        "9: .4byte gUnknown_030013A0\n"
        "10: .4byte gUnknown_0300087C\n"
        "11: .4byte gUnknown_03001398\n"
        "12: .4byte gUnknown_0300139C\n"
        "5:\n\t"
        "ldr r0, 14f\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 15f\n\t"
        "ldr r5, 16f\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
        "14: .4byte gUnknown_030013B9\n"
        "16: .4byte 0x06000020\n"
        "15:\n\t"
        "ldr r0, 17f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, 18f\n\t"
        "add r5, r0, r1\n\t"
        "13:\n\t"
        "ldr r2, 19f\n\t"
        "str r6, [r2]\n\t"
        "str r5, [r2, #4]\n\t"
        "ldr r0, 17f\n\t"
        "ldr r0, [r0]\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #0x18\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2, #8]\n\t"
        "ldr r0, [r2, #8]\n\t"
        "ldr r1, 20f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "17: .4byte gUnknown_030013A0\n"
        "18: .4byte 0x06000020\n"
        "19: .4byte 0x040000D4\n"
        "20: .4byte gUnknown_030013BA\n"
    );
}

extern s32 gUnknown_030013AC;
extern s32 gUnknown_030013C8;
extern s32 gUnknown_030013A8;
extern s32 gUnknown_030013B4;
extern s32 gUnknown_030013BC;
extern void sub_802996C(void);
extern s32 sub_803ADB4(s32 a, s32 b);

/* NAKED - (re)configures the console/text-plane cell geometry from a
 * fresh cell record at `arg1` (a `{..., s16 width @0x200,
 * s16 height @0x202}` layout) - cell pixel area, its DMA-scroll-wrap
 * threshold, and the initial X/Y scroll accumulators - then rebuilds
 * both VRAM screen blocks via `sub_802996C`. Fully understood; a
 * plain-C attempt (register pins on `base`/`arg2`) reproduced every
 * instruction but landed 4 bytes short - the same kind of register-role
 * permutation gap already parked elsewhere in this file/chunk. See
 * docs/workflow.md's NAKED-transcription escape hatch. */
NAKED void sub_8029890(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r1, #0\n\t"
        "add r6, r2, #0\n\t"
        "add r7, r3, #0\n\t"
        "ldr r1, 1f\n\t"
        "mov r5, #0\n\t"
        "cmp r0, #0\n\t"
        "bne 2f\n\t"
        "mov r5, #1\n\t"
        "2:\n\t"
        "strb r5, [r1]\n\t"
        "ldr r0, 3f\n\t"
        "str r4, [r0]\n\t"
        "ldr r1, 4f\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #2\n\t"
        "add r0, r4, r2\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r0, r3]\n\t"
        "str r2, [r1]\n\t"
        "ldr r1, 5f\n\t"
        "ldr r3, 6f\n\t"
        "add r0, r4, r3\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r0, r3]\n\t"
        "str r0, [r1]\n\t"
        "ldr r1, 7f\n\t"
        "mul r2, r0, r2\n\t"
        "lsl r0, r2, #5\n\t"
        "str r0, [r1]\n\t"
        "ldr r4, 8f\n\t"
        "add r3, r0, #0\n\t"
        "add r1, r4, #0\n\t"
        "cmp r5, #0\n\t"
        "beq 9f\n\t"
        "add r0, r2, #7\n\t"
        "cmp r0, #0\n\t"
        "bge 10f\n\t"
        "add r0, #7\n\t"
        "10:\n\t"
        "asr r0, r0, #3\n\t"
        "lsl r0, r0, #2\n\t"
        "add r3, r3, r0\n\t"
        "9:\n\t"
        "str r3, [r4]\n\t"
        "ldr r4, 11f\n\t"
        "ldr r2, 12f\n\t"
        "add r0, r6, r2\n\t"
        "ldr r1, [r1]\n\t"
        "bl sub_803ADB4\n\t"
        "lsl r0, r0, #8\n\t"
        "str r0, [r4]\n\t"
        "ldr r0, 13f\n\t"
        "mov r4, #0\n\t"
        "str r4, [r0]\n\t"
        "bl sub_802996C\n\t"
        "ldr r0, 14f\n\t"
        "str r4, [r0]\n\t"
        "ldr r0, 1f\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 15f\n\t"
        "ldr r1, 16f\n\t"
        "ldr r0, 17f\n\t"
        "ldr r0, [r0]\n\t"
        "sub r0, r7, r0\n\t"
        "b 18f\n\t"
        ".align 2, 0\n"
        "1: .4byte gUnknown_030013B8\n"
        "3: .4byte gUnknown_03001394\n"
        "4: .4byte gUnknown_03001398\n"
        "5: .4byte gUnknown_0300139C\n"
        "6: .4byte 0x00000202\n"
        "7: .4byte gUnknown_030013A0\n"
        "8: .4byte gUnknown_030013A4\n"
        "11: .4byte gUnknown_030013AC\n"
        "12: .4byte 0xFFFFFDFC\n"
        "13: .4byte gUnknown_030013B0\n"
        "14: .4byte gUnknown_030013B4\n"
        "16: .4byte gUnknown_030013A8\n"
        "17: .4byte gUnknown_030013C8\n"
        "15:\n\t"
        "ldr r1, 19f\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r7, r0\n\t"
        "18:\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [r1]\n\t"
        "ldr r1, 21f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r1]\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "19: .4byte gUnknown_030013A8\n"
        "20: .4byte gUnknown_030013C8\n"
        "21: .4byte gUnknown_030013BC\n"
    );
}

/* NAKED - tried a direct C translation of this nested-loop VRAM
 * tilemap-fill pair (the same shape as sub_8029BC4 in actor_part98.c,
 * but with an extra high-register pair `sb`/`sl` live for the loop
 * bounds ON TOP of the `r8` 0x7c0-offset constant sub_8029BC4 already
 * needed) - the extra register pressure made this strictly harder than
 * sub_8029BC4's own unresolved permutation gap. Documented per
 * docs/workflow.md's NAKED escape hatch. */
NAKED void sub_802996C(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #4\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #0x13\n\t"
        "ldr r2, 20f\n\t"
        "add r0, r2, #0\n\t"
        "strh r0, [r1]\n\t"
        "add r1, #8\n\t"
        "ldr r2, 21f\n\t"
        "add r0, r2, #0\n\t"
        "strh r0, [r1]\n\t"
        "add r1, #0xcc\n\t"
        "ldr r0, 22f\n\t"
        "ldr r0, [r0]\n\t"
        "str r0, [r1]\n\t"
        "mov r0, #0xa0\n\t"
        "lsl r0, r0, #0x13\n\t"
        "str r0, [r1, #4]\n\t"
        "ldr r0, 23f\n\t"
        "str r0, [r1, #8]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "ldr r0, 24f\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "b 12f\n\t"
    "1:\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #0x13\n\t"
        "ldr r0, 25f\n\t"
        "mov sb, r0\n\t"
        "ldr r2, 26f\n\t"
        "mov sl, r2\n\t"
        "mov r2, #0\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x1c\n\t"
    "2:\n\t"
        "str r2, [r0]\n\t"
        "sub r0, #4\n\t"
        "cmp r0, r1\n\t"
        "bge 2b\n\t"
        "mov r1, sp\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r1]\n\t"
        "ldr r1, 27f\n\t"
        "mov r0, sp\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, 28f\n\t"
        "str r0, [r1, #4]\n\t"
        "ldr r0, 29f\n\t"
        "str r0, [r1, #8]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "mov r1, sb\n\t"
        "ldr r7, [r1]\n\t"
        "mov r2, sl\n\t"
        "ldr r2, [r2]\n\t"
        "mov ip, r2\n\t"
        "ldr r4, 30f\n\t"
        "mov r3, #1\n\t"
        "mov r0, #0\n\t"
        "cmp r0, ip\n\t"
        "bge 7f\n\t"
        "mov r1, #0xf8\n\t"
        "lsl r1, r1, #3\n\t"
        "mov r8, r1\n\t"
    "5:\n\t"
        "mov r2, #0\n\t"
        "add r5, r4, #0\n\t"
        "add r5, #0x40\n\t"
        "add r6, r0, #1\n\t"
        "cmp r2, r7\n\t"
        "bge 6f\n\t"
        "mov r0, r8\n\t"
        "add r1, r4, r0\n\t"
        "add r0, r4, #0\n\t"
    "13:\n\t"
        "cmp r2, #0x1f\n\t"
        "bgt 3f\n\t"
        "strh r3, [r0]\n\t"
        "b 4f\n\t"
        ".align 2, 0\n"
        "20: .4byte 0x00001141\n"
        "21: .4byte 0x00005C02\n"
        "22: .4byte gUnknown_03001394\n"
        "23: .4byte 0x80000100\n"
        "24: .4byte gUnknown_030013B8\n"
        "25: .4byte gUnknown_03001398\n"
        "26: .4byte gUnknown_0300139C\n"
        "27: .4byte 0x040000D4\n"
        "28: .4byte 0x0600E000\n"
        "29: .4byte 0x81001000\n"
        "30: .4byte 0x0600E400\n"
    "3:\n\t"
        "strh r3, [r1]\n\t"
    "4:\n\t"
        "add r3, #1\n\t"
        "add r1, #2\n\t"
        "add r0, #2\n\t"
        "add r2, #1\n\t"
        "cmp r2, r7\n\t"
        "blt 13b\n\t"
    "6:\n\t"
        "add r4, r5, #0\n\t"
        "add r0, r6, #0\n\t"
        "cmp r0, ip\n\t"
        "blt 5b\n\t"
    "7:\n\t"
        "mov r1, sb\n\t"
        "ldr r5, [r1]\n\t"
        "mov r2, sl\n\t"
        "ldr r2, [r2]\n\t"
        "mov ip, r2\n\t"
        "ldr r4, 31f\n\t"
        "mov r0, ip\n\t"
        "mul r0, r5, r0\n\t"
        "add r0, #1\n\t"
        "mov r1, #0\n\t"
        "cmp r1, ip\n\t"
        "bge 12f\n\t"
        "mov r2, #0xf8\n\t"
        "lsl r2, r2, #3\n\t"
        "mov r8, r2\n\t"
    "8:\n\t"
        "mov r3, #0\n\t"
        "add r7, r4, #0\n\t"
        "add r7, #0x40\n\t"
        "add r6, r1, #1\n\t"
        "cmp r3, r5\n\t"
        "bge 11f\n\t"
        "mov r1, r8\n\t"
        "add r2, r4, r1\n\t"
        "add r1, r4, #0\n\t"
    "14:\n\t"
        "cmp r3, #0x1f\n\t"
        "bgt 9f\n\t"
        "strh r0, [r1]\n\t"
        "b 10f\n\t"
        ".align 2, 0\n"
        "31: .4byte 0x0600F400\n"
    "9:\n\t"
        "strh r0, [r2]\n\t"
    "10:\n\t"
        "add r0, #1\n\t"
        "add r2, #2\n\t"
        "add r1, #2\n\t"
        "add r3, #1\n\t"
        "cmp r3, r5\n\t"
        "blt 14b\n\t"
    "11:\n\t"
        "add r4, r7, #0\n\t"
        "add r1, r6, #0\n\t"
        "cmp r1, ip\n\t"
        "blt 8b\n\t"
    "12:\n\t"
        "mov r0, #1\n\t"
        "ldr r2, 32f\n\t"
        "strb r0, [r2]\n\t"
        "bl sub_80297C8\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "32: .4byte gUnknown_030013B9\n"
    );
}
