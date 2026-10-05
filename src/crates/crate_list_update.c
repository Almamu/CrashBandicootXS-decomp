#include "core.h"
#include "actor.h"
#include "box_part.h"
#include <agb_syscall.h>
#include "crates.h"

extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);
extern void *_call_via_r1(void *arg0, void *fn);
extern void *gLevelLayers;

/* A per-frame spatial-hash-grid maintenance pass over `manager`'s
 * `struct pool_manager` (`crate_list.c`), scoped to the 3-bucket
 * window `[baseIdx, baseIdx+2]` around `baseIdx` (the same
 * `max(gLevelLayers`'s sub-object's own `x >> 8`, `0)` bucket
 * index `DrawCrateList`/`CollideCrateGrid` compute), plus the special "large
 * object" bucket 255 in a second pass - not a full 0-255 sweep like
 * those two sibling functions.
 *
 * For each node's object (`part`) in the windowed buckets:
 *  - If `part->flags` bit 4 ("large object") is set and this node has
 *    no bucket-255 secondary link yet (`node+0xc == 0`), lazily creates
 *    one - an inline copy of `LinkCrateToActiveBucket`'s own body (pop a node off
 *    the free list, wrap `part` in it, splice it into bucket 255's
 *    head/tail list, cross-link the two nodes via `+0xc`).
 *  - Otherwise, if `part->flags` bit 0 is set (a "pending removal"
 *    flag), removes `part` from `manager`'s active-object array the
 *    same way `RemoveCrateFromList` does (linear search, `UnlinkCrateFromGrid` to
 *    unlink the grid node(s), `CpuSet`-based compaction), then
 *    fires a `part->table+0x50/0x54`-driven trampoline via
 *    `_call_via_r2` with constant arg `3` - the exact same "destroy"
 *    trampoline `ResetCrateList`'s teardown loop fires.
 *  - Otherwise, tests `part` against a computed box (the tracked
 *    sub-object's position, offset by fixed constants `-0x6400`/
 *    `-0x3C00` in Q8 and sized `0x1B8`x`0x118` in Q8 - an "extended"
 *    region wider than the plain 240x160 screen box `DrawCrateList`/
 *    `CollideCrateGrid` use, meaning/purpose not yet confirmed) via a
 *    `part->table+0x40/0x44`-driven trampoline; on a hit, fires a
 *    `part->table+0x18/0x1c`-driven trampoline and marks the node
 *    (`node+0x10 = 1`) so the bucket-255 second pass knows to skip a
 *    node already handled via its primary bucket (clearing the mark
 *    instead), mirroring `DrawCrateList`'s own primary/secondary-pass
 *    marking convention (there via `node+0x11`).
 *
 * Built with old_agbcc (see `OLD_AGBCC_OBJS` in the Makefile).
 * docs/matching/issue-9-raw-asm-pass.md has how it was matched. */
struct pool_node {
    struct box_part *data;
    struct pool_node *next;
    void *wrap;
    struct pool_node *link;
    u8 mark;
    u8 mark2;
};

struct pool_entry {
    struct pool_node *node;
    struct pool_entry *next;
};

struct pool_manager {
    s32 activeCount;
    s32 capacity;
    struct box_part **slotArray;
    void *nodeArray;
    struct pool_node *gridHead[256];
    struct pool_node *gridTail[256];
    void *freeListArray;
    struct pool_entry *freeListHead;
};

struct track_obj {
    u8 unused_00[0x10];
    s32 *pos;
};

/* `RemoveCrateFromList`'s body, inlined. `holdR2` is a constant: nonzero only
 * for the first loop's copy (see the hold below). */
static inline void pool_remove(struct pool_manager *manager, struct box_part *target, s32 holdR2)
{
    s32 i = 0;
    s32 searchCount = manager->capacity;
    struct box_part **base;
    register s32 hold asm("r2");

    /* Emits no code. It keeps gcse's copy of `capacity` from landing
     * right after the load; cse2 would otherwise swap the two and put
     * the loaded value in the pre/post tests instead of the loop test. */
    asm("");

    if (i >= searchCount) {
        goto done;
    }
    {
        struct box_part **p0 = manager->slotArray;
        struct box_part *val = *p0;
        base = p0;
        if (val != target) {
            struct box_part **p = base;
            do {
                p++;
                i++;
                if (i >= searchCount) {
                    goto done;
                }
            } while (*p != target);
        }
    }

    /* Hard-register hold (emits no code): in the first loop's copy, r2
     * is live from here until `base[i]` is read, so `base` (live across
     * that span) can't take r2. It goes to r3 and `capacity` gets r2,
     * as in the ROM. The second loop's copy already matches without it. */
    if (holdR2)
        asm("" : "=r"(hold));
    if (i < manager->capacity) {
        s32 off = i * 4;
        struct box_part *item = base[i];

        /* End of the hold above (emits no code). */
        if (holdR2)
            asm("" : : "r"(hold));
        UnlinkCrateFromGrid(manager, (struct pool_item *)item);

        {
            s32 srcOff = off + 4;
            struct box_part **base2 = manager->slotArray;
            void *src = (u8 *)base2 + srcOff;
            void *dst = (u8 *)base2 + off;
            s32 control = (manager->activeCount - i) & 0x1FFFFF;
            s32 cnt;
            struct box_part **base3;

            control |= 0x4000000;
            CpuSet(src, dst, control);

            cnt = manager->activeCount;
            base3 = manager->slotArray;
            *(void **)((u8 *)base3 + cnt * 4 - 4) = 0;
            cnt -= 1;
            manager->activeCount = cnt;
        }
    }
done:
    return;
}

