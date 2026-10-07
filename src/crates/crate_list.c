#include "core.h"
#include "match.h"
#include "actor.h"
#include "actor_self.h"
#include "box_part.h"
#include <agb_syscall.h>
#include "crates.h"
#include "objects.h"
#include "memory.h"

typedef void (*part_method3_fn)(void *self, s32 a, s32 b, s32 c);

/* `CollidePartWithObject`'s twin (part_collide.c): the same collision-hit
 * resolver, called from elsewhere in this AI/collision cluster (`list`
 * is never read). Tests `part` against the incoming box via
 * `ClassifySpriteContact`; on a hit, calls `part`'s method-table +0x68 method
 * with `other->kind` as the second argument, then sets `other`'s hit
 * flag (bit 3).
 *
 * The box arrives by value (three words in r1-r3, one on the stack) -
 * the old "leave one scalar in its incoming stack slot" blocker was
 * just that. See docs/matching/archive/issue-9-naked-retry.md. */
void CollideCrateGridPartWithObject(struct part_list *list, struct aabb box, struct box_part *part,
                                    struct box_part *other)
{
    if (ClassifySpriteContact(part, &box)) {
        struct part_method *m = PART_METHOD(part, 0x68);

        ((part_method3_fn)m->fn)((u8 *)part + m->thisOffset, 1, other->kind, 0);
        other->flags |= 8;
    }
}

/* Searches `manager->slotArray` (bounded by `capacity`, for the
 * search) for `target`; if found, removes it from the collision grid
 * and returns its pool node to the free list via `UnlinkCrateFromGrid`, then
 * compacts the array (bounded this time by `activeCount`) via the
 * same CpuSet-based shift used throughout this cluster, decrementing
 * `activeCount`. */
void RemoveCrateFromList(struct pool_manager *manager, struct box_part *target)
{
    s32 i = 0;
    s32 searchCount = manager->capacity;
    void **base;

    if (i >= searchCount) {
        goto done;
    }
    {
        void **p0 = (void **)manager->slotArray;
        void *val = *p0;
        base = p0;
        if (val != target) {
            void **p = base;
            do {
                p++;
                i++;
                if (i >= searchCount) {
                    goto done;
                }
            } while (*p != target);
        }
    }

    if (i < manager->capacity) {
        s32 off = i * 4;
        void *item = base[i];

        UnlinkCrateFromGrid(manager, item);

        {
            s32 srcOff = off + 4;
            void **base2 = (void **)manager->slotArray;
            void *src = (u8 *)base2 + srcOff;
            void *dst = (u8 *)base2 + off;
            s32 control = (manager->activeCount - i) & 0x1FFFFF;
            s32 cnt;
            void **base3;

            control |= 0x4000000;
            CpuSet(src, dst, control);

            cnt = manager->activeCount;
            base3 = (void **)manager->slotArray;
            base3[cnt - 1] = 0;
            cnt -= 1;
            manager->activeCount = cnt;
        }
    }
done:
    return;
}

/* Removes the entry at `index` from `manager`'s active-object array
 * the same way `RemoveCrateFromList` does after its own search - unlinks it
 * from the grid via `UnlinkCrateFromGrid`, then compacts via `CpuSet`. */
void RemoveCrateListAt(struct pool_manager *manager, s32 index)
{
    if (index < manager->capacity) {
        void **base = (void **)manager->slotArray;
        s32 off = index * 4;
        void *item = base[index];

        UnlinkCrateFromGrid(manager, item);

        {
            s32 srcOff = off + 4;
            void **base2 = (void **)manager->slotArray;
            void *src = (u8 *)base2 + srcOff;
            void *dst = (u8 *)base2 + off;
            s32 control = (manager->activeCount - index) & 0x1FFFFF;
            s32 cnt;
            void **base3;

            control |= 0x4000000;
            CpuSet(src, dst, control);

            cnt = manager->activeCount;
            base3 = (void **)manager->slotArray;
            base3[cnt - 1] = 0;
            cnt -= 1;
            manager->activeCount = cnt;
        }
    }
}

