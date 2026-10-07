#include "crate.hpp"
#include "crate_list.hpp"

extern "C" {
#include "objects.h"
#include "level.h"
#include "globals.h"
#include "player.h"
}

/* The player's pass over the crate grid (#664, part 7f;
 * include/crate_list.hpp). */

/* For the crates in the three grid buckets from the camera's column on
 * (as UpdateCrateList's window): a player in control mode 3 breaks the
 * ones it touches (BreakCrateTouchedByPlayer); otherwise each collides
 * with the player (CollideCrateWithPlayer), with the player's action (its
 * controller's state; 0 in control mode 1, or 0xD for a kind 0x13
 * player). Old_agbcc C (see docs/matching/archive/issue-9-naked-retry.md):
 * the camera x is read before the `>> 8`. */
void CrateList::CollidePlayer(s32 unused)
{
    s32 lo = gLevelLayers->layer0->x;
    s32 i;
    struct player *p;
    u8 mode;

    lo >>= 8;
    LIMIT_MIN(lo, 0);
    i = lo + 2;
    p = gPlayer;
    mode = p->ctrlMode;
    if (mode == 3) {
        do {
            CrateGridNode *node;

            for (node = heads[i]; node != 0; node = node->next)
                node->data->BreakIfTouchedByPlayer();
            i--;
        } while (i >= lo);
    } else {
        s32 action = ((struct ctrl *)p->ctrl)->state;
        s32 px = p->x;
        s32 py = p->y;

        if (mode == 1) {
            action = 0;
            if (p->kind == 0x13)
                action = 0xd;
        }
        do {
            CrateGridNode *node;

            for (node = heads[i]; node != 0; node = node->next)
                node->data->CollideWithPlayer(action, px, py);
            i--;
        } while (i >= lo);
    }
}
