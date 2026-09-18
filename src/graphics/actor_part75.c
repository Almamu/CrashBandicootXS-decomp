#include "core.h"

/* Sits right after actor_part59.c's `sub_802DCC0` and before
 * actor_part60.c's `sub_802DFBC` - the whole contiguous range that used
 * to be `asm/code_3_2_20_28568_c99c_dd9c.s`. Both functions continue the
 * `gUnknown_030014BC`-rooted "gauge" object documented in actor_part59.c/
 * actor_part74.c's header comments. */

extern u8 gStaticData_0817AA8C[];
extern s32 gUnknown_030014C4;
extern s32 gUnknown_030014C8;
extern void *sub_800014C(void *dest, void *src, s32 size);

/* `sub_802D7B0`'s (actor_part74.c) shared AABB-overlap-test tail,
 * factored out as its own function taking `self` explicitly instead of
 * always reading the player global - used by `sub_802D6A0`
 * (actor_part58.c, already matched, called as `sub_802DD9C(self)`)
 * among others. Same 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` record
 * shape and same self-copy-through-`sub_800014C` idiom as `sub_802D7B0`
 * - see that function's doc comment for the full record-layout writeup.
 * Box A: `gStaticData_0817AA8C` (a record adjacent to `sub_802D7B0`'s
 * own `gStaticData_0817AA98` - literal-pool-verified 0xC bytes apart)
 * with `gUnknown_030014C4`/`030014C8` (both `>>8`) added into its `x`/
 * `z` fields only. Box B: `self+0x38`'s own 12-byte vector, with
 * `self`'s own `+0x1c`/`0x20`/`0x24` position (all `>>8`) added into
 * all three of `x`/`y`/`z` - this is the "self+0x38's own vector"
 * referenced from docs/matching/issue-54-actor-d3a8.md's original
 * parked writeup.
 *
 * Written as NAKED asm for the same register-pressure reasons as
 * `sub_802D7B0` (actor_part74.c) - mechanical, byte-verified
 * transcription, not an inferred guess. */
NAKED u8 sub_802DD9C(void *self)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "sub sp, #0x24\n\t"
        "mov r2, sp\n\t"
        "ldr r1, 2f\n\t"
        "ldm r1!, {r3, r4, r5}\n\t"
        "stm r2!, {r3, r4, r5}\n\t"
        "ldr r1, 3f\n\t"
        "ldr r2, [r1]\n\t"
        "asr r2, r2, #8\n\t"
        "ldr r1, 4f\n\t"
        "ldr r3, [r1]\n\t"
        "asr r3, r3, #8\n\t"
        "mov r1, sp\n\t"
        "ldrh r4, [r1]\n\t"
        "add r2, r4, r2\n\t"
        "strh r2, [r1]\n\t"
        "ldrh r5, [r1, #4]\n\t"
        "add r3, r5, r3\n\t"
        "strh r3, [r1, #4]\n\t"
        "add r2, sp, #0x18\n\t"
        "add r1, r0, #0\n\t"
        "add r1, #0x38\n\t"
        "ldm r1!, {r3, r4, r5}\n\t"
        "stm r2!, {r3, r4, r5}\n\t"
        "ldr r2, [r0, #0x1c]\n\t"
        "asr r2, r2, #8\n\t"
        "ldr r4, [r0, #0x20]\n\t"
        "asr r4, r4, #8\n\t"
        "ldr r3, [r0, #0x24]\n\t"
        "asr r3, r3, #8\n\t"
        "add r1, sp, #0x18\n\t"
        "ldrh r0, [r1]\n\t"
        "add r2, r0, r2\n\t"
        "strh r2, [r1]\n\t"
        "ldrh r0, [r1, #2]\n\t"
        "add r0, r0, r4\n\t"
        "strh r0, [r1, #2]\n\t"
        "ldrh r2, [r1, #4]\n\t"
        "add r3, r2, r3\n\t"
        "strh r3, [r1, #4]\n\t"
        "add r0, sp, #0xc\n\t"
        "ldm r1!, {r3, r4, r5}\n\t"
        "stm r0!, {r3, r4, r5}\n\t"
        "add r4, sp, #0xc\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r4, #0\n\t"
        "mov r2, #0xc\n\t"
        "bl sub_800014C\n\t"
        "mov r1, sp\n\t"
        "mov r0, #4\n\t"
        "ldrsh r2, [r1, r0]\n\t"
        "mov r5, #4\n\t"
        "ldrsh r3, [r4, r5]\n\t"
        "mov r5, #0xa\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 1f\n\t"
        "mov r5, #0xa\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 1f\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r1, r0]\n\t"
        "mov r5, #2\n\t"
        "ldrsh r3, [r4, r5]\n\t"
        "mov r5, #8\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 1f\n\t"
        "mov r5, #8\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 1f\n\t"
        "mov r0, #0\n\t"
        "ldrsh r2, [r1, r0]\n\t"
        "mov r5, #0\n\t"
        "ldrsh r3, [r4, r5]\n\t"
        "mov r5, #6\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 1f\n\t"
        "mov r4, #6\n\t"
        "ldrsh r0, [r1, r4]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "bgt 5f\n\t"
        "1:\n\t"
        "mov r0, #0\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
        "2: .4byte gStaticData_0817AA8C\n"
        "3: .4byte gUnknown_030014C4\n"
        "4: .4byte gUnknown_030014C8\n"
        "5:\n\t"
        "mov r0, #1\n\t"
        "6:\n\t"
        "add sp, #0x24\n\t"
        "pop {r4, r5}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    );
}

