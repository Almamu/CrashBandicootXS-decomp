#include "core.h"

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * game_loop18.c - see game_loop17.c's header comment and
 * docs/matching/issue-38-medal-results-tally.md for the full write-up.
 * This single function sits between the BeginSlide..ShowSlidePicture run
 * (game_loop37.c - BeginSlide NAKED-parked, the rest matched - see
 * docs/matching/issue-38-sound-channel-family.md) and EndSlide
 * (matched, game_loop38.c). */

extern void *gUnknown_03001314;

/* Trivial setter: `gUnknown_03001314 = value` - see game_loop37.c's
 * ShowSlidePicture for the other (bitfield-level) writer of this same
 * global. */
void sub_8024784(void *value)
{
    gUnknown_03001314 = value;
}
