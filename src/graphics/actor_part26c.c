#include "core.h"

/* Same boss-weapon subsystem as actor_part20.c/actor_part26b.c - see
 * actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Per docs/rom_map.md ("A new mechanism: a procedurally-generated VRAM
 * fill-level meter"): a near-identical twin of the still-raw
 * `sub_80336CC` (outside this chunk, issue #62's range) that
 * procedurally generates a vertical meter/fill-level tile graphic.
 * Indexes `gStaticData_08167AD4`'s per-level table via the
 * `gUnknown_03001528`/`gUnknown_0300152C` fields (row/column counts,
 * both capped near 32), sums four rows' worth of heights into
 * `gUnknown_03001530`, computes `0xFF - sum` as a fill level, and DMAs
 * the result - packed two nibbles per byte from each row's raw byte
 * data - as 4-bit tile data into VRAM (`0x06008000`), a health-bar/
 * water-level-style meter built fresh per frame from up to 4 rows of
 * `gUnknown_03001580`-indexed level data.
 *
 * Semantics are understood at the level above, but this is transcribed
 * as NAKED asm rather than plain C: the inner nibble-packing loop
 * holds `r8`/`sb`/`sl`/`ip` live simultaneously (the row's raw byte
 * pointer, the running bit-position count, the outer/inner loop
 * indices and the output cursor all survive the whole loop body plus
 * its own internal branches), the same many-high-register allocation
 * gcc-2.9 difficulty already documented throughout this project for
 * `sub_8006600`/`sub_80372BC`/`sub_8038538` and others - not something
 * a plain-C reconstruction can coax this compiler into reproducing
 * register-for-register. Mechanical, byte-verified transcription. */
extern s32 gUnknown_03001528;
extern s32 gUnknown_0300152C;
extern s32 gUnknown_03001530;
extern u8 gStaticData_08167AD4[];
extern void *gUnknown_03001580[];

NAKED void sub_8031604(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x14\n\t"
        "mov r5, #0\n\t"
        "mov r3, #0x81\n\t"
        "lsl r3, r3, #2\n\t"
        "ldr r0, 1f\n\t"
        "ldr r1, 2f\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "mul r0, r2\n\t"
        "add r0, #1\n\t"
        "lsr r0, r0, #1\n\t"
        "lsl r0, r0, #2\n\t"
        "str r0, [sp, #0x10]\n\t"
        "ldr r7, 3f\n\t"
        "ldr r4, 4f\n\t"
        "mov r1, sp\n\t"
        "ldr r6, 5f\n\t"
        "mov r2, #3\n\t"
    "6:\n\t"
        "add r0, r3, r4\n\t"
        "ldr r0, [r0]\n\t"
        "str r0, [r1]\n\t"
        "add r5, r5, r0\n\t"
        "add r3, #4\n\t"
        "add r0, r3, r4\n\t"
        "stm r6!, {r0}\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "add r3, r3, r0\n\t"
        "ldm r1!, {r0}\n\t"
        "lsl r0, r0, #5\n\t"
        "add r3, r3, r0\n\t"
        "sub r2, #1\n\t"
        "cmp r2, #0\n\t"
        "bge 6b\n\t"
        "mov r0, #0xff\n\t"
        "sub r0, r0, r5\n\t"
        "str r0, [r7]\n\t"
        "lsl r0, r0, #6\n\t"
        "ldr r1, 7f\n\t"
        "add r3, r0, r1\n\t"
        "mov r2, #0\n\t"
    "8:\n\t"
        "lsl r1, r2, #2\n\t"
        "ldr r4, 5f\n\t"
        "add r0, r1, r4\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, sp\n\t"
        "mov sb, r3\n\t"
        "ldr r3, [sp, #0x10]\n\t"
        "add r5, r0, r3\n\t"
        "ldr r1, [r1]\n\t"
        "mov r8, r1\n\t"
        "mov r4, #0\n\t"
        "mov ip, r4\n\t"
        "lsl r0, r1, #4\n\t"
        "add r2, #1\n\t"
        "mov sl, r2\n\t"
        "cmp ip, r0\n\t"
        "bge 9f\n\t"
        "mov r6, #0xf\n\t"
        "mov r7, #0x10\n\t"
    "10:\n\t"
        "ldrb r0, [r5]\n\t"
        "add r4, r6, #0\n\t"
        "and r4, r0\n\t"
        "mov r1, #0\n\t"
        "cmp r4, #0\n\t"
        "beq 11f\n\t"
        "add r1, r7, #0\n\t"
        "orr r1, r4\n\t"
    "11:\n\t"
        "add r4, r1, #0\n\t"
        "lsr r0, r0, #4\n\t"
        "and r0, r6\n\t"
        "add r5, #1\n\t"
        "mov r1, #0\n\t"
        "cmp r0, #0\n\t"
        "beq 12f\n\t"
        "add r1, r7, #0\n\t"
        "orr r1, r0\n\t"
    "12:\n\t"
        "add r0, r1, #0\n\t"
        "ldrb r3, [r5]\n\t"
        "add r1, r6, #0\n\t"
        "and r1, r3\n\t"
        "mov r2, #0\n\t"
        "cmp r1, #0\n\t"
        "beq 13f\n\t"
        "add r2, r7, #0\n\t"
        "orr r2, r1\n\t"
    "13:\n\t"
        "add r1, r2, #0\n\t"
        "lsr r3, r3, #4\n\t"
        "and r3, r6\n\t"
        "add r5, #1\n\t"
        "mov r2, #0\n\t"
        "cmp r3, #0\n\t"
        "beq 14f\n\t"
        "add r2, r7, #0\n\t"
        "orr r2, r3\n\t"
    "14:\n\t"
        "lsl r0, r0, #8\n\t"
        "orr r0, r4\n\t"
        "lsl r1, r1, #0x10\n\t"
        "orr r1, r0\n\t"
        "lsl r0, r2, #0x18\n\t"
        "orr r0, r1\n\t"
        "mov r1, sb\n\t"
        "add r1, #4\n\t"
        "mov sb, r1\n\t"
        "sub r1, #4\n\t"
        "stm r1!, {r0}\n\t"
        "mov r3, #1\n\t"
        "add ip, r3\n\t"
        "mov r4, r8\n\t"
        "lsl r0, r4, #4\n\t"
        "cmp ip, r0\n\t"
        "blt 10b\n\t"
    "9:\n\t"
        "mov r3, sb\n\t"
        "mov r2, sl\n\t"
        "cmp r2, #3\n\t"
        "ble 8b\n\t"
        "add sp, #0x14\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001528\n"
    "2: .4byte gUnknown_0300152C\n"
    "3: .4byte gUnknown_03001530\n"
    "4: .4byte gStaticData_08167AD4\n"
    "5: .4byte gUnknown_03001580\n"
    "7: .4byte 0x06008000\n"
    );
}
