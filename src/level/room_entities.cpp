#include "crate_list.hpp"
#include "crate.hpp"
#include "spawners.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "match.h"
#include <agb_syscall.h>
#include "crates.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* GitHub issue #34/#40/#41, `UpdateGameFrame`-`MainLoop` cluster: the
 * second of the two raw functions `docs/matching/issue-34-game-loop-
 * 8022d50-80255d4.md` left for a follow-up pass (the first,
 * `StartTimeTrial`, is `time_trial.cpp`, now plain C).
 *
 * `self` is `*gEntityFlags` (the same collision-bitmap base
 * `SetEntityIdGone`/`IsEntityIdGone`/`IsEntityIdActivated`, entity_flags.cpp, and
 * `MarkEntityIdActivated`, entity_flags.cpp, already operate on).
 *
 * First half (fully understood, matches the ROM's own idiom one for
 * one): if `list` differs from `self`'s cached copy at `self+0`,
 * `self+8`/`self+0x208` (the first two of the three overlapping
 * collision-bitmap arrays `entity_flags.cpp`'s header comment documents)
 * are DMA3-zero-filled 64 bytes each (`DmaFill32(3, 0, dest, 64)`,
 * expanding to the exact same `REG_DMA3`-field-by-field store sequence
 * seen here); either way `self+8`->`self+0x108` and
 * `self+0x208`->`self+0x308` get unconditionally `CpuSet`-copied via
 * `CpuSet(src, dst, 0x04000040)` (the same idiom `SetCheckpointAtPlayer`,
 * bonus_round.c, already documents in the opposite direction), and
 * `self+4` is set from `posArg >> 8` (a Q8-to-int truncation). `list`
 * is then walked as a `{count:u16 @2, groups:ptr @4}` header over
 * `{count:u16 @2, items:ptr @4}` 8-byte group records, each holding
 * `{tableIdx:u16, p1:u16, p2:u16, p3:u16}` 8-byte item records; for
 * each item not already flagged in the `self+8` bit-grid
 * (`IsEntityIdGone`), `SpawnEntity` (the table-indexed interworking-
 * trampoline dispatcher, entity_spawner.cpp) fires with a running,
 * never-reset-per-group counter as its own `self` argument, indexing
 * `gEntitySpawner`'s table.
 *
 * Second half (skipped when `links` is NULL): each crate in
 * `gCrateList` whose id is a link's `from` is chained
 * (`Crate::SetAbove`/`SetBelow`) to the crate with the link's `to` id,
 * following further links while `to` isn't spawned. Then each link whose
 * `from` crate doesn't exist resolves its `to` chain to a spawned crate
 * and moves that crate's stack (`GetAbove`/`SetEntityPos`) up by the
 * height of its box (`GetBounds()->h`, a virtual call).
 *
 * C++ since the #664 cleanup (include/crate.hpp's Crate), built with
 * old_agbcp (room_entities.o is on OLD_AGBCC_OBJS; this file holds only
 * this function; agbcp loads the group count into another register). Earlier passes had it NAKED (153, then 219
 * halfwords off); the third pass (docs/matching/archive/big-naked-retry-3.md)
 * closed it:
 * - old_agbcc's expand_end_loop rotation does not stop at a nested
 *   loop, so a `break` inside a search loop is taken as the loop's exit
 *   test and the search body is duplicated ahead of the loop. The ROM
 *   has none of that: the searches leave with `goto` to a label after
 *   the loop instead.
 * - The link-chasing do-while keeps its search first once the scan is
 *   written as `if (!done) { ... }` instead of `if (done) break;`.
 * - The third pass's `for (;;)` has one `break` (when no link is left),
 *   and the "got an actor" exits jump to `move:`. The dead
 *   `mov r0, #0; cmp r0, #0` tests are `got` tests after gcse has
 *   proved `got` is 0 on every path that reaches them; `got = 0` has to
 *   sit before the loop for that. `if (got && actor != 0)` lets the
 *   no-link exit jump straight past the move.
 * - No `continue` before `move:`: loop.c moves a block that jumps out
 *   of a loop to just before its target when it finds a barrier there,
 *   which put `actor = a` next to `move:`.
 * - Separate `k`/`k2`/`k3` search counters, `i` declared at function
 *   scope (stack-slot order), `n > 0` guard + do-while for the link
 *   scan, `struct lk_point *pp = &p` for the move call, and a
 *   `u32 zero` for the DMA fills.
 * - The first search's id is pinned to r1 (see the comment there); the
 *   C++ still needs it.
 */
struct lk_point {
    s32 x;
    s32 y;
};

static inline Crate *Slot(s32 i)
{
    return gCrateList->slots[i];
}

void SpawnRoomEntities(struct entity_flags *self, const struct level_entity_list *list,
                       const struct level_link_list *links, s32 posArg, s32 unused)
{
    s32 i;
    s32 counter;
    s32 g;
    s32 n;
    const struct level_link *lk;
    u32 zero = 0;

    if (list != self->list) {
        self->list = list;
        DmaFill32(3, zero, self->bits0, 64);
        DmaFill32(3, zero, self->bits1, 64);
    }
    CpuSet(self->bits0, self->bits0Copy, CPU_SET_32BIT | 0x40);
    CpuSet(self->bits1, self->bits1Copy, CPU_SET_32BIT | 0x40);
    self->pos = Q8_TO_INT(posArg);

    counter = 0;
    for (g = self->list->groupCount - 1; g >= 0; g--) {
        const struct level_entity_group *group = &self->list->groups[g];
        s32 k;

        for (k = 0; k < group->count; k++) {
            if (!(u8)IsEntityIdGone(self, counter))
                gEntitySpawner->Spawn(counter, &group->entities[k]);
            counter++;
        }
    }

    if (links == 0)
        return;
    n = links->count;
    lk = links->links;

    {
        for (i = gCrateList->count - 1; i >= 0; i--) {
            Crate *actor = Slot(i);
            u16 id = actor->id;
            s32 j;

            for (j = 0; j < n; j++) {
                if (id == lk[j].from) {
                    s32 done = 0;
                    s32 to = lk[j].to;

                    do {
                        s32 k;
                        s32 missing;
                        s32 m;

                        for (k = gCrateList->count - 1; k >= 0; k--) {
                            Crate *other = Slot(k);

                            if (to == other->id) {
                                actor->SetAbove(other);
                                other->SetBelow(actor);
                                done = 1;
                                break;
                            }
                        }
                        if (!done) {
                            missing = 1;
                            for (m = 0; m < n; m++) {
                                if (to == lk[m].from) {
                                    to = lk[m].to;
                                    missing = 0;
                                    break;
                                }
                            }
                            if (missing)
                                done = 1;
                        }
                    } while (!done);
                    break;
                }
            }
        }
    }

    {
        s32 j;

        for (j = 0; j < n; j++) {
            u16 from = lk[j].from;
            s32 found = 0;
            s32 k;
            s32 k2;
            s32 k3;
            Crate *actor;
            s32 to;
            s32 missing;
            s32 next;
            s32 got;
            s32 m;

            for (k = 0; k < gCrateList->count; k++) {
                if ((Slot(k))->id == from) {
                    found = 1;
                    goto chk;
                }
            }
        chk:
            if (found)
                continue;

            to = (u16)lk[j].to;
            actor = 0;
            got = 0;
            for (;;) {
                missing = 1;
                next = 0;
                m = 0;
                if (n > 0)
                    do {
                        if (to == lk[m].from) {
                            missing = 0;
                            next = (u16)lk[m].to;
                            for (k2 = 0; k2 < gCrateList->count; k2++) {
                                /* Pinned: the ROM keeps the item pointer in
                                 * r0 and the id in r1. As a local temporary
                                 * the id is allocated first and takes r0. */
                                MATCH_HOLD_REG(u16, aid, r1);

                                actor = Slot(k2);
                                aid = actor->id;
                                if (aid == to) {
                                    got = 1;
                                    goto t1;
                                }
                            }
                            goto t1;
                        }
                    } while (++m < n);
            t1:
                if (got)
                    goto move;
                if (missing) {
                    for (k3 = 0; k3 < gCrateList->count; k3++) {
                        Crate *a = Slot(k3);

                        if (a->id == to) {
                            actor = a;
                            got = 1;
                            goto t2;
                        }
                    }
                }
            t2:
                if (got)
                    goto move;
                if (missing)
                    break;
                to = next;
            }
        move:
            if (got && actor != 0) {
                s32 lift = INT_TO_Q8(actor->GetBounds()->h + 1);
                struct lk_point p;
                struct lk_point *pp = &p;

                do {
                    p.x = actor->x;
                    pp->y = actor->y + lift;
                    SetEntityPos(actor, p.x, pp->y);
                    actor = actor->GetAbove();
                } while (actor != 0);
            }
        }
    }
}
