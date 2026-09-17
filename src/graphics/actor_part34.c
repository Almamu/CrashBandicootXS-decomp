#include "core.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

/* Constant getter - returns `self`'s death flag (`self+0x6c`). */
u8 sub_8033CF0(void *selfArg)
{
    u8 *self = selfArg;

    return self[0x6c];
}

asm(".align 2, 0");
