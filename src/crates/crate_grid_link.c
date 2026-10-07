#include "core.h"
#include "match.h"
#include "actor.h"
#include "box_part.h"
#include "crates.h"

/* Same fixed-slot object-pool/spatial-hash-grid struct `InitCrateList`
 * initializes (`part_list.cpp`) and `crate_list.c` operates on - see
 * that file for the full field writeup. */
/* Searches every bucket (254 down to 0, i.e. every bucket except the
 * special "large object" bucket 255) of `manager`'s spatial hash grid
 * for a node whose data pointer equals `obj`. On the first match: if
 * the object's `flags` bit 4 isn't set, returns immediately
 * (nothing to do). If it IS set but the node already has a bucket-255
 * secondary link (`node->link != 0`, the same field
 * `AddCrateGridNode`/`LinkCrateInGrid` set up), also returns immediately - the
 * link already exists. Otherwise, pops a fresh node off the free list
 * (the same `AddCrateGridNode` pop idiom), wraps `obj` in it, and inserts
 * that new node into bucket 255's head/tail list, finally linking the
 * two nodes together via the original node's `link` - lazily
 * creating the "large object" bucket-255 registration for an object
 * that didn't get one when it was originally inserted (`LinkCrateInGrid`
 * only creates it when `obj->flags` bit 4 is already set at insert
 * time; this looks like the retroactive counterpart, called when an
 * object transitions to "large" status after insertion).
 *
 * Two compiler gaps, both closed with opaque-to-the-optimizer register
 * pins rather than any change in behavior:
 *
 * - The free-list-head field's address (`&manager->freeListHead`, i.e.
 *   `manager+0x814`) doesn't change across the whole 255-bucket outer
 *   loop, so a plain `&manager->freeListHead` naturally gets hoisted
 *   out of the loop by this compiler (computed once before the loop,
 *   then just copied into place per bucket) - but the ROM instead
 *   recomputes it fresh every time a non-empty bucket is found. An
 *   `MATCH_KEEP_VOLATILE(manager)` barrier placed right before the
 *   address computation, executed once per non-empty bucket (i.e.
 *   still inside the outer loop, same place the computation already
 *   sat), makes `manager`'s value opaque to the optimizer at that
 *   point in the loop, which is enough to defeat the loop-invariant
 *   hoist without needing to hand-write the address arithmetic - the
 *   recomputation lands back in the exact same shared constant pool
 *   (`0x814`/`0x40c`/`0x80c`, in that order) the ROM's own build groups
 *   together right after the function's fall-through return.
 * - The `data->flags` bit-4 test (`(data->flags >> 4) & 1`) picks a
 *   different pair of scratch registers than the ROM for the
 *   byte-load/shift/mask sequence (`r0`/`r0` here vs. the ROM's
 *   `r1`/`r0`) - an ordinary small register-choice gap, fixed the usual
 *   way by pinning each intermediate value to the register the ROM
 *   actually used (`flags`/`shifted`/`mask`/`result` to
 *   `r1`/`r0`/`r1`/`r0`).
 *
 * Verified byte-identical via an isolated `agbcc` compile assembled
 * with `arm-none-eabi-as` and compared directly against the ROM's raw
 * bytes, then confirmed again by a full clean `make compare`. */
void LinkCrateToActiveBucket(struct pool_manager *manager, struct box_part *objArg)
{
    MATCH_HOLD_REG(struct box_part *, obj, r3) = objArg;
    s32 bucket;

    for (bucket = 0xFE; bucket >= 0; bucket--) {
        struct pool_node *node = manager->gridHead[bucket];

        if (node == 0) {
            continue;
        }

        {
            MATCH_HOLD_REG(struct pool_link **, headField, r6);

            /* Opaque barrier: prevents this compiler from proving
             * `&manager->freeListHead` is loop-invariant and hoisting
             * it above the outer `for` loop - see the function-level
             * comment above. */
            MATCH_KEEP_VOLATILE(manager);
            headField = &manager->freeListHead;

            for (;;) {
                MATCH_HOLD_REG(struct box_part *, data, r5) = node->data;

                if (data == obj) {
                    struct pool_node *link;
                    /* Register-pinned to match the ROM's own
                     * scratch-register choice for this bit test - see
                     * the function-level comment above. */
                    MATCH_HOLD_REG(u8, flags, r1) = data->flags;
                    MATCH_HOLD_REG(s32, shifted, r0) = flags >> 4;
                    MATCH_HOLD_REG(s32, mask, r1) = 1;
                    MATCH_HOLD_REG(s32, result, r0) = shifted & mask;

                    if (!result) {
                        return;
                    }
                    link = node->link;
                    if (link != 0) {
                        return;
                    }
                    {
                        struct pool_link *entry = *headField;
                        MATCH_HOLD_REG(struct pool_node *, newNode, r2) = entry->node;

                        /* `link` is NULL here: the ROM stores its register
                         * as every zero below. */
                        *headField = entry->next;
                        entry->next = (struct pool_link *)link;

                        newNode->data = data;
                        newNode->next = link;
                        newNode->link = node;
                        newNode->mark = (u8)(s32)link;
                        newNode->mark2 = (u8)(s32)link;

                        if (manager->gridHead[255] == 0) {
                            manager->gridHead[255] = newNode;
                        }
                        if (manager->gridTail[255] != 0) {
                            manager->gridTail[255]->next = newNode;
                        }
                        manager->gridTail[255] = newNode;
                        node->link = newNode;
                    }
                    return;
                }

                node = node->next;
                if (node == 0) {
                    break;
                }
            }
        }
    }
}