/* Pops a node off `manager->freeListHead` (unlinked via the wrapper
 * entry's own `next` field), reuses it to wrap `data`/`extra`,
 * and inserts it into the spatial hash grid bucket `bucket`:
 * `gridHead` holds each bucket's head pointer (set only the first
 * time a bucket goes from empty), `gridTail` holds each bucket's tail
 * pointer (always updated, chaining the previous tail's `next` field
 * to the new node). Returns the
 * node. */
void *AddCrateGridNode(struct pool_manager *manager, struct box_part *data, s32 bucket, s32 extra)
{
    struct pool_link **headField = &manager->freeListHead;
    struct pool_link *entry = *headField;
    struct pool_node *node = entry->node;

    *headField = entry->next;
    entry->next = 0;

    node->data = data;
    node->next = 0;
    node->link = (struct pool_node *)extra;
    node->mark = 0;
    node->mark2 = 0;

    {
        s32 off = bucket * 4;
        void **gridABase = (void **)manager->gridHead;
        void **gridASlot = (void **)((u8 *)gridABase + off);
        if (*gridASlot == 0) {
            *gridASlot = node;
        }
        {
            void **gridBBase = (void **)manager->gridTail;
            void **gridBSlot = (void **)((u8 *)gridBBase + off);
            struct pool_node *tail = *gridBSlot;
            if (tail != 0) {
                tail->next = node;
            }
            *gridBSlot = node;
        }
    }

    return node;
}

/* Inserts `obj` into the grid via `AddCrateGridNode`, bucketed by
 * the high halfword of `obj->x` (its 256-px column; the ROM loads it
 * with one `ldrsh` from +2). If `obj->flags` bit 4 is set (a "large
 * object" case, spanning more than one cell), also inserts it into
 * the special bucket 0xff (using the first node as the second
 * insertion's "extra" argument), linking the first node's `link`
 * to the second node - the two nodes referencing each other.
 * The ROM never sets up a return value here (its only caller,
 * `AddCrateToList`, ignores it), so this is `void` despite `AddCrateGridNode`
 * itself returning the node. */
void LinkCrateInGrid(struct pool_manager *manager, struct box_part *obj)
{
    s16 bucket = obj->x >> 16;
    struct pool_node *node1 = AddCrateGridNode(manager, obj, bucket, 0);

    {
        MATCH_HOLD_REG(u8, byte, r1) = obj->flags;
        MATCH_HOLD_REG(s32, shifted, r0) = byte >> 4;
        MATCH_HOLD_REG(s32, mask, r1) = 1;
        MATCH_HOLD_REG(s32, test, r0);

        test = shifted & mask;
        if (!test) {
            return;
        }
    }
    {
        void *node2 = AddCrateGridNode(manager, obj, 0xff, (s32)node1);
        node1->link = node2;
    }
}

/* Appends `obj` to `manager->slotArray` if there's room below
 * `capacity`, inserting it into the collision grid via `LinkCrateInGrid`
 * first. */
void AddCrateToList(struct pool_manager *manager, struct box_part *obj)
{
    if (manager->activeCount < manager->capacity) {
        s32 idx;

        LinkCrateInGrid(manager, obj);

        idx = manager->activeCount;
        {
            void **base = (void **)manager->slotArray;
            base[idx] = obj;
        }
        manager->activeCount = idx + 1;
    }
}

/* Tears down a pool manager: frees `freeListArray`, `nodeArray`, and
 * `slotArray`, each via `OperatorDeleteArray` if non-`NULL`; resets `capacity`
 * to 0; and, if a flags bit is set, frees the manager itself via
 * `OperatorDelete`. */
void DestroyCrateList(struct pool_manager *manager, s32 flags)
{
    if (manager->freeListArray != 0) {
        OperatorDeleteArray(manager->freeListArray);
    }
    if (manager->nodeArray != 0) {
        OperatorDeleteArray(manager->nodeArray);
    }
    if (manager->slotArray != 0) {
        OperatorDeleteArray(manager->slotArray);
    }
    manager->capacity = 0;
    if (flags & 1) {
        OperatorDelete(manager);
    }
}
