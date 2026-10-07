#include "crate_list.hpp"

/* CrateList::LinkActive (#664, part 7f; include/crate_list.hpp). An
 * old_agbcp object (OLD_AGBCC_OBJS; was agbcc): the flag test is
 * old_agbcp's `ldrb r1; lsrs r0, r1, #4`. The C reproduced it under
 * agbcc with 9 pins and a `MATCH_KEEP_VOLATILE`; with Append inlined,
 * nothing is left of them. */

/* Files `sprite` in column 255 when it has become always active after it
 * was listed (Link files an always-active sprite there from the start):
 * finds its node in the columns from 254 down, and unless the sprite
 * isn't always active or the node already has a column-255 node, takes a
 * node off the free list for it (AddNode's body), appends it to column
 * 255, and links the two. crate_break.cpp calls it when a crate starts to
 * move. */
void CrateList::LinkActive(Crate *sprite)
{
    s32 column;

    for (column = 0xFE; column >= 0; column--) {
        CrateGridNode *node = heads[column];

        if (node == 0)
            continue;
        do {
            Crate *data = node->data;

            if (data == sprite) {
                if (data->IsAlwaysActive() && node->link == 0)
                    node->link = Append(data, 255, node);
                return;
            }
            node = node->next;
        } while (node != 0);
    }
}
