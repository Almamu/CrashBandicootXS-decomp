#include "crate_list.hpp"

extern "C" {
#include "math_util.h"
#include "globals.h"
#include "level.h"
#include "bg_scroll_layer.h"
}

/* CrateList::Update (#664, part 7f; include/crate_list.hpp). An
 * old_agbcp object (OLD_AGBCC_OBJS). */

/* `s` again, as the inlined call's own value: each Copy is a register
 * copy of its own, which Destroy's two calls need for the ROM's register
 * pairs (sb/ip in the columns' copy, sl/sb in column 255's); the C had
 * statement expressions for them. Without them, the sprite takes another
 * register and both copies' code is off. */
static inline Sprite *Copy(Sprite *s)
{
    return s;
}

/* Removes `sprite` (Detach, Remove's body, inlined) and deletes it. */
static inline void Destroy(CrateList *list, Sprite *sprite)
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
    struct bg_scroll_layer *cam;
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
            Sprite *sprite = node->data;

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
        Sprite *sprite = node->data;

        if (sprite->IsGone()) {
            Destroy(this, Copy(sprite));
        } else if (node->link->mark == 0) {
            sprite->Update();
        } else {
            node->link->mark = 0;
        }
    }
}
