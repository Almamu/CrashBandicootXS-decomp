#include "core.h"

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem (see game_loop6.c's header comment and
 * docs/matching/issue-12-physics-collision.md). `sub_0800D18C` and
 * `sub_800E08C` between game_loop6.c's `sub_800D040` and this file's
 * `sub_800E494` are left untouched raw - see
 * asm/code_3_2_17_d18c.s and the write-up doc. */

extern void *sub_801070C(void *obj);
extern void *sub_8010708(void *obj);

/* Walks `obj`'s doubly-linked neighbor list both ways (`sub_801070C`
 * = next, `sub_8010708` = prev - the same "get next"-style pair
 * `docs/rom_map.md` ties to the physics/collision subsystem's
 * `sub_0800D18C`), clearing each visited neighbor's `+0x58` byte
 * whenever its own `+0x4d & 0x7f` state byte is 0.
 *
 * Written as NAKED asm, not plain C: every operation, operand and
 * branch matches the ROM one-for-one except a single recurring
 * instruction-order swap - the ROM materializes the `0x7f` mask
 * immediate *before* the `ldrb` byte load (`movs r2,#0x7f;
 * ldrb r0,[r0]; ands r2,r0`), while gcc 2.9 always schedules the load
 * first regardless of source operand order - see
 * docs/matching/issue-12-physics-collision.md. Transcribed
 * instruction-for-instruction from the ROM disassembly instead. */
NAKED void sub_800E494(void *obj)
{
    asm(
        "push {r4, lr}\n\t"
        "add r4, r0, #0\n\t"
        "b 3f\n\t"
    "1:\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x4d\n\t"
        "mov r2, #0x7f\n\t"
        "ldrb r0, [r0]\n\t"
        "and r2, r0\n\t"
        "cmp r2, #0\n\t"
        "bne 2f\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x58\n\t"
        "strb r2, [r0]\n\t"
    "2:\n\t"
        "add r0, r1, #0\n\t"
    "3:\n\t"
        "bl sub_801070C\n\t"
        "add r1, r0, #0\n\t"
        "cmp r1, #0\n\t"
        "bne 1b\n\t"
        "add r0, r4, #0\n\t"
        "b 6f\n\t"
    "4:\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x4d\n\t"
        "mov r2, #0x7f\n\t"
        "ldrb r0, [r0]\n\t"
        "and r2, r0\n\t"
        "cmp r2, #0\n\t"
        "bne 5f\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x58\n\t"
        "strb r2, [r0]\n\t"
    "5:\n\t"
        "add r0, r1, #0\n\t"
    "6:\n\t"
        "bl sub_8010708\n\t"
        "add r1, r0, #0\n\t"
        "cmp r1, #0\n\t"
        "bne 4b\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}

/* Same bidirectional-neighbor walk as `sub_800E494` above, but instead
 * of clearing `+0x58` it sets it to 1 and, for the forward
 * (`sub_801070C`) direction only, also debits `ctx+4` and credits
 * `ctx+0xc` by `ctx+0xc`'s *original* value (`step`, cached once
 * before the loop - the ROM keeps it in `r5` throughout, since
 * `ctx+0xc` itself is mutated inside the loop and can't be re-read);
 * the reverse (`sub_8010708`) direction only credits `ctx+0xc`. Reads
 * as redistributing some accumulated "budget" field between a
 * just-touched neighbor and the rest of the chain.
 *
 * Written as NAKED asm, not plain C: same single resistant gap as
 * `sub_800E494` above (the `0x7f`-mask-before-`ldrb` instruction-order
 * swap, 4 occurrences here) - see
 * docs/matching/issue-12-physics-collision.md. Transcribed
 * instruction-for-instruction from the ROM disassembly instead. */
NAKED void sub_800E4E4(void *obj, void *ctx)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r6, r0, #0\n\t"
        "add r4, r1, #0\n\t"
        "ldr r5, [r4, #0xc]\n\t"
        "bl sub_801070C\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "beq 1f\n\t"
        "mov r7, #1\n\t"
    "2:\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x58\n\t"
        "strb r7, [r0]\n\t"
        "ldr r0, [r4, #4]\n\t"
        "sub r0, r0, r5\n\t"
        "str r0, [r4, #4]\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "add r0, r0, r5\n\t"
        "str r0, [r4, #0xc]\n\t"
    "3:\n\t"
        "add r0, r2, #0\n\t"
        "bl sub_801070C\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "bne 2b\n\t"
    "1:\n\t"
        "add r0, r6, #0\n\t"
        "bl sub_8010708\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "beq 6f\n\t"
        "mov r6, #1\n\t"
    "4:\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 5f\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x58\n\t"
        "strb r6, [r0]\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "add r0, r0, r5\n\t"
        "str r0, [r4, #0xc]\n\t"
    "5:\n\t"
        "add r0, r2, #0\n\t"
        "bl sub_8010708\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "bne 4b\n\t"
    "6:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
