#include "part_list.hpp"
#include "crate_list.hpp"

extern "C" {
#include <agb_syscall.h>
#include "crates.h"
#include "memory.h"
}

/* The part list's own methods (#664, part 7c; include/part_list.hpp).
 * The crate list's constructor, which the ROM puts right after them,
 * starts src/crates/crate_list_unlink.cpp. */

/* Draws every part on screen. */
void PartList::Draw()
{
    s32 i;

    for (i = 0; i < visibleCount; i++)
        visible[i]->Draw();
}

/* Removes `part` from `items`: the entries after it move down one place
 * (CpuSet), and the last slot is cleared. */
void PartList::Remove(Sprite *part)
{
    s32 i = 0;
    s32 n = capacity;

    if (i >= n)
        return;
    while (items[i] != part)
        if (++i >= n)
            return;
    if (i < capacity) {
        CpuSet(&items[i + 1], &items[i], ((count - i) & CPU_SET_COUNT_MASK) | CPU_SET_32BIT);
        count--;
        items[count] = 0;
    }
}

/* Removes the entry at `index`, as Remove does. */
void PartList::RemoveAt(s32 index)
{
    if (index < capacity) {
        CpuSet(&items[index + 1], &items[index],
               ((count - index) & CPU_SET_COUNT_MASK) | CPU_SET_32BIT);
        count--;
        items[count] = 0;
    }
}

/* Appends `part` if there's room. */
void PartList::Add(Sprite *part)
{
    if (count < capacity)
        items[count++] = part;
}

PartList::~PartList()
{
    if (visible)
        delete[] visible;
    if (items)
        delete[] items;
}

PartList::PartList(s32 n)
{
    s32 i;

    count = 0;
    visibleCount = 0;
    capacity = n;
    items = new Sprite *[n];
    visible = new Sprite *[n];
    for (i = 0; i < capacity; i++)
        items[i] = 0;
}
