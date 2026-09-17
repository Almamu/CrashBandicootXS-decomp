#include "core.h"

/* Same boss-weapon "self" object family as actor_part20.c - see that
 * file's header comment and docs/matching/issue-58-0x08030334-actor.md. */

/* Trivial getter counterpart to `sub_8030640` (actor_part21.c): reads
 * `self+0x68`. */
u8 sub_80306A4(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x68];
}

asm(".align 2, 0");
