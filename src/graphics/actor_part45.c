#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

extern u8 gJetpackPlayerInactive;

/* Trivial byte getter for the singleton's own flag, `JetpackPlayerStateResume`/
 * `JetpackPlayerStateEnter`'s read counterpart. */
u8 IsJetpackPlayerInactive(void)
{
    return gJetpackPlayerInactive;
}

asm(".align 2, 0");
