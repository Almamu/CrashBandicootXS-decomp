#include "core.h"
#include "actor.h"

/* The fixed-slot object-pool manager struct `sub_8008F20`
 * (`actor_part11.c`) initializes: `slotArray` holds the active
 * objects (bounded by `activeCount`, up to `capacity`); `nodeArray`
 * is a flat array of `capacity` 0x14-byte pool nodes; `gridHead`/
 * `gridTail` are a 256-bucket spatial hash grid, each bucket a
 * singly-linked list of pool nodes (head set once when a bucket
 * leaves empty, tail always updated for O(1) append - see
 * `sub_8009AF0`); `freeListArray` is `capacity` 8-byte {node, next}
 * pairs threaded into a singly-linked free list, `freeListHead`
 * pointing at its first still-free entry. */
struct pool_manager {
    s32 activeCount;         // +0x0
    s32 capacity;               // +0x4
    void **slotArray;              // +0x8
    void *nodeArray;                  // +0xc
    void *gridHead[256];                 // +0x10
    void *gridTail[256];                    // +0x410
    void *freeListArray;                       // +0x810
    void *freeListHead;                           // +0x814
};

extern void sub_8009008(struct pool_manager *manager, void *item);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void *sub_8009AF0(struct pool_manager *manager, void *data, s32 bucket, s32 extra);
extern void sub_8009B3C(struct pool_manager *manager, void *obj);
extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *manager);

