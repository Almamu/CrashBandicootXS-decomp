#include "part_list.hpp"

/* PartList::CollideWithObject (#664, part 7c; include/part_list.hpp). An
 * old_agbcp object (OLD_AGBCC_OBJS). It comes right after
 * part_list_cull.cpp's functions in the ROM (0x08008D80). */

/* CollideWithPlayer's counterpart for a list collided with another
 * object (CollidePartList): when `part` touches the box
 * (ClassifySpriteContact), its HandleEvent with `other`'s kind, and
 * `other` is marked touched. */
void PartList::CollideWithObject(struct aabb box, MovingSprite *part, MovingSprite *other)
{
    if (part->ClassifyContact(&box)) {
        part->HandleEvent(1, other->kind, 0);
        other->f.b.bit3 = 1;
    }
}
