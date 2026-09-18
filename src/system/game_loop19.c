#include "core.h"

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * game_loop18.c - see game_loop17.c's header comment and
 * docs/matching/issue-38-medal-results-tally.md for the full write-up.
 * This single function sits between the sub_8024590..sub_8024708 run
 * (parked/left in asm/code_3_2_17_24590.s) and sub_8024790 (parked/left
 * in asm/code_3_2_17_24790.s). */

extern void *gUnknown_03001314;

/* Trivial setter: `gUnknown_03001314 = value` - see
 * asm/code_3_2_17_24590.s's sub_8024708 for the other (bitfield-level)
 * writer of this same global. */
void sub_8024784(void *value)
{
    gUnknown_03001314 = value;
}
