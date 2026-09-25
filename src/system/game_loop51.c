#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). Sits between the matched
 * `sub_8010480` (game_loop35.c) and `sub_8010674` (game_loop23.c) in
 * ROM, so it needs its own file - see docs/workflow.md's "one file
 * per contiguous ROM region" rule. `self` throughout is the same
 * "collision box" object every other function in this subsystem
 * operates on - offsets kept raw rather than a named struct, matching
 * every already-matched sibling in this file family
 * (game_loop22.c-game_loop35.c).
 *
 * A per-frame state-machine tick. While `self+0x4f` (a per-object
 * throttle counter several siblings in this family also drive) is
 * nonzero, decrements it and, only for the frame it does so,
 * dispatches once more on `self+0x4e` (the settle-state byte):
 * - `0x13`-`0x15`: re-enters the edge-settle chain (`sub_800F8E0`),
 *   also arming the global one-shot rescan flag `gUnknown_030012B0`
 *   (the same flag `sub_800FC70`, game_loop32.c, reads).
 * - `0xf`: re-triggers `sub_800F990` when `self+0x4d`'s low 7 bits
 *   are already 0.
 * - `0xc`: once `self+0x4f` has reached 0 this frame, clears
 *   `self+0x50`.
 * - `3`: re-triggers `sub_800F4F4`.
 *
 * Unconditionally afterwards: while `self+0x4e == 0xc`, counts
 * `self+0x48` down toward 0; always calls `sub_800FC70` (the
 * position-wrap advance). Then, if `self+0x4d`'s bit 7 is set and
 * `self+0x38` is nonzero, re-derives `self+0x30`'s index via the same
 * `self+0x20`-pointer-to-manager/`self+0x2d`-tag/0x1c-stride hitbox-
 * record clamp `sub_8010480` uses, clears `self+0x38` and
 * `self+0x4d`'s bit 7, and clears the "recently touched" object's
 * (`gUnknown_030012D8`) own `+0x80` byte - then, depending on
 * `self+0x4e`: state 6 settles to state 7, tags `self+0x2d = 0x20`,
 * runs the `sub_80087C0`/`sub_80087B4`/`sub_800872C` triplet, then
 * folds the low nibble of a `sub_8006DF8` tile-cache lookup (keyed by
 * the freshly-retagged hitbox record's own `+0x14`) into `self+0x29`;
 * state 3 just tags `0x20` and runs the same triplet. If bit 7 was
 * clear instead, `self+0x4d`'s low 7 bits == 1 triggers
 * `sub_800F798`. Finally, unconditionally, calls `sub_8008044` and
 * hands `self+0x18`'s table's own `+0x60`/`+0x64` offset/function-
 * pointer pair off to the `sub_803AD7C` table-trampoline (the same
 * convention `sub_8007048`/`sub_80070D4`, graphics.c, establish).
 *
 * Written as NAKED asm, not plain C: this function threads a long,
 * ever-shifting set of field *addresses* (not just values) through
 * r0/r1/r6/r2/r5/r8/ip across several intervening `bl` calls, each
 * one either reused verbatim for a later load/store (e.g. the
 * `self+0x4f` address computed once into r1 and reused for both the
 * throttle check and its decrement store) or deliberately
 * *recomputed* fresh a few instructions later even though an
 * equivalent address is still sitting in a live register (e.g.
 * `self+0x4e`'s address is recomputed fresh for the `0xc`/`3` checks
 * rather than reusing the r6 copy the immediately-preceding `0xf`
 * check made) - a finer-grained, less consistent version of the same
 * "which anonymous scratch register" register-pressure gap that
 * already forced `sub_8007B00`/`sub_8007B98` (`actor_part.c`) and
 * `sub_8010B6C` (game_loop28.c) fully NAKED. A plain-C attempt
 * (register-pinned per nested scope, following `sub_8010480`'s own
 * successful technique for its near-identical hitbox-record clamp)
 * matched the first ~10 instructions exactly but diverged as soon as
 * this compiler's own CSE/register-allocation choices no longer lined
 * up with the ROM's - see docs/matching/issue-13-fc70-continuation.md
 * for the specific gotchas hit. Every instruction below is checked
 * byte-identical to the ROM. */
NAKED void sub_80104E4(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "add r4, r0, #0\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4f\n\t"
        "ldrb r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "sub r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x4e\n\t"
        "ldrb r1, [r0]\n\t"
        "add r6, r0, #0\n\t"
        "cmp r1, #0x12\n\t"
        "ble 2f\n\t"
        "cmp r1, #0x15\n\t"
        "bgt 2f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800F8E0\n\t"
        "ldr r1, 1f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012B0\n"
    "2:\n\t"
        "ldrb r6, [r6]\n\t"
        "cmp r6, #0xf\n\t"
        "bne 3f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800F990\n\t"
        "b 5f\n\t"
    "3:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x4e\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0xc\n\t"
        "bne 4f\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x4f\n\t"
        "ldrb r1, [r0]\n\t"
        "cmp r1, #0\n\t"
        "bne 5f\n\t"
        "add r0, #1\n\t"
        "strb r1, [r0]\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "cmp r0, #3\n\t"
        "bne 5f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800F4F4\n\t"
    "5:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x4e\n\t"
        "add r6, r0, #0\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #0xc\n\t"
        "bne 6f\n\t"
        "ldr r0, [r4, #0x48]\n\t"
        "cmp r0, #0\n\t"
        "ble 6f\n\t"
        "sub r0, #1\n\t"
        "str r0, [r4, #0x48]\n\t"
    "6:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800FC70\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x4d\n\t"
        "ldrb r1, [r2]\n\t"
        "mov r0, #0x80\n\t"
        "and r0, r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 11f\n\t"
        "mov r1, #0x38\n\t"
        "add r1, r1, r4\n\t"
        "mov r8, r1\n\t"
        "ldrb r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "beq 12f\n\t"
        "mov r3, #0\n\t"
        "mov ip, r3\n\t"
        "ldr r0, [r4, #0x20]\n\t"
        "add r5, r4, #0\n\t"
        "add r5, #0x2d\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r7, [r5]\n\t"
        "lsl r0, r7, #3\n\t"
        "sub r0, r0, r7\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "cmp r3, r0\n\t"
        "blt 7f\n\t"
        "sub r0, #1\n\t"
        "mov ip, r0\n\t"
    "7:\n\t"
        "mov r0, ip\n\t"
        "str r0, [r4, #0x30]\n\t"
        "mov r1, r8\n\t"
        "strb r3, [r1]\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r7, [r2]\n\t"
        "and r0, r0, r7\n\t"
        "strb r0, [r2]\n\t"
        "ldr r0, 8f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x80\n\t"
        "strb r3, [r0]\n\t"
        "ldrb r0, [r6]\n\t"
        "cmp r0, #6\n\t"
        "bne 10f\n\t"
        "mov r0, #7\n\t"
        "strb r0, [r6]\n\t"
        "mov r0, #0x20\n\t"
        "strb r0, [r5]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "ldr r0, [r4, #0x20]\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r2, [r5]\n\t"
        "lsl r0, r2, #3\n\t"
        "sub r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrb r1, [r1, #0x14]\n\t"
        "bl sub_8006DF8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r1, r3\n\t"
        "orr r1, r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "b 12f\n\t"
        ".align 2, 0\n"
    "8: .4byte gUnknown_030012D8\n"
    "9: .4byte gUnknown_030012B8\n"
    "10:\n\t"
        "cmp r0, #3\n\t"
        "bne 12f\n\t"
        "mov r0, #0x20\n\t"
        "strb r0, [r5]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "b 12f\n\t"
    "11:\n\t"
        "mov r0, #0x7f\n\t"
        "and r0, r0, r1\n\t"
        "cmp r0, #1\n\t"
        "bne 12f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800F798\n\t"
    "12:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8008044\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x60\n\t"
        "mov r7, #0\n\t"
        "ldrsh r0, [r1, r7]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
