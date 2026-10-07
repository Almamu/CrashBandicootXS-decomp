#include "core.h"
#include "box_part.h"
#include "actor_self.h"
#include "objects.h"


/* `CollidePartWithPlayer`'s sibling: resolves the same collision-hit logic when
 * the "compare viewport" doesn't match the current one (see
 * `CollidePartList` in sprite_anim.cpp) - `other` here plays the role
 * `gPlayer` (the player) plays in `CollidePartWithPlayer`. Tests `part`
 * against the incoming box via `ClassifySpriteContact`; on a hit, calls `part`'s
 * method-table +0x68 method with `other->kind` as the second argument,
 * then sets `other`'s hit flag (bit 3).
 *
 * The box arrives by value (three words in r1-r3, one on the stack) -
 * the old "leave one scalar in its incoming stack slot" blocker was
 * just that. Kept in its own translation unit since its ROM address,
 * 0x08008D80, isn't adjacent to sprite_anim.cpp's functions
 * (part_list_cull.c's CullPartList/ClearPartList/CollidePartsOfClass sit between).
 * See docs/matching/archive/issue-9-naked-retry.md. */
void CollidePartWithObject(struct part_list *list, struct aabb box, struct box_part *part,
                           struct box_part *other)
{
    if (ClassifySpriteContact(part, &box)) {
        struct part_method *m = PART_METHOD(part, 0x68);

        ((part_method3_fn)m->fn)((u8 *)part + m->thisOffset, 1, other->kind, 0);
        other->flags |= PART_FLAG_TOUCHED;
    }
}
