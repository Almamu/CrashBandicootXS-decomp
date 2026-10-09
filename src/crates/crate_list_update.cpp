#include "crate_list.hpp"
#include "bg_layer.hpp"

extern "C" {
#include "crates.h"
#include "memory.h"
#include "math_util.h"
#include "globals.h"
#include "level.h"
}

/* CrateList's constructor and Unlink (#664, part 7f;
 * include/crate_list.hpp). An old_agbcp object (OLD_AGBCC_OBJS). */

/* InitCrateList: an empty list for `n` sprites (play_room.cpp's
 * `InitCrateList(OperatorNew(0x818), 0xC0)`), the slots cleared and every
 * node on the free list. */
CrateList::CrateList(s32 n)
{
    count = 0;
    capacity = n;
    slots = new Crate *[n];
    nodes = new CrateGridNode[capacity];
    links = new CrateGridLink[capacity];
    for (s32 j = 0; j < capacity; j++)
        slots[j] = 0;
    ResetGrid();
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
 * 255, and links the two. crate_break.cpp and crate_switches.cpp call it
 * when a crate starts to move. */
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

/* CrateList::Update (#664, part 7f; include/crate_list.hpp). An
 * old_agbcp object (OLD_AGBCC_OBJS). */

/* `s` again, as the inlined call's own value: each Copy is a register
 * copy of its own, which Destroy's two calls need for the ROM's register
 * pairs (sb/ip in the columns' copy, sl/sb in column 255's); the C had
 * statement expressions for them. Without them, the sprite takes another
 * register and both copies' code is off. */
static inline Crate *Copy(Crate *s)
{
    return s;
}

/* Removes `sprite` (Detach, Remove's body, inlined) and deletes it. */
static inline void Destroy(CrateList *list, Crate *sprite)
{
    list->Detach(Copy(sprite));
    delete Copy(sprite);
}

/* The list's per-frame pass, over the camera's column and the two to its
 * right, then column 255. For each sprite in the columns: one that has
 * become always active without a column-255 node gets one (as LinkActive
 * does); a gone one is removed (Detach, Remove's body) and
 * deleted; any other is updated when it is inside the update region (the
 * screen widened by 100 pixels each side and 60 above and below, in Q8),
 * and marked (`mark`) so that the pass over column 255 skips its second
 * node, clearing the mark instead. Column 255's gone sprites are removed
 * and deleted too, and the others not updated yet are updated. */
void CrateList::Update()
{
    struct aabb region;
    BgLayer *cam;
    s32 lo;
    s32 i;
    s32 next;
    CrateGridNode *node;
    CrateGridNode **last;

    {
        s32 w = 0x1b800;
        s32 h = 0x11800;
        region.w = w;
        region.h = h;
    }
    cam = gLevelLayers->layer0;
    {
        s32 x = INT_TO_Q8(cam->x) - 0x6400;
        s32 y = INT_TO_Q8(cam->y) - 0x3c00;
        region.x = x;
        region.y = y;
    }
    lo = cam->x;
    lo >>= 8;
    LIMIT_MIN(lo, 0);
    i = lo + 2;
    do {
        /* As in Draw: computed in the loop, it is hoisted after `heads`, as
         * in the ROM; it is column 255's head in the append below too. */
        last = &heads[255];
        node = heads[i];
        next = i - 1; // before the walk, as the ROM's `subs`
        while (node != 0) {
            /* Reading `node->data` twice gives the ROM's load into r2 and
             * the copy into r5. */
            s32 active = (node->data->f.flags >> 4) & 1;
            Crate *sprite = node->data;

            if (active && node->link == 0) {
                /* Append's body, through `last`: Append itself recomputes
                 * column 255's address, which takes `last` out of its
                 * register. */
                CrateGridLink *entry = freeHead;
                CrateGridNode *added = entry->node;

                freeHead = entry->next;
                entry->next = 0;
                added->data = sprite;
                added->next = 0;
                added->link = node;
                added->mark = 0;
                added->mark2 = 0;
                if (*last == 0)
                    *last = added;
                if (tails[255] != 0)
                    tails[255]->next = added;
                tails[255] = added;
                node->link = added;
            } else if (sprite->IsGone()) {
                Destroy(this, Copy(sprite));
            } else if ((u8)sprite->IsInsideRect(&region)) {
                node->data->Update();
                node->mark = 1;
            }
            node = node->next;
        }
        i = next;
    } while (i >= lo);
    for (node = *last; node != 0; node = node->next) {
        Crate *sprite = node->data;

        if (sprite->IsGone()) {
            Destroy(this, Copy(sprite));
        } else if (node->link->mark == 0) {
            sprite->Update();
        } else {
            node->link->mark = 0;
        }
    }
}
