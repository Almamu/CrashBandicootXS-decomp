#include "core.h"

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * game_loop19.c - see game_loop17.c's header comment and
 * docs/matching/issue-38-medal-results-tally.md for the full write-up.
 * This is the tail of the chunk, right after sub_8024790 (parked/left
 * in asm/code_3_2_17_24790.s). */

extern void sub_8026ED0(void *self);

/* Same wrapper shape as sub_802425C (game_loop17.c): tears `self` down
 * via sub_8026ED0 if bit 0 of `flags` is set. */
void sub_80247EC(void *self, s32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Trivial constructor: zeroes `self->0`/`self->4`, sets `self->0xc` to
 * 1 (the sub_8024708 VRAM-bank toggle's initial state, see
 * asm/code_3_2_17_24590.s). */
void sub_8024804(void *self)
{
    u8 *s = (u8 *)self;

    *(s32 *)s = 0;
    *(s32 *)(s + 4) = 0;
    *(s32 *)(s + 0xc) = 1;
}
