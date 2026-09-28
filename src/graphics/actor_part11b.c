#include "core.h"
#include "actor.h"

/* The spatial-hash-grid removal primitive `sub_8009A30`/`sub_8009AA0`
 * (`actor_part12.c`) call before compacting `manager`'s active-object
 * array - unlinks `item`'s pool node(s) from `manager`'s grid
 * (the `struct pool_manager` documented in `actor_part12.c`) and
 * returns them to the free list.
 *
 * Two-phase search, matching the two places `sub_8009B3C` (insert side)
 * can register an object:
 *  - Phase 1 walks `item`'s own *primary* bucket
 *    (`gridHead[item->bucket]`, the same bucket index `sub_8009B3C`
 *    computes on insert) looking for a node whose data pointer equals
 *    `item`; on the first match, unlinks it from that bucket's
 *    singly-linked list (head/tail/prev-next patched as needed, mirroring
 *    `sub_8009AF0`'s own insert-side bookkeeping) and pushes the node's
 *    wrapper entry back onto `manager->freeListHead`.
 *  - Phase 2 always runs unless phase 1 found a match *and* neither of
 *    two escape conditions hold (`item->field_8 == 0xFFFF`, or
 *    `item`'s flags byte bit 4 - the "large object" flag `sub_8009150`/
 *    `sub_8009B3C` also test - is clear): it then walks every bucket
 *    from 255 down to 0 (the special "large object" bucket first,
 *    matching where `sub_8009B3C` links a second node for large objects),
 *    removing any further node matching `item` the same way, and stops
 *    scanning a bucket's list once the combined removal count across
 *    both phases exceeds 1.
 *
 * The one oddity that kept this NAKED: after every phase-2 removal the
 * ROM sets the bucket index to 0x100 (before the `count > 1` test), so
 * the outer loop's `i--` restarts the scan at bucket 255 - and a later
 * match in the same list would unlink against `gridHead[0x100]`, which
 * aliases `gridTail[0]`. That is what the original source did, so the
 * C below does it too. Matches under old_agbcc (the object is on the
 * Makefile's OLD_AGBCC_OBJS list); see
 * docs/matching/issue-9-naked-retry.md. */
struct pool_node {
    void *data;
    struct pool_node *next;
    struct pool_node *wrap;
};

struct pool_item {
    u8 unused_00[2];
    s16 bucket;
    u8 unused_04[4];
    u16 field_8;
    u8 unused_0a[2];
    u8 flags;
};

struct pool_manager {
    s32 activeCount;
    s32 capacity;
    void **slotArray;
    void *nodeArray;
    struct pool_node *gridHead[256];
    struct pool_node *gridTail[256];
    void *freeListArray;
    struct pool_node *freeListHead;
};

/* Pushes a removed node's wrapper back onto the free list. The ROM
 * loads the free-list head into r1 before it loads `node->wrap` into
 * r0; the r1 pin reproduces that (unpinned, gcc gives the head r0 and
 * the wrapper r1, or loads the wrapper first). */
#define POOL_FREE_NODE(m, node)                                         \
    {                                                                   \
        register struct pool_node *_head asm("r1") = (m)->freeListHead; \
        (node)->wrap->next = _head;                                     \
        (m)->freeListHead = (node)->wrap;                               \
    }

void sub_8009008(struct pool_manager *manager, struct pool_item *item)
{
    s32 bucket = item->bucket;
    struct pool_node *found = manager->gridHead[bucket];
    struct pool_node *node;
    struct pool_node *prev = NULL;
    s32 count = 0;
    s32 i;

    for (; found != NULL; prev = found, found = found->next) {
        if (found->data == item) {
            found->data = NULL;
            count++;
            if (found == manager->gridHead[bucket]) {
                struct pool_node *next = found->next;

                if (next != NULL) {
                    manager->gridHead[bucket] = next;
                } else {
                    manager->gridHead[bucket] = next;
                    manager->gridTail[bucket] = next;
                }
            } else if (prev != NULL) {
                if (found == manager->gridTail[bucket])
                    manager->gridTail[bucket] = prev;
                prev->next = found->next;
            }
            POOL_FREE_NODE(manager, found);
            break;
        }
    }

    {
        u16 f8 = item->field_8;

        if (found != NULL && f8 != 0xFFFF && !((item->flags >> 4) & 1))
            return;
    }

    for (i = 0xFF; i >= 0; i--) {
        prev = NULL;
        for (node = manager->gridHead[i]; node != NULL; prev = node, node = node->next) {
            if (node->data == item) {
                found = node;
                node->data = NULL;
                count++;
                if (node == manager->gridHead[i]) {
                    struct pool_node *next = node->next;

                    if (next != NULL) {
                        manager->gridHead[i] = next;
                    } else {
                        manager->gridHead[i] = next;
                        manager->gridTail[i] = next;
                    }
                } else if (prev != NULL) {
                    if (node == manager->gridTail[i])
                        manager->gridTail[i] = prev;
                    prev->next = node->next;
                }
                POOL_FREE_NODE(manager, found);
                /* Restarts the bucket scan (see the comment above). */
                i = 0x100;
                if (count > 1)
                    break;
            }
        }
    }
}
asm(".align 2, 0");
