#include "core.h"

/* Sits right after actor_part58.c's `sub_802D764` and before
 * actor_part59.c's `sub_802DB2C`/`sub_802DCC0` - the whole contiguous
 * range that used to be `asm/code_3_2_20_28568_c99c_d7b0.s`. All three
 * functions here operate on the `gUnknown_030014BC`-rooted "position-
 * tracking object with tier-threshold sound cues" documented in
 * actor_part59.c's header comment and docs/matching/issue-54-actor-d3a8.md
 * (the "third RAM-struct family" from docs/rom_map.md). See that issue
 * doc's "Second pass" section for how the 12-byte AABB-record layout
 * used here and by `sub_802DD9C` (actor_part75.c) was finally pinned
 * down. */

extern s32 gUnknown_030014D0;
extern s32 gUnknown_030014C4;
extern void *gUnknown_03000884;
extern void *gUnknown_030014BC;
extern u16 GetAnimFrameBaseOffset(void *self);
extern u8 gStaticData_0817A840[];
extern void sub_803AD78(void *fn);
extern void *gUnknown_03000898;
extern void sub_803AD80(void *arg0, s32 arg1, void *fn);
extern u8 gUnknown_030014C0;
extern u8 gUnknown_030014C1;
extern s32 gUnknown_030014CC;
extern void sub_8029E34(s32 arg0);
extern void sub_802D9A8(void);
extern u8 gStaticData_0817AA98[];
extern s32 gUnknown_030014C8;
extern u8 gUnknown_030014A0;
extern void *sub_800014C(void *dest, void *src, s32 size);
extern void sub_802C018(void *self);
extern void sub_8029BAC(s32 arg0);

/* One of two confirmed slots (index 3, dispatched via
 * `gStaticData_0817A840[gUnknown_030014D0]`) of the type-0
 * `category_vtable` (`gStaticData_081756C4[0]`, `include/actor_anim.h`)
 * - `UpdateGameFrame`'s own direct top-level callee for this object, per
 * docs/rom_map.md's "Two new type-0 vtable slots confirmed" section.
 *
 * Unless `gUnknown_030014D0 == 3`, first eases `gUnknown_030014C4`
 * toward the player's cached X position (`gUnknown_03000884->+0x1c`,
 * divisor 32 - the same rsb/lsr/add/asr round-toward-zero idiom as
 * `sub_802D3A8`/`sub_80070EC`). Then advances the object's own anim
 * frame (`+8` accumulator by the `+0x10` per-frame increment,
 * `GetAnimFrameBaseOffset` against the current part-table record's
 * `+4`/`+6` timing fields, latching the `+0x12` done flag and correcting
 * the accumulator on overrun), fires the current
 * `gStaticData_0817A840[gUnknown_030014D0]` vtable slot via
 * `sub_803AD78`, and - only when the accumulator's `>>8` value actually
 * changed this frame - fires a `sub_803AD80` trampoline from the part
 * table's own `+2`-offset record (latching `gUnknown_030014C1`).
 *
 * Finally, while `gUnknown_030014D0 <= 1`, runs a 3-axis AABB overlap
 * test between two 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` records
 * (axes compared Z, then Y, then X - matching the ROM's own instruction
 * order, not storage order) built the same way both times: a
 * `gStaticData_0817AA98`-rooted static record with `gUnknown_030014C4`/
 * `030014C8` (both `>>8`) added into its `x`/`z` fields only (this
 * object tracks no Y), against the player's own `+0x38` 12-byte vector
 * with the player's `+0x1c`/`0x20`/`0x24` position (all `>>8`) added
 * into all three of `x`/`y`/`z`. The second record is then run through
 * `sub_800014C` - a real, byte-verified `memcpy(box, box, 0xc)`
 * self-copy (`sub_800014C`'s own definition, `src/system/boot_util.c`,
 * confirmed a plain `memcpy`-style `CpuSet` wrapper) - a genuine no-op
 * kept byte-faithful since a shared "copy src into a working buffer,
 * then test" helper is being called here with a buffer that already
 * *is* its own source, not a disassembly artifact. On overlap, arms
 * `gUnknown_030014D0 = 2`, resets the object's kind/anim state to the
 * part table's `+0x18` record, and refreshes the player via
 * `sub_802C018`/`sub_8029BAC(0)`; skipped once `gUnknown_030014A0` (an
 * already-consumed one-shot flag elsewhere in this ROM region) is set.
 *
 * Written as NAKED asm, not plain C: this function's heavy stack-buffer
 * use (two 12-byte scratch AABB records sharing one 0x24-byte frame,
 * built with raw `ldm`/`stm` block copies) and register reuse (`r5`
 * holds the `gUnknown_030014BC` pointer early on, then gets clobbered
 * with an unrelated accumulator delta later; `r7` similarly holds the
 * vtable dispatch index then the "is `030014D0` <= 1" comparison
 * operand) make this a poor match for gcc 2.9's register allocator
 * without extensive per-register archaeology - each individual
 * instruction's operation/operand/order was fully confirmed against the
 * ROM disassembly (`expected/code_3.s`) first, so this is a mechanical,
 * byte-verified transcription (translated from the disassembler's
 * unified syntax to this project's established NAKED plain/divided
 * syntax, `adds`->`add`/`ands`->`and`/etc, local labels renumbered per
 * docs/matching/issue-4-sio-settings-sync.md's convention), not an
 * inferred control-flow guess. */
