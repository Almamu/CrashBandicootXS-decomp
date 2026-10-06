#include "core.h"
#include "actor_self.h"
#include <agb_syscall.h>
#include "crates.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"

/* GitHub issue #34/#40/#41, `UpdateGameFrame`-`MainLoop` cluster: the
 * second of the two raw functions `docs/matching/issue-34-game-loop-
 * 8022d50-80255d4.md` left for a follow-up pass (the first,
 * `StartTimeTrial`, is `time_trial.c`, now plain C).
 *
 * `self` is `*gEntityFlags` (the same collision-bitmap base
 * `sub_8025944`/`sub_8025968`/`sub_802599C`, entity_flags.c, and
 * `sub_8025A0C`, entity_flags.c, already operate on).
 *
 * First half (fully understood, matches the ROM's own idiom one for
 * one): if `list` differs from `self`'s cached copy at `self+0`,
 * `self+8`/`self+0x208` (the first two of the three overlapping
 * collision-bitmap arrays `entity_flags.c`'s header comment documents)
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
 * (`sub_8025968`), `SpawnEntity` (the table-indexed interworking-
 * trampoline dispatcher, entity_spawner.c) fires with a running,
 * never-reset-per-group counter as its own `self` argument, indexing
 * `gEntitySpawner`'s table.
 *
 * Second half (skipped when `links` is NULL): each actor in
 * `gCrateList` whose id is a link's `from` is chained
 * (`SetCrateAbove`/`SetCrateBelow`) to the actor with the link's `to` id,
 * following further links while `to` isn't spawned. Then each link whose
 * `from` actor doesn't exist resolves its `to` chain to a spawned actor
 * and moves that actor's neighbour chain (`GetCrateAbove`/`SetEntityPos`)
 * up by its `+0x10` method's height.
 *
 * Built with old_agbcc (room_entities.o is on OLD_AGBCC_OBJS; this file
 * holds only this function). Earlier passes had it NAKED (153, then 219
 * halfwords off); the third pass (docs/matching/big-naked-retry-3.md)
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
 *   sit before the loop for that. `if (got && actor != NULL)` lets the
 *   no-link exit jump straight past the move.
 * - No `continue` before `move:`: loop.c moves a block that jumps out
 *   of a loop to just before its target when it finds a barrier there,
 *   which put `actor = a` next to `move:`.
 * - Separate `k`/`k2`/`k3` search counters, `i` declared at function
 *   scope (stack-slot order), `n > 0` guard + do-while for the link
 *   scan, `struct lk_point *pp = &p` for the move call, and a
 *   `u32 zero` for the DMA fills.
 * - The first search's id is pinned to r1 (see the comment there).
 */
struct lk_point
{
    s32 x;
    s32 y;
};

struct lk_vtable
{
    u8 unk_00[0x10];
    struct actor_method height;        /* +0x10 */
};

struct lk_actor
{
    struct lk_point pos;            /* +0x00 */
    u16 id;                         /* +0x08 */
    u8 unk_0A[0xE];
    struct lk_vtable *vtable;       /* +0x18 */
};

extern u8 *_call_via_r1(void *self, void *fn);

void SpawnRoomEntities(struct entity_flags *self, const struct level_entity_list *list, const struct level_link_list *links, s32 posArg, s32 unused)
{
    s32 i;
    s32 counter;
    s32 g;
    s32 n;
    const struct level_link *lk;
    u32 zero = 0;

    if (list != self->list)
    {
        self->list = list;
        DmaFill32(3, zero, self->bits0, 64);
        DmaFill32(3, zero, self->bits1, 64);
    }
    CpuSet(self->bits0, self->bits0Copy, CPU_SET_32BIT | 0x40);
    CpuSet(self->bits1, self->bits1Copy, CPU_SET_32BIT | 0x40);
    self->pos = posArg >> 8;

    counter = 0;
    for (g = self->list->groupCount - 1; g >= 0; g--)
    {
        const struct level_entity_group *group = &self->list->groups[g];
        s32 k;

        for (k = 0; k < group->count; k++)
        {
            if (!(u8)sub_8025968(self, counter))
                SpawnEntity(gEntitySpawner, counter, (u16 *)&group->entities[k]);
            counter++;
        }
    }

    if (links == NULL)
        return;
    n = links->count;
    lk = links->links;

    {
        for (i = gCrateList->activeCount - 1; i >= 0; i--)
        {
            struct lk_actor *actor = (struct lk_actor *)gCrateList->slotArray[i];
            u16 id = actor->id;
            s32 j;

            for (j = 0; j < n; j++)
            {
                if (id == lk[j].from)
                {
                    s32 done = 0;
                    s32 to = lk[j].to;

                    do
                    {
                        s32 k;
                        s32 missing;
                        s32 m;

                        for (k = gCrateList->activeCount - 1; k >= 0; k--)
                        {
                            struct lk_actor *other = (struct lk_actor *)gCrateList->slotArray[k];

                            if (to == other->id)
                            {
                                SetCrateAbove((struct crate *)actor, (struct crate *)other);
                                SetCrateBelow((struct crate *)other, (struct crate *)actor);
                                done = 1;
                                break;
                            }
                        }
                        if (!done)
                        {
                            missing = 1;
                            for (m = 0; m < n; m++)
                            {
                                if (to == lk[m].from)
                                {
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

        for (j = 0; j < n; j++)
        {
            u16 from = lk[j].from;
            s32 found = 0;
            s32 k;
            s32 k2;
            s32 k3;
            struct lk_actor *actor;
            s32 to;
            s32 missing;
            s32 next;
            s32 got;
            s32 m;

            for (k = 0; k < gCrateList->activeCount; k++)
            {
                if (((struct lk_actor *)gCrateList->slotArray[k])->id == from)
                {
                    found = 1;
                    goto chk;
                }
            }
        chk:
            if (found)
                continue;

            to = (u16)lk[j].to;
            actor = NULL;
            got = 0;
            for (;;)
            {
                missing = 1;
                next = 0;
                m = 0;
                if (n > 0)
                do
                {
                    if (to == lk[m].from)
                    {
                        missing = 0;
                        next = (u16)lk[m].to;
                        for (k2 = 0; k2 < gCrateList->activeCount; k2++)
                        {
                            /* Pinned: the ROM keeps the item pointer in
                             * r0 and the id in r1. As a local temporary
                             * the id is allocated first and takes r0. */
                            register u16 aid asm("r1");

                            actor = (struct lk_actor *)gCrateList->slotArray[k2];
                            aid = actor->id;
                            if (aid == to)
                            {
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
                if (missing)
                {
                    for (k3 = 0; k3 < gCrateList->activeCount; k3++)
                    {
                        struct lk_actor *a = (struct lk_actor *)gCrateList->slotArray[k3];

                        if (a->id == to)
                        {
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
            if (got && actor != NULL)
            {
                struct actor_method *hm = &actor->vtable->height;
                s32 lift = (_call_via_r1((u8 *)actor + hm->thisOffset, hm->fn)[5] + 1) << 8;
                struct lk_point p;
                struct lk_point *pp = &p;

                do
                {
                    p.x = actor->pos.x;
                    pp->y = actor->pos.y + lift;
                    SetEntityPos((struct actor *)actor, p.x, pp->y);
                    actor = (struct lk_actor *)GetCrateAbove((struct crate *)actor);
                } while (actor != NULL);
            }
        }
    }
}