extern u8 gUnknown_030014C0;
extern void *gUnknown_03000898;
extern void *gUnknown_030014BC;
extern void sub_803AD80(void *arg0, s32 arg1, void *fn);
extern u8 gUnknown_030014C1;
extern void sub_802DA68(void);
extern void sub_802D9A8(void);

/* The `gUnknown_030014BC` object's own initial VRAM-pattern/DMA setup
 * (called once from `sub_802DFDC`'s constructor, actor_part60.c): sets
 * `REG_DISPCNT`'s OBJ-window-enable bit (`DISPCNT_OBJWIN_ON`, bit 15),
 * then runs the same 16x16 triangular-fill dot-pattern loop twice into a
 * 0x100-byte stack buffer (`sub_802E058`'s own loop body, parameterized
 * there by seed/destination but fixed here to a `0`/`0x80` seed pair) -
 * DMA3-transferring the first fill to VRAM tile `0x0600D000` and the
 * second to `0x0600D800` (`REG_DMA3SAD`/`DAD`/`CNT` at `0x040000D4`,
 * 0x80 words, 32-bit transfers). Then clears a third tile
 * (`0x0600BFC0`-`0x0600BFFC`) word-by-word, arms the object
 * (`gUnknown_030014C0 = 1`), fires a `sub_803AD80` trampoline from the
 * part table's own `+2`-offset record (the same call shape
 * `sub_802D7B0`/actor_part74.c uses, latching `gUnknown_030014C1`), and
 * finally calls `sub_802DA68`/`sub_802D9A8` (actor_part74.c) to prime
 * the gauge's sound/palette state immediately.
 *
 * Written as NAKED asm: ~160 instructions with heavy `r8`/`sb`/`sl`
 * register pressure (three globals cached across the whole double loop)
 * and DMA-timing-sensitive register reuse that resisted a plain-C
 * reconstruction - mechanical, byte-verified transcription of the
 * already-fully-understood semantics above (every operand/order
 * confirmed against `expected/code_3.s` first), not an inferred
 * control-flow guess. */
