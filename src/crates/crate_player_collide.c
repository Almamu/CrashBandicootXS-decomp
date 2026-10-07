#include "core.h"
#include "actor.h"
#include "crates.h"
#include "level.h"
#include "globals.h"
#include "player.h"

/* Another per-frame spatial-hash-grid pass over `manager`, scoped to
 * the same 3-bucket window `[baseIdx, baseIdx+2]` (`baseIdx` computed
 * the same way as `UpdateCrateList`'s: `max(gLevelLayers`'s
 * sub-object's own `x >> 8`, `0)`), reading the player
 * (`gPlayer`) rather than writing to the grid.
 *
 * If the player's `+0x88` byte is `3`: for every windowed node, calls
 * `BreakCrateTouchedByPlayer(part)`.
 *
 * Otherwise: computes a dispatch value from the player (`*(void **)
 * (player+0x44) + 8`'s pointee by default; `0` if the player's `+0x88`
 * byte is `1`, further overridden to `0xd` if that byte is also `1`
 * *and* the player's `+0xa` byte is `0x13`), then for every windowed
 * node calls `CollideCrateWithPlayer(part, dispatchValue, player->x, player->y)`.
 *
 * Matches under old_agbcc (see docs/matching/archive/issue-9-naked-retry.md):
 * the "cross-branch register-role gap" this was parked for was the
 * newer compiler; the only shaping detail is reading the camera x
 * before the `>> 8`. */
void CollidePlayerWithCrates(struct pool_manager *m, s32 unused)
{
    s32 lo = gLevelLayers->layer0->x;
    s32 i;
    struct player *p;
    u8 state;

    lo >>= 8;
    if (lo < 0)
        lo = 0;
    i = lo + 2;
    p = gPlayer;
    state = p->ctrlMode;
    if (state == 3) {
        do {
            struct pool_node *node;
            for (node = m->gridHead[i]; node != NULL; node = node->next)
                BreakCrateTouchedByPlayer(node->data);
            i--;
        } while (i >= lo);
    } else {
        s32 mode = ((struct ctrl *)p->ctrl)->state;
        s32 px = p->x;
        s32 py = p->y;

        if (state == 1) {
            mode = 0;
            if (p->kind == 0x13)
                mode = 0xd;
        }
        do {
            struct pool_node *node;
            for (node = m->gridHead[i]; node != NULL; node = node->next)
                CollideCrateWithPlayer((struct crate *)node->data, mode, px, py);
            i--;
        } while (i >= lo);
    }
}