NAKED void sub_802D7B0(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #0x24\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r0, #3\n\t"
        "beq 2f\n\t"
        "ldr r2, 8f\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x1c]\n\t"
        "ldr r1, [r2]\n\t"
        "sub r0, r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bge 1f\n\t"
        "add r0, #0x1f\n\t"
        "1:\n\t"
        "asr r0, r0, #5\n\t"
        "add r0, r1, r0\n\t"
        "str r0, [r2]\n\t"
        "2:\n\t"
        "ldr r5, 10f\n\t"
        "ldr r4, [r5]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "asr r6, r0, #8\n\t"
        "mov r2, #0x10\n\t"
        "ldrsh r1, [r4, r2]\n\t"
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
        "blt 3f\n\t"
        "mov r3, #6\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "sub r0, r2, r0\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r4, #8]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r4, #8]\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r4, #0x12]\n\t"
        "3:\n\t"
        "ldr r1, 11f\n\t"
        "ldr r7, 7f\n\t"
        "ldr r0, [r7]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_803AD78\n\t"
        "ldr r4, [r5]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "asr r5, r0, #8\n\t"
        "cmp r6, r5\n\t"
        "beq 4f\n\t"
        "ldr r3, 12f\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "ldr r2, [r4]\n\t"
        "lsl r0, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r2\n\t"
        "mov r1, #2\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r1, [r4, #4]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #4\n\t"
        "ldr r1, 13f\n\t"
        "ldrb r1, [r1]\n\t"
        "ldr r2, [r3]\n\t"
        "bl sub_803AD80\n\t"
        "ldr r1, 14f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "4:\n\t"
        "ldr r0, 15f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8029E34\n\t"
        "bl sub_802D9A8\n\t"
        "mov r1, sp\n\t"
        "ldr r0, 16f\n\t"
        "ldm r0!, {r2, r3, r4}\n\t"
        "stm r1!, {r2, r3, r4}\n\t"
        "ldr r0, 8f\n\t"
        "ldr r1, [r0]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, 17f\n\t"
        "ldr r2, [r0]\n\t"
        "asr r2, r2, #8\n\t"
        "mov r0, sp\n\t"
        "ldrh r5, [r0]\n\t"
        "add r1, r5, r1\n\t"
        "strh r1, [r0]\n\t"
        "ldrh r1, [r0, #4]\n\t"
        "add r2, r1, r2\n\t"
        "strh r2, [r0, #4]\n\t"
        "ldr r0, [r7]\n\t"
        "cmp r0, #1\n\t"
        "bls 5f\n\t"
        "b 21f\n\t"
        "5:\n\t"
        "ldr r2, 9f\n\t"
        "ldr r0, 18f\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 21f\n\t"
        "ldr r2, [r2]\n\t"
        "add r1, sp, #0x18\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x38\n\t"
        "ldm r0!, {r3, r4, r5}\n\t"
        "stm r1!, {r3, r4, r5}\n\t"
        "ldr r0, [r2, #0x1c]\n\t"
        "asr r0, r0, #8\n\t"
        "ldr r3, [r2, #0x20]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r2, [r2, #0x24]\n\t"
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
        "ldrsh r2, [r4, r5]\n\t"
        "mov r0, #4\n\t"
        "ldrsh r3, [r1, r0]\n\t"
        "mov r5, #0xa\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 6f\n\t"
        "mov r5, #0xa\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 6f\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r4, r0]\n\t"
        "mov r5, #2\n\t"
        "ldrsh r3, [r1, r5]\n\t"
        "mov r5, #8\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 6f\n\t"
        "mov r5, #8\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 6f\n\t"
        "mov r0, #0\n\t"
        "ldrsh r2, [r4, r0]\n\t"
        "mov r5, #0\n\t"
        "ldrsh r3, [r1, r5]\n\t"
        "mov r5, #6\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 6f\n\t"
        "mov r1, #6\n\t"
        "ldrsh r0, [r4, r1]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "bgt 19f\n\t"
        "6:\n\t"
        "mov r0, #0\n\t"
        "b 20f\n\t"
        ".align 2, 0\n"
        "7: .4byte gUnknown_030014D0\n"
        "8: .4byte gUnknown_030014C4\n"
        "9: .4byte gUnknown_03000884\n"
        "10: .4byte gUnknown_030014BC\n"
        "11: .4byte gStaticData_0817A840\n"
        "12: .4byte gUnknown_03000898\n"
        "13: .4byte gUnknown_030014C0\n"
        "14: .4byte gUnknown_030014C1\n"
        "15: .4byte gUnknown_030014CC\n"
        "16: .4byte gStaticData_0817AA98\n"
        "17: .4byte gUnknown_030014C8\n"
        "18: .4byte gUnknown_030014A0\n"
        "19:\n\t"
        "mov r0, #1\n\t"
        "20:\n\t"
        "cmp r0, #0\n\t"
        "beq 21f\n\t"
        "ldr r0, 22f\n\t"
        "mov r2, #2\n\t"
        "str r2, [r0]\n\t"
        "ldr r0, 23f\n\t"
        "ldr r1, [r0]\n\t"
        "str r2, [r1, #0xc]\n\t"
        "ldr r0, [r1]\n\t"
        "ldrh r0, [r0, #0x18]\n\t"
        "mov r2, #0\n\t"
        "mov r3, #0\n\t"
        "strh r0, [r1, #0x10]\n\t"
        "strb r2, [r1, #0x12]\n\t"
        "str r3, [r1, #8]\n\t"
        "ldr r0, 24f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_802C018\n\t"
        "mov r0, #0\n\t"
        "bl sub_8029BAC\n\t"
        "21:\n\t"
        "add sp, #0x24\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "22: .4byte gUnknown_030014D0\n"
        "23: .4byte gUnknown_030014BC\n"
        "24: .4byte gUnknown_03000884\n"
    );
}

