#include "crate_list.hpp"

/* CrateList::Reset (#664, part 7f; include/crate_list.hpp). In its own
 * file for ROM order: 0x08009914, between CollidePlayer
 * (crate_list_collide.cpp) and crate_list.cpp's functions. */

/* Deletes every listed sprite and empties the list, the grid and the
 * free list (ResetGrid, the constructor's tail). */
void CrateList::Reset()
{
    s32 i;

    for (i = 0; i < count; i++) {
        delete slots[i];
        slots[i] = 0;
    }
    count = 0;
    ResetGrid();
}