NAKED void sub_802DE70(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x100\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #0x13\n\t"
        "ldrh r1, [r0]\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #3\n\t"
        "add r2, r3, #0\n\t"
        "orr r1, r2\n\t"
        "strh r1, [r0]\n\t"
        "mov ip, sp\n\t"
        "mov r6, #0\n\t"
        "mov r5, #0\n\t"
        "ldr r0, 4f\n\t"
        "mov sb, r0\n\t"
        "ldr r1, 5f\n\t"
        "mov sl, r1\n\t"
        "ldr r3, 6f\n\t"
        "mov r8, r3\n\t"
        "mov r7, #0xff\n\t"
        "1:\n\t"
        "mov r4, #0\n\t"
        "lsl r0, r5, #4\n\t"
        "add r2, r5, #1\n\t"
        "mov r1, ip\n\t"
        "add r3, r0, r1\n\t"
        "2:\n\t"
        "sub r0, r4, #3\n\t"
        "cmp r0, #9\n\t"
        "bhi 3f\n\t"
        "cmp r5, #2\n\t"
        "ble 3f\n\t"
        "cmp r5, #0xc\n\t"
        "ble 7f\n\t"
        "3:\n\t"
        "strb r7, [r3]\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
        "4: .4byte gUnknown_030014C0\n"
        "5: .4byte gUnknown_03000898\n"
        "6: .4byte gUnknown_030014BC\n"
        "7:\n\t"
        "add r1, r6, #0\n\t"
        "add r0, r1, #1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r6, r0, #0x18\n\t"
        "strb r1, [r3]\n\t"
        "8:\n\t"
        "add r3, #1\n\t"
        "add r4, #1\n\t"
        "cmp r4, #0xf\n\t"
        "ble 2b\n\t"
        "add r5, r2, #0\n\t"
        "cmp r5, #0xf\n\t"
        "ble 1b\n\t"
        "ldr r1, 12f\n\t"
        "mov r3, sp\n\t"
        "str r3, [r1]\n\t"
        "ldr r0, 13f\n\t"
        "str r0, [r1, #4]\n\t"
        "ldr r0, 14f\n\t"
        "str r0, [r1, #8]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "mov ip, sp\n\t"
        "mov r6, #0x80\n\t"
        "mov r5, #0\n\t"
        "mov r7, #0xff\n\t"
        "9:\n\t"
        "mov r4, #0\n\t"
        "lsl r0, r5, #4\n\t"
        "add r2, r5, #1\n\t"
        "mov r1, ip\n\t"
        "add r3, r0, r1\n\t"
        "10:\n\t"
        "sub r0, r4, #3\n\t"
        "cmp r0, #9\n\t"
        "bhi 11f\n\t"
        "cmp r5, #2\n\t"
        "ble 11f\n\t"
        "cmp r5, #0xc\n\t"
        "ble 15f\n\t"
        "11:\n\t"
        "strb r7, [r3]\n\t"
        "b 16f\n\t"
        ".align 2, 0\n"
        "12: .4byte 0x040000D4\n"
        "13: .4byte 0x0600D000\n"
        "14: .4byte 0x80000080\n"
        "15:\n\t"
        "add r1, r6, #0\n\t"
        "add r0, r1, #1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r6, r0, #0x18\n\t"
        "strb r1, [r3]\n\t"
        "16:\n\t"
        "add r3, #1\n\t"
        "add r4, #1\n\t"
        "cmp r4, #0xf\n\t"
        "ble 10b\n\t"
        "add r5, r2, #0\n\t"
        "cmp r5, #0xf\n\t"
        "ble 9b\n\t"
        "ldr r1, 18f\n\t"
        "mov r3, sp\n\t"
        "str r3, [r1]\n\t"
        "ldr r0, 19f\n\t"
        "str r0, [r1, #4]\n\t"
        "ldr r0, 20f\n\t"
        "str r0, [r1, #8]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "ldr r1, 21f\n\t"
        "mov r3, #0\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x3c\n\t"
        "17:\n\t"
        "str r3, [r0]\n\t"
        "sub r0, #4\n\t"
        "cmp r0, r1\n\t"
        "bge 17b\n\t"
        "mov r5, #1\n\t"
        "mov r0, sb\n\t"
        "strb r5, [r0]\n\t"
        "mov r1, r8\n\t"
        "ldr r2, [r1]\n\t"
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
        "add r0, #4\n\t"
        "mov r3, sl\n\t"
        "ldr r2, [r3]\n\t"
        "mov r1, #1\n\t"
        "bl sub_803AD80\n\t"
        "ldr r0, 22f\n\t"
        "strb r5, [r0]\n\t"
        "bl sub_802DA68\n\t"
        "bl sub_802D9A8\n\t"
        "add sp, #0x100\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "18: .4byte 0x040000D4\n"
        "19: .4byte 0x0600D800\n"
        "20: .4byte 0x80000080\n"
        "21: .4byte 0x0600BFC0\n"
        "22: .4byte gUnknown_030014C1\n"
    );
}

asm(".align 2, 0");
