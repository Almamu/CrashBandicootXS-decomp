#include "core.h"
#include "box_part.h"
#include "actor_self.h"

ACTOR_CALL_VIA_ALIASES

typedef void (*part_method3_fn)(void *self, s32 a, s32 b, s32 c);

extern s32 sub_8009FF4(struct box_part *part, struct part_aabb *box);

/* `sub_8008AD8`'s sibling: resolves the same collision-hit logic when
 * the "compare viewport" doesn't match the current one (see
 * `sub_8008A40` in actor_part7.c) - `other` here plays the role
 * `gUnknown_030012D8` (the player) plays in `sub_8008AD8`. Tests `part`
 * against the incoming box via `sub_8009FF4`; on a hit, calls `part`'s
 * method-table +0x68 method with `other->kind` as the second argument,
 * then sets `other`'s hit flag (bit 3).
 *
 * The box arrives by value (three words in r1-r3, one on the stack) -
 * the old "leave one scalar in its incoming stack slot" blocker was
 * just that. Kept in its own translation unit since its ROM address,
 * 0x08008D80, isn't adjacent to actor_part7.c's functions
 * (actor_part10.c's sub_8008C80/sub_8008CEC/sub_8008D30 sit between).
 * See docs/matching/issue-9-naked-retry.md. */
void sub_8008D80(struct part_list *list, struct part_aabb box, struct box_part *part, struct box_part *other)
{
    if (sub_8009FF4(part, &box)) {
        struct part_method *m = PART_METHOD(part, 0x68);

        ((part_method3_fn)m->fn)((u8 *)part + m->thisOffset, 1, other->kind, 0);
        other->flags |= 8;
    }
}
asm(".align 2, 0");
