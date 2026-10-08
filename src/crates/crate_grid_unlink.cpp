#include "crate_list.hpp"

extern "C" {
#include "match.h"
}

/* CrateList::Unlink (#664, part 7f; include/crate_list.hpp). An
 * old_agbcp object (OLD_AGBCC_OBJS). */

/* Puts `node`'s free-list entry back at the head of the free list. The
 * ROM loads the head into r1 before it loads `node->wrap` into r0; the r1
 * pin reproduces that. Unpinned, the head gets r0 and the entry r1, or
 * (read in the store, `node->wrap->next = freeHead`) the entry is loaded
 * first; a copy of `node`, the loads into locals in either order and an
 * inline setter don't change that. Through a reference or a pointer to
 * `freeHead` (#662 round 2) the registers are the ROM's but `node->wrap`
 * is loaded first again; an inline push taking the head by pointer or
 * reference, or the link first, doesn't help either. */
static inline void FreeNode(CrateList *list, CrateGridNode *node)
{
    MATCH_HOLD_REG(CrateGridLink *, head, r1) = list->freeHead;

    node->wrap->next = head;
    list->freeHead = node->wrap;
}

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
 * it too. */
void CrateList::Unlink(Crate *sprite)
{
    s32 column = ColumnOf(sprite);
    CrateGridNode *found = heads[column];
    CrateGridNode *node;
    CrateGridNode *prev = 0;
    s32 removed = 0;
    s32 i;

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
            FreeNode(this, found);
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
                FreeNode(this, found);
                /* Restarts the search (see above). */
                i = 0x100;
                if (removed > 1)
                    break;
            }
        }
    }
}
