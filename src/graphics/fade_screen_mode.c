#include "core.h"

/* Sits right after the still-raw remainder of asm/code_3_1_6.s'
 * SIO/link-cable and overlay_ui functions and before the small
 * fade/screen-mode utility cluster this file documents - see
 * docs/rom_map.md "A fourth thing in this file". This cluster's
 * parked functions interleave with the matched ones - see
 * docs/matching.md for the full split. */

extern void sub_80013FC(s32 factor);
extern void sub_80006A8(void *arg0);
extern u16 gUnknown_03000A80[512];
extern u16 gUnknown_03000E80[512];

/* Backs the real palette (0x05000000) up into `gUnknown_03000A80`,
 * then, for each factor 0/2/4/.../16, blends it toward black via the
 * already-matched `sub_80013FC` into `gUnknown_03000E80` and DMAs
 * that result into the real palette, waiting one VBlank between each
 * step - a textbook fade-to-black animation. Once fully faded, sets
 * up the hardware blend registers (`REG_BLDCNT`/`REG_BLDY`) and
 * restores the original backed-up palette.
 *
 * Written as NAKED asm, not plain C: a full C reconstruction (kept in
 * git history) got every field/branch/call right, but the ROM caches
 * the blended-buffer address (`gUnknown_03000E80`) in a register
 * across the loop while recomputing the other two DMA fields (the
 * 0x05000000 destination and the 0x80000200 control word) fresh every
 * iteration, plus an extra "rename" copy of the DMA register pointer
 * right before the loop - this compiler's loop-invariant hoisting pass
 * never reproduces that specific split: giving the buffer address its
 * own local gets it cached consistently but also hoists at least one
 * of the other two fields (or, with an inline-asm register-clobber
 * barrier on the destination value to block just that hoist, drops the
 * buffer caching instead). Same root cause as the `sub_8009150`
 * loop-invariant-hoisting gap documented in docs/matching.md, applied
 * to a memory-mapped-I/O DMA setup instead of a pointer computation.
 * Every instruction below is confirmed byte-identical to the ROM (the
 * paragraph above is that derivation) - full NAKED transcription, like
 * this project's other hard-compiler-limitation cases (see
 * `src/util/printf_util.c`'s `sub_8000CBC` for the established
 * pattern), is more honest than continuing to chase this one loop's
 * register-caching split through plain C. */
NAKED void sub_80014A4(void)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "ldr r1, 2f\n\t"
        "mov r0, #0xa0\n\t"
        "lsl r0, r0, #0x13\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, 3f\n\t"
        "str r0, [r1, #4]\n\t"
        "ldr r0, 4f\n\t"
        "str r0, [r1, #8]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "mov r5, #0\n\t"
        "add r4, r1, #0\n\t"
        "ldr r6, 5f\n\t"
    "1:\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_80013FC\n\t"
        "bl sub_80006A8\n\t"
        "str r6, [r4]\n\t"
        "mov r3, #0xa0\n\t"
        "lsl r3, r3, #0x13\n\t"
        "str r3, [r4, #4]\n\t"
        "ldr r2, 4f\n\t"
        "str r2, [r4, #8]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "add r5, #2\n\t"
        "cmp r5, #0x10\n\t"
        "ble 1b\n\t"
        "ldr r1, 6f\n\t"
        "mov r0, #0xff\n\t"
        "strh r0, [r1]\n\t"
        "add r1, #4\n\t"
        "mov r0, #0x10\n\t"
        "strh r0, [r1]\n\t"
        "ldr r0, 2f\n\t"
        "ldr r1, 3f\n\t"
        "str r1, [r0]\n\t"
        "str r3, [r0, #4]\n\t"
        "str r2, [r0, #8]\n\t"
        "ldr r0, [r0, #8]\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n\t"
    "2: .4byte 0x040000D4\n\t"
    "3: .4byte gUnknown_03000A80\n\t"
    "4: .4byte 0x80000200\n\t"
    "5: .4byte gUnknown_03000E80\n\t"
    "6: .4byte 0x04000050\n\t"
    );
}
asm(".align 2, 0");

/* `gUnknown_030007E8.field_0 != -1` - the same "idle" sentinel
 * documented on the struct in fade_util.c, exposed here as a plain
 * s32 read (this file doesn't share that struct definition, per this
 * project's per-file raw-offset convention). */
extern s32 gUnknown_030007E8;

s32 sub_8001510(void)
{
    return gUnknown_030007E8 != -1;
}