extern s32 sub_8009FF4(void *part, void *region);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* `sub_8008D80`'s sibling: the same collision-hit resolver, called
 * from elsewhere in this AI/collision cluster (`manager` itself is
 * never read here either, a dead parameter kept for a uniform call
 * signature). Tests `part` against the incoming box via `sub_8009FF4`;
 * on a hit, fires a `part->table+0x68`-driven trampoline (same
 * "dead read" idiom already established for `sub_8008AD8`/
 * `sub_8008D80`) with `otherViewport->field_0A` as the third
 * argument, then sets `otherViewport->flags` bit 3.
 *
 * PARKED AS NAKED: identical structural gap as `sub_8008D80` - this
 * compiler has no way to leave `boxH` untouched in its own incoming
 * stack slot while still building a 4-word AABB pointer that includes
 * it, the ABI stack-layout trick the ROM's own tighter local frame
 * relies on. Every load, store, branch and computed delta is
 * confirmed correct (in fact this function's instruction stream is
 * byte-identical to `sub_8008D80`'s, down to the label offsets), so
 * it's hand-transcribed as literal Thumb asm instead of guessed at in
 * C - same technique as `sub_8008D80` (`actor_part7b.c`). See
 * `docs/matching.md`, "Parked, not matched: `sub_80099F0`". */
NAKED void sub_80099F0(void *manager, s32 boxX, s32 boxY, s32 boxW, s32 boxH, void *partArg, void *otherViewportArg)
{
    asm(
        "sub sp, #0xc\n\t"
        "push {r4, r5, lr}\n\t"
        "str r1, [sp, #0xc]\n\t"
        "str r2, [sp, #0x10]\n\t"
        "str r3, [sp, #0x14]\n\t"
        "ldr r4, [sp, #0x1c]\n\t"
        "ldr r5, [sp, #0x20]\n\t"
        "add r0, r4, #0\n\t"
        "add r1, sp, #0xc\n\t"
        "bl sub_8009FF4\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldrb r2, [r5, #0xa]\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #1\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "mov r0, #8\n\t"
        "ldrb r1, [r5, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r5, #0xc]\n\t"
    "1:\n\t"
        "pop {r4, r5}\n\t"
        "pop {r3}\n\t"
        "add sp, #0xc\n\t"
        "bx r3\n\t"
    );
}
asm(".align 2, 0");

/* Searches `manager->slotArray` (bounded by `capacity`, for the
 * search) for `target`; if found, removes it from the collision grid
 * and returns its pool node to the free list via `sub_8009008`, then
 * compacts the array (bounded this time by `activeCount`) via the
 * same CpuSet-based shift used throughout this cluster, decrementing
 * `activeCount`. */
void sub_8009A30(struct pool_manager *manager, void *target)
{
    s32 i = 0;
    s32 searchCount = manager->capacity;
    void **base;

    if (i >= searchCount) {
        goto done;
    }
    {
        void **p0 = manager->slotArray;
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

        sub_8009008(manager, item);

        {
            s32 srcOff = off + 4;
            void **base2 = manager->slotArray;
            void *src = (u8 *)base2 + srcOff;
            void *dst = (u8 *)base2 + off;
            s32 control = (manager->activeCount - i) & 0x1FFFFF;
            s32 cnt;
            void **base3;

            control |= 0x4000000;
            sub_803A94C(src, dst, control);

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

/* Removes the entry at `index` from `manager`'s active-object array
 * the same way `sub_8009A30` does after its own search - unlinks it
 * from the grid via `sub_8009008`, then compacts via `sub_803A94C`. */
void sub_8009AA0(struct pool_manager *manager, s32 index)
{
    if (index < manager->capacity) {
        void **base = manager->slotArray;
        s32 off = index * 4;
        void *item = base[index];

        sub_8009008(manager, item);

        {
            s32 srcOff = off + 4;
            void **base2 = manager->slotArray;
            void *src = (u8 *)base2 + srcOff;
            void *dst = (u8 *)base2 + off;
            s32 control = (manager->activeCount - index) & 0x1FFFFF;
            s32 cnt;
            void **base3;

            control |= 0x4000000;
            sub_803A94C(src, dst, control);

            cnt = manager->activeCount;
            base3 = manager->slotArray;
            *(void **)((u8 *)base3 + cnt * 4 - 4) = 0;
            cnt -= 1;
            manager->activeCount = cnt;
        }
    }
}

/* Pops a node off `manager->freeListHead` (unlinked via the wrapper
 * entry's own `+4` "next" field), reuses it to wrap `data`/`extra`,
 * and inserts it into the spatial hash grid bucket `bucket`:
 * `gridHead` holds each bucket's head pointer (set only the first
 * time a bucket goes from empty), `gridTail` holds each bucket's tail
 * pointer (always updated, chaining the previous tail's `+4` "next"
 * field to the new node). Returns the
 * node. */
void *sub_8009AF0(struct pool_manager *manager, void *data, s32 bucket, s32 extra)
{
    void **headField = &manager->freeListHead;
    void **entry = *headField;
    void *node = *(void **)entry;

    *headField = *(void **)((u8 *)entry + 4);
    *(void **)((u8 *)entry + 4) = 0;

    *(void **)node = data;
    *(void **)((u8 *)node + 4) = 0;
    *(s32 *)((u8 *)node + 0xc) = extra;
    *((u8 *)node + 0x10) = 0;
    *((u8 *)node + 0x11) = 0;

    {
        s32 off = bucket * 4;
        void **gridABase = manager->gridHead;
        void **gridASlot = (void **)((u8 *)gridABase + off);
        if (*gridASlot == 0) {
            *gridASlot = node;
        }
        {
            void **gridBBase = manager->gridTail;
            void **gridBSlot = (void **)((u8 *)gridBBase + off);
            void *tail = *gridBSlot;
            if (tail != 0) {
                *(void **)((u8 *)tail + 4) = node;
            }
            *gridBSlot = node;
        }
    }

    return node;
}

/* Inserts `obj` into the grid via `sub_8009AF0`, bucketed by
 * `obj`'s own `+2` field. If `obj->flags` bit 4 is set (a "large
 * object" case, spanning more than one cell), also inserts it into
 * the special bucket 0xff (using the first node as the second
 * insertion's "extra" argument), linking the first node's `+0xc`
 * field to the second node - the two nodes referencing each other.
 * The ROM never sets up a return value here (its only caller,
 * `sub_8009B70`, ignores it), so this is `void` despite `sub_8009AF0`
 * itself returning the node. */
void sub_8009B3C(struct pool_manager *manager, void *obj)
{
    s16 bucket = *(s16 *)((u8 *)obj + 2);
    void *node1 = sub_8009AF0(manager, obj, bucket, 0);

    {
        register u8 byte asm("r1") = *((u8 *)obj + 0xc);
        register s32 shifted asm("r0") = byte >> 4;
        register s32 mask asm("r1") = 1;
        register s32 test asm("r0");

        test = shifted & mask;
        if (!test) {
            return;
        }
    }
    {
        void *node2 = sub_8009AF0(manager, obj, 0xff, (s32)node1);
        *(void **)((u8 *)node1 + 0xc) = node2;
    }
}

/* Appends `obj` to `manager->slotArray` if there's room below
 * `capacity`, inserting it into the collision grid via `sub_8009B3C`
 * first. */
void sub_8009B70(struct pool_manager *manager, void *obj)
{
    if (manager->activeCount < manager->capacity) {
        s32 idx;

        sub_8009B3C(manager, obj);

        idx = manager->activeCount;
        {
            void **base = manager->slotArray;
            base[idx] = obj;
        }
        manager->activeCount = idx + 1;
    }
}

/* Tears down a pool manager: frees `freeListArray`, `nodeArray`, and
 * `slotArray`, each via `sub_8026EB4` if non-`NULL`; resets `capacity`
 * to 0; and, if a flags bit is set, frees the manager itself via
 * `sub_8026ED0`. */
void sub_8009B9C(struct pool_manager *manager, s32 flags)
{
    if (manager->freeListArray != 0) {
        sub_8026EB4(manager->freeListArray);
    }
    if (manager->nodeArray != 0) {
        sub_8026EB4(manager->nodeArray);
    }
    if (manager->slotArray != 0) {
        sub_8026EB4(manager->slotArray);
    }
    manager->capacity = 0;
    if (flags & 1) {
        sub_8026ED0(manager);
    }
}
asm(".align 2, 0");
