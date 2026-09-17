#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

/* Trivial constant predicate - always "true". */
s32 sub_802FA34(void)
{
    return 1;
}

asm(".align 2, 0");
