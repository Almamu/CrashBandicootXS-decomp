#include "bg_layer.hpp"
#include "crate_list.hpp"

extern "C" {
#include "math_util.h"
#include "globals.h"
#include "level.h"
}

/* CrateList::Draw (#664, part 7f; include/crate_list.hpp). */

/* Draws the listed sprites on screen (the 240x160 GBA screen, in Q8, at
 * `gLevelLayers->layer0`'s scroll position): those in the camera's column
 * and the two to its right, each marked (`mark2`) so that the pass over
 * column 255 skips its second node, clearing the mark instead; then the
 * always-active ones in column 255 not drawn yet. PartList::Draw's
 * counterpart. */
void CrateList::Draw()
{
    struct aabb screen;
    BgLayer *cam = gLevelLayers->layer0;
    s32 lo;
    s32 i;
    CrateGridNode *node;
    CrateGridNode **last;

    {
        s32 cx = INT_TO_Q8(cam->x);
        s32 cy = INT_TO_Q8(cam->y);
        screen.x = cx;
        screen.y = cy;
    }
    {
        s32 w = INT_TO_Q8(240);
        s32 h = INT_TO_Q8(160);
        screen.w = w;
        screen.h = h;
    }
    lo = cam->x;
    lo >>= 8;
    LIMIT_MIN(lo, 0);
    i = lo + 2;
    do {
        /* Column 255's head, computed in the loop: the loop optimizer
         * hoists it after the address of `heads`, as in the ROM, and keeps
         * `heads[i]`'s `add` base first. Computed before the loop, it
         * comes first; through a `heads` pointer local, the `add` is
         * offset first. The C needed an `asm` add and 7 pins for this. */
        last = &heads[255];
        for (node = heads[i]; node != 0; node = node->next) {
            if ((u8)node->data->OverlapsRect(&screen)) {
                node->data->Draw();
                node->mark2 = 1;
            }
        }
        i--;
    } while (i >= lo);
    for (node = *last; node != 0; node = node->next) {
        if (node->link->mark2 == 0) {
            if ((u8)node->data->OverlapsRect(&screen))
                node->data->Draw();
        } else {
            node->link->mark2 = 0;
        }
    }
}