static inline void part_destroy(struct box_part *part)
{
    if (part != NULL) {
        struct part_method *m = PART_METHOD(part, 0x50);

        _call_via_r2((u8 *)part + m->thisOffset, (void *)3, m->fn);
    }
}

/* The statement expressions give each callee its own copy of the
 * pointer (the ROM's sb/ip and sl/sb pairs). */
#define PART_COPY(p) ({ struct box_part *_t = (p); _t; })

static inline void pool_destroy(struct pool_manager *manager, struct box_part *obj, s32 holdR2)
{
    pool_remove(manager, PART_COPY(obj), holdR2);
    part_destroy(PART_COPY(obj));
}

void UpdateCrateList(struct pool_manager *manager)
{
    s32 box[4];
    s32 *pos;
    s32 base;
    s32 i;
    s32 next;
    struct pool_node *node;
    struct pool_node **gridHeadBase;
    struct pool_node **gridHead255;
    s32 v2, v3, v0, v1;

    v2 = 0x1b800;
    v3 = 0x11800;
    box[2] = v2;
    box[3] = v3;
    pos = ((struct track_obj *)gLevelLayers)->pos;
    v0 = (pos[0] << 8) - 0x6400;
    v1 = (pos[1] << 8) - 0x3c00;
    box[0] = v0;
    box[1] = v1;
    base = pos[0];
    base >>= 8;
    if (base < 0)
        base = 0;

    i = base + 2;
    gridHeadBase = manager->gridHead;
    gridHead255 = &manager->gridHead[255];
    do {
        /* Byte arithmetic on the base gives the ROM's `adds r0, r2, r0`
         * operand order; `gridHeadBase[i]` swaps the operands. */
        node = *(struct pool_node **)((s32)gridHeadBase + (i << 2));
        next = i - 1;
        while (node != NULL) {
            /* Reading `node->data` twice gives the ROM's load into r2
             * and the copy into r5. */
            s32 large = (node->data->flags >> 4) & 1;
            struct box_part *part = node->data;

            if (large && node->link == NULL) {
                struct pool_entry *entry = manager->freeListHead;
                struct pool_node *newNode = entry->node;

                manager->freeListHead = entry->next;
                entry->next = NULL;
                newNode->data = part;
                newNode->next = NULL;
                newNode->link = node;
                newNode->mark = 0;
                newNode->mark2 = 0;
                if (*gridHead255 == NULL)
                    *gridHead255 = newNode;
                if (manager->gridTail[255] != NULL)
                    manager->gridTail[255]->next = newNode;
                manager->gridTail[255] = newNode;
                node->link = newNode;
            } else if (part->flags & 1) {
                pool_destroy(manager, PART_COPY(part), 1);
            } else {
                struct part_method *m = PART_METHOD(part, 0x40);

                if ((u8)_call_via_r2((u8 *)part + m->thisOffset, box, m->fn)) {
                    struct box_part *p2 = node->data;
                    struct part_method *m2 = PART_METHOD(p2, 0x18);

                    _call_via_r1((u8 *)p2 + m2->thisOffset, m2->fn);
                    node->mark = 1;
                }
            }
            node = node->next;
        }
        i = next;
    } while (i >= base);

    for (node = *gridHead255; node != NULL; node = node->next) {
        struct box_part *part = node->data;

        if (part->flags & 1) {
            pool_destroy(manager, PART_COPY(part), 0);
        } else if (node->link->mark == 0) {
            struct part_method *m = PART_METHOD(part, 0x18);

            _call_via_r1((u8 *)part + m->thisOffset, m->fn);
        } else {
            node->link->mark = 0;
        }
    }
}
asm(".align 2, 0");
