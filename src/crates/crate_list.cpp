#include "crate_list.hpp"

extern "C" {
#include <agb_syscall.h>
#include "memory.h"
}

/* The crate list's slots and grid nodes (#664, part 7f;
 * include/crate_list.hpp). An old_agbcp object (OLD_AGBCC_OBJS). */

/* PartList::CollideWithObject's twin (part_list_collide.cpp), for Collide:
 * when `part` touches the box (ClassifySpriteContact), its HandleEvent
 * with `other`'s kind, and `other` is marked touched. */
void CrateList::CollideWithObject(struct aabb box, MovingSprite *part, MovingSprite *other)
{
    if (part->ClassifyContact(&box)) {
        part->HandleEvent(1, other->kind, 0);
        other->f.b.bit3 = 1;
    }
}

/* Removes `sprite` (Detach, out of line). */
void CrateList::Remove(Crate *sprite)
{
    Detach(sprite);
}

/* Removes the sprite in slot `index`, as Remove does. */
void CrateList::RemoveAt(s32 index)
{
    if (index < capacity) {
        Unlink(slots[index]);
        CpuSet(&slots[index + 1], &slots[index],
               ((count - index) & CPU_SET_COUNT_MASK) | CPU_SET_32BIT);
        slots[count - 1] = 0;
        count--;
    }
}

/* Takes a node off the free list for `sprite` and appends it to column
 * `column`, with `link` its other node (Append, out of line). */
CrateGridNode *CrateList::AddNode(Crate *sprite, s32 column, CrateGridNode *link)
{
    return Append(sprite, column, link);
}

/* Files `sprite` in the column of its x, and an always-active one in
 * column 255 too, the two nodes linked to each other. */
void CrateList::Link(Crate *sprite)
{
    CrateGridNode *node = AddNode(sprite, ColumnOf(sprite), 0);

    if (sprite->IsAlwaysActive())
        node->link = AddNode(sprite, 255, node);
}

/* Appends `sprite` if there's room, filing it in the grid first. */
void CrateList::Add(Crate *sprite)
{
    if (count < capacity) {
        Link(sprite);
        slots[count] = sprite;
        count++;
    }
}

CrateList::~CrateList()
{
    if (links)
        delete[] links;
    if (nodes)
        delete[] nodes;
    if (slots)
        delete[] slots;
    capacity = 0;
}
