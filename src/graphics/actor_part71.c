#include "core.h"

/* Same "self" object family as actor_part63.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. */

/* Constant getter - returns `self`'s one-shot flag (`self+0x58`). */
u8 sub_803436C(void *selfArg)
{
    u8 *self = selfArg;

    return self[0x58];
}

asm(".align 2, 0");