/* Palette-gradient cursor for the `gUnknown_030014BC` "gauge" object.
 * Below `0x5000`, DMAs a fixed 16-color gradient (`gStaticData_0817AA6C`)
 * straight into BG palette RAM at `0x050001E0` (`REG_DMA3` at
 * `0x040000D4`). Above `0xBE00`, DMAs a single zeroed halfword instead
 * (blanking the gradient). In between, computes a `sub_803ADB4`-scaled
 * factor from how far `gUnknown_030014CC` sits into that `[0x5000,
 * 0xBE00]` range, then directly writes 16 colors: for each
 * `gStaticData_0817AA6C` source halfword (a packed BGR555 color), its
 * 5-bit R/G/B channels are each independently scaled by that factor
 * (`>>8` after the multiply) and repacked (`R | (G<<5) | (B<<10)`) into
 * BG palette RAM at `0x050001E0` onward - a manual brightness ramp
 * rather than a second DMA.
 *
 * Written as NAKED asm for the same register-pressure reasons as
 * `sub_802D7B0` above (`ip` holds a repeated shift mask across the
 * whole 16-entry loop, `r3` the scale factor, `r4`/`r5`/`r6`/`r7`
 * juggling the loop counter and both moving source/dest pointers) -
 * mechanical, byte-verified transcription, not an inferred guess (every
 * operand/order confirmed against `expected/code_3.s` first). */
