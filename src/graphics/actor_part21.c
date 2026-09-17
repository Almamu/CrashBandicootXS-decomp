#include "core.h"

/* Same boss-weapon "self" object family as actor_part20.c - see that
 * file's header comment and docs/matching/issue-58-0x08030334-actor.md. */

/* Trivial setter: marks `self+0x68` (a small state/flag byte, meaning
 * not yet understood beyond its offset). */
void sub_8030640(void *selfArg)
{
    u8 *self = selfArg;
    self[0x68] = 1;
}
