#include "core.h"

/* GitHub issue #34, UpdateGameFrame-MainLoop cluster (docs/rom_map.md).
 * `self` throughout is the same per-level state object
 * `sub_8022BF0`/`sub_8022CA0` (game_loop.c) and the
 * `self+0x80`/`+0x84`/... accessor family (game_loop2.c) operate on -
 * every caller passes `*gUnknown_030012C0`.
 *
 * NAKED, not plain C: semantics are fully traced against the ROM -
 * clears `self+0x8c`/`0x90`-`0xa0`, tears down the two actor slots at
 * `self+0x1b8`/`0x1bc` (via `sub_80087C0`/`sub_80087B4`/`sub_800872C`,
 * the same OAM-trio teardown `sub_802375C`, game_loop39.c, already
 * uses) when non-null, feeds `self+0x1bc`'s own `+0x20`-table/
 * `+0x2d`-tag hitbox record into `sub_8006D08` (the tile-asset-cache
 * slot loader), then walks `gUnknown_030012EC`'s array firing each
 * entry's `+0x48`/`0x4c` `sub_803AD7C` trampoline - a result of `2`
 * fires the `+0x28`/`0x2c` trampoline too, and either flags the entry
 * for despawn (`sub_8011448`) or marks it "seen" (`+0xc` bit 0) and
 * sets its bit in the `gUnknown_030012B4+0x108` collision bitmap (the
 * same inline idiom `sub_80072D8`, graphics.c, uses on a `struct
 * actor`). Skips everything past the two clears when `self+0xdc`'s
 * level object is already in state 3.
 *
 * A plain C reconstruction gets every field offset, branch, and call
 * argument byte-identical in isolation - register pins plus small
 * anchored `asm volatile` blocks reproduced each individual quirk
 * (the ROM's own `self+0x8c`/`self+0x90` address computed into two
 * separate registers rather than this compiler's `(self+0x8c)+4`
 * reuse, a running pointer walked through the zero-fill straight on
 * to `self+0xdc` instead of a fresh recompute, several
 * constant-materialized-before-address orderings, `part->table+0x48`
 * advanced in place and reused for `fn` rather than re-derived, a
 * `flags |= 1` built with the constant loaded first matching
 * `sub_80072D8`'s own idiom, and `self+8` read twice - once for the
 * sentinel compare, again for the bitmap math). But the loop's own
 * `0xffff` sentinel has to live in r7 for the ROM's whole array walk,
 * and this compiler's `register T x asm("r7")` never adds an
 * inline-asm-clobbered r7 to the function's own push/pop list (the
 * same confirmed toolchain gap as `sub_8010674`'s `success` local,
 * game_loop23.c, and this doc's own `sub_8025A64` entry) - a *plain*
 * (non-pinned) sentinel local happens to dodge the bug only in
 * isolation from the rest of this loop's other anchored blocks; once
 * every other quirk above is also anchored, register pressure shifts
 * enough that the natural allocator stops landing the sentinel on r7
 * at all. Transcribed straight from the confirmed-correct ROM
 * disassembly instead of re-chasing this combination further - see
 * docs/matching/issue-34-game-loop-8022d50-80255d4.md. */
NAKED void sub_8022D50(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r5, r0, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_80231EC\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x8c\n\t"
        "mov r1, #0\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r2]\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x90\n\t"
        "str r1, [r0]\n\t"
        "add r0, #4\n\t"
        "str r1, [r0]\n\t"
        "add r0, #4\n\t"
        "str r1, [r0]\n\t"
        "add r0, #4\n\t"
        "str r1, [r0]\n\t"
        "add r0, #4\n\t"
        "str r1, [r0]\n\t"
        "add r0, #0x3c\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #8]\n\t"
        "cmp r0, #3\n\t"
        "bne 1f\n\t"
        "b 6f\n\t"
    "1:\n\t"
        "mov r1, #0xdc\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r5, r1\n\t"
        "ldr r4, [r0]\n\t"
        "cmp r4, #0\n\t"
        "beq 2f\n\t"
        "mov r0, #7\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x2d\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
    "2:\n\t"
        "mov r2, #0xde\n\t"
        "lsl r2, r2, #1\n\t"
        "add r5, r5, r2\n\t"
        "ldr r4, [r5]\n\t"
        "cmp r4, #0\n\t"
        "beq 3f\n\t"
        "mov r0, #0xc\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x2d\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r3, [r5]\n\t"
        "add r1, r3, #0\n\t"
        "add r1, #0x29\n\t"
        "ldrb r1, [r1]\n\t"
        "lsl r1, r1, #0x1c\n\t"
        "lsr r1, r1, #0x1c\n\t"
        "ldr r2, [r3, #0x20]\n\t"
        "add r3, #0x2d\n\t"
        "ldr r4, [r2]\n\t"
        "ldrb r5, [r3]\n\t"
        "lsl r2, r5, #3\n\t"
        "sub r2, r2, r5\n\t"
        "lsl r2, r2, #2\n\t"
        "add r2, r2, r4\n\t"
        "ldrb r2, [r2, #0x14]\n\t"
        "bl sub_8006D08\n\t"
    "3:\n\t"
        "bl sub_8010804\n\t"
        "mov r6, #0\n\t"
        "ldr r0, 8f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "cmp r6, r0\n\t"
        "bge 6f\n\t"
        "ldr r7, 9f\n\t"
    "4:\n\t"
        "ldr r0, 8f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #0xc]\n\t"
        "lsl r0, r6, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r4, [r0]\n\t"
        "add r5, r4, #0\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x48\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "cmp r0, #2\n\t"
        "bne 5f\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "mov r2, #0x28\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #0x2c]\n\t"
        "bl sub_803AD7C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 10f\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #1\n\t"
        "bl sub_8011448\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
    "7: .4byte gUnknown_030012B8\n"
    "8: .4byte gUnknown_030012EC\n"
    "9: .4byte 0x0000FFFF\n"
    "10:\n\t"
        "mov r0, #1\n\t"
        "ldrb r4, [r5, #0xc]\n\t"
        "orr r0, r4\n\t"
        "strb r0, [r5, #0xc]\n\t"
        "ldrh r0, [r5, #8]\n\t"
        "cmp r0, r7\n\t"
        "beq 5f\n\t"
        "ldrh r3, [r5, #8]\n\t"
        "ldr r0, 11f\n\t"
        "ldr r2, [r0]\n\t"
        "add r0, r3, #0\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r1, r0, #2\n\t"
        "mov r4, #0x84\n\t"
        "lsl r4, r4, #1\n\t"
        "add r2, r2, r4\n\t"
        "add r2, r2, r1\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r0, r3, r0\n\t"
        "mov r1, #1\n\t"
        "lsl r1, r0\n\t"
        "ldr r0, [r2]\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2]\n\t"
    "5:\n\t"
        "add r6, #1\n\t"
        "ldr r0, 12f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "cmp r6, r0\n\t"
        "blt 4b\n\t"
    "6:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "11: .4byte gUnknown_030012B4\n"
    "12: .4byte gUnknown_030012EC\n"
    );
}