NAKED void sub_802D9A8(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #4\n\t"
        "ldr r0, 1f\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, 2f\n\t"
        "cmp r1, r0\n\t"
        "bgt 7f\n\t"
        "ldr r1, 3f\n\t"
        "ldr r0, 4f\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, 5f\n\t"
        "str r0, [r1, #4]\n\t"
        "ldr r0, 6f\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
        "1: .4byte gUnknown_030014CC\n"
        "2: .4byte 0x00004FFF\n"
        "3: .4byte 0x040000D4\n"
        "4: .4byte gStaticData_0817AA6C\n"
        "5: .4byte 0x050001E0\n"
        "6: .4byte 0x80000010\n"
        "7:\n\t"
        "ldr r0, 9f\n\t"
        "cmp r1, r0\n\t"
        "ble 13f\n\t"
        "mov r1, sp\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r1]\n\t"
        "ldr r1, 10f\n\t"
        "mov r0, sp\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, 11f\n\t"
        "str r0, [r1, #4]\n\t"
        "ldr r0, 12f\n\t"
        "8:\n\t"
        "str r0, [r1, #8]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "b 15f\n\t"
        ".align 2, 0\n"
        "9: .4byte 0x0000BDFF\n"
        "10: .4byte 0x040000D4\n"
        "11: .4byte 0x050001E0\n"
        "12: .4byte 0x81000010\n"
        "13:\n\t"
        "mov r0, #0xbe\n\t"
        "lsl r0, r0, #8\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #8\n\t"
        "mov r1, #0xdc\n\t"
        "lsl r1, r1, #7\n\t"
        "bl sub_803ADB4\n\t"
        "add r3, r0, #0\n\t"
        "mov r0, #0x1f\n\t"
        "mov ip, r0\n\t"
        "ldr r6, 16f\n\t"
        "ldr r5, 17f\n\t"
        "mov r7, #0x1f\n\t"
        "mov r4, #0xf\n\t"
        "14:\n\t"
        "ldrh r1, [r5]\n\t"
        "add r0, r7, #0\n\t"
        "and r0, r1\n\t"
        "add r2, r0, #0\n\t"
        "mul r2, r3, r2\n\t"
        "asr r2, r2, #8\n\t"
        "lsr r1, r1, #5\n\t"
        "mov r0, ip\n\t"
        "and r1, r0\n\t"
        "add r0, r1, #0\n\t"
        "mul r0, r3, r0\n\t"
        "asr r0, r0, #8\n\t"
        "lsl r1, r0, #5\n\t"
        "orr r2, r1\n\t"
        "lsl r0, r0, #0xa\n\t"
        "orr r2, r0\n\t"
        "strh r2, [r6]\n\t"
        "add r6, #2\n\t"
        "add r5, #2\n\t"
        "sub r4, #1\n\t"
        "cmp r4, #0\n\t"
        "bge 14b\n\t"
        "15:\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "16: .4byte 0x050001E0\n"
        "17: .4byte gStaticData_0817AA6C\n"
    );
}

