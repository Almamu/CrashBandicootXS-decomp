#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `sub_8010A00` right after
 * this file is already matched in game_loop26.c. `self` throughout
 * this file is the same actor/"collision box" object every other
 * function in this subsystem operates on - offsets `0`/`4`/`0xc` here
 * line up with `struct actor`'s own `x`/`y`/`flags` fields
 * (include/actor.h), kept as raw offsets rather than that struct type
 * to stay consistent with every already-matched sibling in this file
 * family (game_loop22.c-game_loop29.c), which do the same. */

extern void *sub_8010708(void *selfArg);
extern void *sub_801070C(void *selfArg);
extern void sub_0800D18C(void *selfArg);

/* Walks the "get prev" neighbor-list chain (`sub_801070C`) starting at
 * `self`, returning the furthest node reachable while every node
 * visited (other than `self` itself) has a `+0x4d & 0x7f` state != 1 -
 * the walk stops (returning the last accepted node) as soon as it
 * would step onto a node in state 1, or runs out of neighbors. If
 * `self` has no "prev" neighbor at all, or that first neighbor is
 * already in state 1, falls back to returning `self` itself:
 *
 *   cur = sub_801070C(self);
 *   if (cur == NULL) return self;
 *   if ((cur[0x4d] & 0x7f) == 1) return self;
 *   for (;;) {
 *       next = sub_801070C(cur);
 *       if (next == NULL) return cur;
 *       if ((next[0x4d] & 0x7f) == 1) return cur;
 *       cur = next;
 *   }
 *
 * Written as NAKED asm, not plain C: this compiler's cross-jump pass
 * merges the loop's two `return cur;` sites (`adds r0,r4,#0; b <exit>`
 * is byte-identical at both) into one shared tail, 4 bytes shorter
 * than the ROM, which keeps them as two separate physical copies - the
 * same gap as `sub_800FDC8` (game_loop33.c). The mask check
 * (`self+0x4d & 0x7f`) also keeps the loaded byte in r1 throughout
 * (`adds r1,ptr,#0; adds r1,#0x4d; movs r0,#0x7f; ldrb r1,[r1]; ands
 * r0,r1`), the same "mask materializes before the byte load" gap
 * already documented for `sub_800FEB0`/`sub_801085C`
 * (game_loop22.c/game_loop24.c). Every instruction below is checked
 * byte-identical to the ROM. */
NAKED void *sub_8010914(void *selfArg)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "add r5, r0, #0\n\t"
        "bl sub_801070C\n\t"
        "add r4, r0, #0\n\t"
        "cmp r4, #0\n\t"
        "beq 1f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r0, r1\n\t"
        "cmp r0, #1\n\t"
        "bne 3f\n\t"
    "1:\n\t"
        "add r0, r5, #0\n\t"
        "b 5f\n\t"
    "2:\n\t"
        "add r0, r4, #0\n\t"
        "b 5f\n\t"
    "3:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_801070C\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "beq 2b\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r0, r1\n\t"
        "cmp r0, #1\n\t"
        "beq 2b\n\t"
        "add r4, r2, #0\n\t"
        "b 3b\n\t"
    "5:\n\t"
        "pop {r4, r5}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}

/* Same walk as `sub_8010914`, but over the "get next" chain
 * (`sub_8010708`) instead of "get prev". Same NAKED-transcription
 * reasoning as that function. */
NAKED void *sub_801095C(void *selfArg)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "add r5, r0, #0\n\t"
        "bl sub_8010708\n\t"
        "add r4, r0, #0\n\t"
        "cmp r4, #0\n\t"
        "beq 1f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r0, r1\n\t"
        "cmp r0, #1\n\t"
        "bne 3f\n\t"
    "1:\n\t"
        "add r0, r5, #0\n\t"
        "b 5f\n\t"
    "2:\n\t"
        "add r0, r4, #0\n\t"
        "b 5f\n\t"
    "3:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8010708\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "beq 2b\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r0, r1\n\t"
        "cmp r0, #1\n\t"
        "beq 2b\n\t"
        "add r4, r2, #0\n\t"
        "b 3b\n\t"
    "5:\n\t"
        "pop {r4, r5}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}

/* Unless `self`'s own `+0x4d & 0x7f` state is 1, and `testX`/`testY`
 * (both raw, same Q8 scale as `self`'s own `+0`/`+4` position pair)
 * are both within `0x3fff` of `self`'s position, and `self`'s `+0x4e`
 * byte isn't `5`, fires `sub_0800D18C(self)` - the physics/collision
 * subsystem's own collision-response commit
 * (docs/matching/issue-12-physics-collision.md). Always clears
 * `self`'s own `+0xc` flags bit 3 before returning, unconditionally. */
s32 sub_80109A4(void *selfArg, u32 unused1, s32 testX, s32 testY)
{
    /* Pinned to r4: the ROM keeps `self` in r4 for the whole function
     * (only the transient mask-check scratch below uses r5/r6/ip), and
     * this compiler's own unforced allocator drifts it onto r6 instead
     * once the tail's `loaded asm("r6")` pin is in scope. */
    register u8 *self asm("r4") = selfArg;
    u32 masked;

    asm volatile(
        "mov r0, #0x4d\n\t"
        "add r0, r0, %1\n\t"
        "mov ip, r0\n\t"
        "mov r0, #0x7f\n\t"
        "mov r5, ip\n\t"
        "ldrb r5, [r5]\n\t"
        "and r0, r0, r5\n\t"
        : "=r"(masked)
        : "r"(self)
        : "r5", "ip", "cc"
    );

    if (masked != 1) {
        s32 dx = *(s32 *)self - testX;
        if (dx < 0) {
            dx = -dx;
        }
        /* Register-pinned, and declared only here (not at the top of
         * the enclosing block): the ROM loads the literal-pool
         * `0x3fff` bound into r2 only *after* `testX` (the incoming
         * parameter register, also r2) is already consumed by the dx
         * computation above, then reuses that same r2 for both the dx
         * and dy comparisons. A pin declared any earlier claims r2 for
         * its whole lexical scope, forcing this compiler to relocate
         * `testX` out of r2 into r1 pre-emptively instead. */
        {
            register s32 limit asm("r2") = 0x3FFF;
            if (dx <= limit) {
                s32 dy = *(s32 *)(self + 4) - testY;
                if (dy < 0) {
                    dy = -dy;
                }
                if (dy <= limit) {
                    if (self[0x4e] != 5) {
                        sub_0800D18C(self);
                    }
                }
            }
        }
    }

    /* Inline-asm-anchored: the ROM computes the `~8` clear-mask at
     * runtime (`movs r0,#9; rsbs r0,r0,#0`, the negative-constant
     * register-pinned mask idiom - matching_decomp_register_pinning)
     * rather than folding it into an 8-bit AND immediate, and keeps
     * the loaded byte in r6, AND-ing into r0 (not r6). A plain
     * `register u8 loaded asm("r6") = self[0xc];` gets optimized away
     * (its single use inlines straight into the AND, dropping the r6
     * pin entirely) - anchored as one literal block instead, since
     * `self` is already known to sit in r4 throughout this function. */
    asm volatile(
        "mov r0, #0x9\n\t"
        "neg r0, r0\n\t"
        "ldrb r6, [r4, #0xc]\n\t"
        "and r0, r0, r6\n\t"
        "strb r0, [r4, #0xc]\n\t"
        :
        :
        : "r0", "r6", "cc", "memory"
    );
    return 0;
}
