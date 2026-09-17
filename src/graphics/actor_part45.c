#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

extern u8 gUnknown_03001506;

/* Trivial byte getter for the singleton's own flag, `sub_802F570`/
 * `sub_802F69C`'s read counterpart. */
u8 sub_802F7A4(void)
{
    return gUnknown_03001506;
}

asm(".align 2, 0");