/* Companion to `sub_802D9A8` above: arms the two hardware sound
 * channels tied to the same gauge (`REG_SOUND1CNT_L`/`0x0400000C` gets
 * one of two fixed sweep/tone words depending on the current
 * `gUnknown_030014C1`/`030014C0` one-shot flags, which then both get
 * cleared/toggled), then derives a shared volume scale from
 * `gUnknown_030014CC` (`sub_803ADB4`-scaled against a `0x5500` divisor)
 * and a `sub_8029EB4()` base, writes `REG_SOUND1CNT_H`/`0x04000028` and
 * `REG_SOUND2CNT_L`/`0x0400002C` each as a fixed ceiling minus that
 * scaled `sub_8029E98()` value, and finally seeds
 * `REG_SOUND1CNT_X`/`REG_SOUND2CNT_H`/`0x04000020` (a 4-halfword run:
 * frequency, 0, 0, frequency again) with the `sub_803ADB4`-scaled
 * frequency computed earlier from `gUnknown_030014C4`.
 *
 * Written as NAKED asm for the same register-pressure reasons as
 * `sub_802D9A8` above. */
NAKED void sub_802DA68(void)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "ldr r0, 1f\n\t"
        "ldrb r1, [r0]\n\t"
        "add r3, r0, #0\n\t"
        "cmp r1, #0\n\t"
        "beq 7f\n\t"
        "ldr r0, 2f\n\t"
        "ldrb r1, [r0]\n\t"
        "add r2, r0, #0\n\t"
        "cmp r1, #0\n\t"
        "beq 5f\n\t"
        "ldr r1, 3f\n\t"
        "ldr r4, 4f\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
        "1: .4byte gUnknown_030014C1\n"
        "2: .4byte gUnknown_030014C0\n"
        "3: .4byte 0x0400000C\n"
        "4: .4byte 0x00001A09\n"
        "5:\n\t"
        "ldr r1, 8f\n\t"
        "ldr r4, 9f\n\t"
        "6:\n\t"
        "add r0, r4, #0\n\t"
        "strh r0, [r1]\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r3]\n\t"
        "mov r0, #1\n\t"
        "ldrb r1, [r2]\n\t"
        "eor r0, r1\n\t"
        "strb r0, [r2]\n\t"
        "7:\n\t"
        "ldr r5, 10f\n\t"
        "ldr r0, [r5]\n\t"
        "lsl r0, r0, #8\n\t"
        "mov r1, #0xaa\n\t"
        "lsl r1, r1, #7\n\t"
        "bl sub_803ADB4\n\t"
        "add r4, r0, #0\n\t"
        "bl sub_8029EB4\n\t"
        "add r6, r0, #0\n\t"
        "ldr r0, 11f\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #4\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r5]\n\t"
        "bl sub_803ADB4\n\t"
        "add r0, r0, r6\n\t"
        "ldr r2, 12f\n\t"
        "add r1, r0, #0\n\t"
        "mul r1, r4, r1\n\t"
        "asr r1, r1, #8\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #7\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [r2]\n\t"
        "bl sub_8029E98\n\t"
        "ldr r2, 13f\n\t"
        "add r1, r0, #0\n\t"
        "mul r1, r4, r1\n\t"
        "asr r1, r1, #8\n\t"
        "mov r0, #0x88\n\t"
        "lsl r0, r0, #7\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [r2]\n\t"
        "ldr r0, 14f\n\t"
        "strh r4, [r0]\n\t"
        "add r0, #2\n\t"
        "mov r1, #0\n\t"
        "strh r1, [r0]\n\t"
        "add r0, #2\n\t"
        "strh r1, [r0]\n\t"
        "add r0, #2\n\t"
        "strh r4, [r0]\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "8: .4byte 0x0400000C\n"
        "9: .4byte 0x00001B09\n"
        "10: .4byte gUnknown_030014CC\n"
        "11: .4byte gUnknown_030014C4\n"
        "12: .4byte 0x04000028\n"
        "13: .4byte 0x0400002C\n"
        "14: .4byte 0x04000020\n"
    );
}

asm(".align 2, 0");
