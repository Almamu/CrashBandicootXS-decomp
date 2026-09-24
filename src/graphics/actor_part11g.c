#include "core.h"
#include "actor.h"

/* Same fixed-slot object-pool/spatial-hash-grid struct `sub_8008F20`
 * initializes (`actor_part11.c`) and `actor_part12.c` operates on - see
 * that file for the full field writeup. */
struct pool_manager {
    s32 activeCount;
    s32 capacity;
    void **slotArray;
    void *nodeArray;
    void *gridHead[256];
    void *gridTail[256];
    void *freeListArray;
    void *freeListHead;
};

/* Searches every bucket (254 down to 0, i.e. every bucket except the
 * special "large object" bucket 255) of `manager`'s spatial hash grid
 * for a node whose data pointer equals `obj`. On the first match: if
 * the object's `+0xc` flags byte bit 4 isn't set, returns immediately
 * (nothing to do). If it IS set but the node already has a bucket-255
 * secondary link (`node->field_0xc != 0`, the same field
 * `sub_8009AF0`/`sub_8009B3C` set up), also returns immediately - the
 * link already exists. Otherwise, pops a fresh node off the free list
 * (the same `sub_8009AF0` pop idiom), wraps `obj` in it, and inserts
 * that new node into bucket 255's head/tail list, finally linking the
 * two nodes together via the original node's `field_0xc` - lazily
 * creating the "large object" bucket-255 registration for an object
 * that didn't get one when it was originally inserted (`sub_8009B3C`
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
 *   `asm volatile("" : "+r"(manager))` barrier placed right before the
 *   address computation, executed once per non-empty bucket (i.e.
 *   still inside the outer loop, same place the computation already
 *   sat), makes `manager`'s value opaque to the optimizer at that
 *   point in the loop, which is enough to defeat the loop-invariant
 *   hoist without needing to hand-write the address arithmetic - the
 *   recomputation lands back in the exact same shared constant pool
 *   (`0x814`/`0x40c`/`0x80c`, in that order) the ROM's own build groups
 *   together right after the function's fall-through return.
 * - The `data->flags` bit-4 test (`(*(data+0xc) >> 4) & 1`) picks a
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
void sub_8009150(struct pool_manager *manager, void *objArg)
{
    register void *obj asm("r3") = objArg;
    s32 bucket;

    for (bucket = 0xFE; bucket >= 0; bucket--) {
        void *node = manager->gridHead[bucket];

        if (node == 0) {
            continue;
        }

        {
            register void **headField asm("r6");

            /* Opaque barrier: prevents this compiler from proving
             * `&manager->freeListHead` is loop-invariant and hoisting
             * it above the outer `for` loop - see the function-level
             * comment above. */
            asm volatile("" : "+r"(manager));
            headField = &manager->freeListHead;

            for (;;) {
                register void *data asm("r5") = *(void **)node;

                if (data == obj) {
                    void *fieldC;
                    /* Register-pinned to match the ROM's own
                     * scratch-register choice for this bit test - see
                     * the function-level comment above. */
                    register u8 flags asm("r1") = *((u8 *)data + 0xc);
                    register s32 shifted asm("r0") = flags >> 4;
                    register s32 mask asm("r1") = 1;
                    register s32 result asm("r0") = shifted & mask;

                    if (!result) {
                        return;
                    }
                    fieldC = *(void **)((u8 *)node + 0xc);
                    if (fieldC != 0) {
                        return;
                    }
                    {
                        void **entry = *headField;
                        register void *newNode asm("r2") = *(void **)entry;

                        *headField = *(void **)((u8 *)entry + 4);
                        *(void **)((u8 *)entry + 4) = fieldC;

                        *(void **)newNode = data;
                        *(void **)((u8 *)newNode + 4) = fieldC;
                        *(void **)((u8 *)newNode + 0xc) = node;
                        *((u8 *)newNode + 0x10) = (u8)(s32)fieldC;
                        *((u8 *)newNode + 0x11) = (u8)(s32)fieldC;

                        if (manager->gridHead[255] == 0) {
                            manager->gridHead[255] = newNode;
                        }
                        if (manager->gridTail[255] != 0) {
                            *(void **)((u8 *)manager->gridTail[255] + 4) = newNode;
                        }
                        manager->gridTail[255] = newNode;
                        *(void **)((u8 *)node + 0xc) = newNode;
                    }
                    return;
                }

                node = *(void **)((u8 *)node + 4);
                if (node == 0) {
                    break;
                }
            }
        }
    }
}
asm(".align 2, 0");
