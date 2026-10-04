#include "core.h"

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem (see game_loop6.c's header comment and
 * docs/matching/issue-12-physics-collision.md). Compiled with
 * old_agbcc (see the Makefile's OLD_AGBCC_OBJS). */

extern void *GetCrateAbove(void *obj);
extern void *GetCrateBelow(void *obj);

/* Walks `obj`'s doubly-linked neighbor list both ways (`GetCrateAbove`
 * = next, `GetCrateBelow` = prev), clearing each visited neighbor's
 * `+0x58` byte whenever its own `+0x4d & 0x7f` state byte is 0.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): its scheduler is what
 * puts the `0x7f` mask immediate before the `ldrb`, the gap that kept
 * this NAKED before. The goto-into-loop shape reproduces the ROM's
 * "jump straight to the call with r0 = arg" loop entry. */
void sub_800E494(void *obj)
{
    u8 *cur;
    void *arg;

    arg = obj;
    goto next;
    do {
        if ((cur[0x4d] & 0x7f) == 0)
            cur[0x58] = 0;
        arg = cur;
    next:
        cur = GetCrateAbove(arg);
    } while (cur != NULL);
    arg = obj;
    goto prev;
    do {
        if ((cur[0x4d] & 0x7f) == 0)
            cur[0x58] = 0;
        arg = cur;
    prev:
        cur = GetCrateBelow(arg);
    } while (cur != NULL);
}

/* Same bidirectional-neighbor walk as `sub_800E494` above, but sets
 * `+0x58` to 1 and, for the forward (`GetCrateAbove`) direction only,
 * also debits `ctx+4` and credits `ctx+0xc` by `ctx+0xc`'s *original*
 * value (`step`, cached once before the loops); the reverse direction
 * only credits `ctx+0xc`. The per-loop `one` local is what makes gcc
 * hoist the constant into a callee-saved register (r7, then r6) like
 * the ROM; a literal `1` isn't hoisted and the loop-entry call gets
 * cross-jumped away. Built with old_agbcc. */
void sub_800E4E4(void *obj, void *ctxArg)
{
    s32 *ctx = ctxArg;
    s32 step = ctx[3];
    u8 *cur;

    cur = GetCrateAbove(obj);
    if (cur != NULL) {
        u8 one = 1;
        do {
            if ((cur[0x4d] & 0x7f) == 0) {
                cur[0x58] = one;
                ctx[1] -= step;
                ctx[3] += step;
            }
            cur = GetCrateAbove(cur);
        } while (cur != NULL);
    }
    cur = GetCrateBelow(obj);
    if (cur != NULL) {
        u8 one = 1;
        do {
            if ((cur[0x4d] & 0x7f) == 0) {
                cur[0x58] = one;
                ctx[3] += step;
            }
            cur = GetCrateBelow(cur);
        } while (cur != NULL);
    }
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
