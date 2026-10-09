#include "crate_list.hpp"

/* CrateList::Unlink (#664, part 7f; include/crate_list.hpp). An
 * old_agbcp object (OLD_AGBCC_OBJS). */

/* Takes `sprite`'s nodes out of the grid (Remove and RemoveAt call it
 * before they compact the slots), back onto the free list. First its
 * column's list, up to the first node holding it. Unless that found one
 * and the sprite has a spawn id and isn't always active (so has no
 * column-255 node), every column from 255 down is searched too, each
 * node holding it unlinked, until two have been.
 *
 * The oddity: after each removal in the second search the column index
 * is set to 0x100, so the loop's `i--` restarts the search at column 255,
 * and a later match in the same list is unlinked against `heads[0x100]`,
 * which is `tails[0]`. That is what the original source did, so this does
 * it too.
 *
 * Each unlinked node's free-list entry goes back at the head of the free
 * list through `head`, one function-scope temporary for both searches.
 * That makes `head` a global-alloc register (it is used in two blocks),
 * which gets r1 next to the entry's r0, as in the ROM. With a temporary
 * of its own in each block (an inline helper's local, #662 round 3),
 * each block has three local quantities, and local-alloc's 3-quantity
 * sort compares quantity numbers instead of sorted positions, so the
 * head (the first born) is allocated first and takes r0. */
void CrateList::Unlink(Crate *sprite)
{
    s32 column = ColumnOf(sprite);
    CrateGridNode *found = heads[column];
    CrateGridNode *node;
    CrateGridNode *prev = 0;
    s32 removed = 0;
    s32 i;
    CrateGridLink *head;

    for (; found != 0; prev = found, found = found->next) {
        if (found->data == sprite) {
            found->data = 0;
            removed++;
            if (found == heads[column]) {
                CrateGridNode *next = found->next;

                if (next != 0) {
                    heads[column] = next;
                } else {
                    heads[column] = next;
                    tails[column] = next;
                }
            } else if (prev != 0) {
                if (found == tails[column])
                    tails[column] = prev;
                prev->next = found->next;
            }
            head = freeHead;
            found->wrap->next = head;
            freeHead = found->wrap;
            break;
        }
    }

    {
        u16 id = sprite->id;

        if (found != 0 && id != ENTITY_ID_NONE) {
            s32 active = (sprite->f.flags >> 4) & 1;

            if (!active)
                return;
        }
    }

    for (i = 0xFF; i >= 0; i--) {
        prev = 0;
        for (node = heads[i]; node != 0; prev = node, node = node->next) {
            if (node->data == sprite) {
                found = node;
                node->data = 0;
                removed++;
                if (node == heads[i]) {
                    CrateGridNode *next = node->next;

                    if (next != 0) {
                        heads[i] = next;
                    } else {
                        heads[i] = next;
                        tails[i] = next;
                    }
                } else if (prev != 0) {
                    if (node == tails[i])
                        tails[i] = prev;
                    prev->next = node->next;
                }
                head = freeHead;
                found->wrap->next = head;
                freeHead = found->wrap;
                /* Restarts the search (see above). */
                i = 0x100;
                if (removed > 1)
                    break;
            }
        }
    }
}
