#include "core.h"
#include "actor.h"

extern void *sub_803AD7C(void *arg0, void *fn);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *manager);
extern void *sub_8026EC0(u32 size);
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);

/* The "filter into a second array" manager struct also used by
 * `sub_8008C80`/`sub_8008CEC`/`sub_8008D30` in `actor_part10.c` -
 * `array1` is the primary list (bounded by `count1`, up to
 * `capacity`), `array2` a filtered/derived list built from it
 * (bounded by `count2`). `sub_8008EE4` below is this struct's own
 * initializer. */
struct dual_array_manager {
    s32 capacity;     // +0x0
    s32 count1;         // +0x4
    s32 count2;           // +0x8
    void **array1;          // +0xc
    void **array2;             // +0x10
};

/* Fires a `part->table+0x20/0x24`-driven trampoline via `sub_803AD7C`
 * for every entry in `manager->array2` (bounded by `count2`). */
void sub_8008DC0(struct dual_array_manager *manager)
{
    s32 i;

    for (i = 0; i < manager->count2; i++) {
        void *part = manager->array2[i];
        u8 *tbl = *(u8 **)((u8 *)part + 0x18);
        s16 offset = *(s16 *)(tbl + 0x20);
        void *addr = (u8 *)part + offset;
        void *fn = *(void **)(tbl + 0x24);

        sub_803AD7C(addr, fn);
    }
}

/* Searches `manager->array1` (bounded by `capacity`) for an entry
 * equal to `target`; if found, compacts the array by shifting every
 * following entry down by one slot via the BIOS `CpuSet` wrapper
 * `sub_803A94C`, decrements `count1`, and clears the now-unused
 * trailing slot. Same removal logic as `sub_8008E50` below, but
 * locates the index by value instead of taking it directly as an
 * argument. */
void sub_8008DEC(struct dual_array_manager *manager, void *target)
{
    s32 i = 0;
    s32 count = manager->capacity;
    void **base;

    if (i >= count) {
        goto done;
    }
    {
        void **p0 = manager->array1;
        void *val = *p0;
        base = p0;
        if (val != target) {
            void **p = base;
            do {
                p++;
                i++;
                if (i >= count) {
                    goto done;
                }
            } while (*p != target);
        }
    }

    if (i < manager->capacity) {
        s32 off = i * 4;
        s32 srcOff = off + 4;
        void *src = (u8 *)base + srcOff;
        void *dst = (u8 *)base + off;
        s32 control = (manager->count1 - i) & 0x1FFFFF;
        s32 newCount;

        control |= 0x4000000;
        sub_803A94C(src, dst, control);

        newCount = manager->count1 - 1;
        manager->count1 = newCount;
        manager->array1[newCount] = 0;
    }
done:
    return;
}

/* Removes the entry at `index` from `manager->array1`, compacting via
 * `sub_803A94C` the same way `sub_8008DEC` does after its own
 * search. */
void sub_8008E50(struct dual_array_manager *manager, s32 index)
{
    if (index < manager->capacity) {
        s32 off = index * 4;
        s32 srcOff = off + 4;
        u8 *base;
        void *src, *dst;
        s32 control;
        s32 newCount;

        base = (u8 *)manager->array1;
        src = base + srcOff;
        dst = base + off;
        control = (manager->count1 - index) & 0x1FFFFF;
        control |= 0x4000000;
        sub_803A94C(src, dst, control);

        newCount = manager->count1 - 1;
        manager->count1 = newCount;
        manager->array1[newCount] = 0;
    }
}

/* Appends `value` to `manager->array1` if there's room (`count1` <
 * `capacity`). */
void sub_8008E94(struct dual_array_manager *manager, void *value)
{
    s32 count = manager->count1;

    if (count < manager->capacity) {
        manager->array1[count] = value;
        manager->count1 = count + 1;
    }
}

/* Tears down a manager: frees both of its arrays (`array2` and
 * `array1`, each via `sub_8026EB4` if non-`NULL`), and, if bit 0 of
 * `flags` is set, frees the manager struct itself via
 * `sub_8026ED0`. */
void sub_8008EB4(struct dual_array_manager *manager, s32 flags)
{
    if (manager->array2 != 0) {
        sub_8026EB4(manager->array2);
    }
    if (manager->array1 != 0) {
        sub_8026EB4(manager->array1);
    }
    if (flags & 1) {
        sub_8026ED0(manager);
    }
}

/* Initializes a manager: sets `count1`/`count2` to 0, `capacity` to
 * `count`, allocates two `count`-word arrays via `sub_8026EC0` for
 * `array1`/`array2`, and zero-fills `array1`. Returns `manager`. */
struct dual_array_manager *sub_8008EE4(struct dual_array_manager *manager, s32 count)
{
    s32 i;
    void **arr;
    s32 allocSize;

    manager->count1 = 0;
    manager->count2 = 0;
    manager->capacity = count;

    allocSize = count * 4;
    manager->array1 = sub_8026EC0(allocSize);
    manager->array2 = sub_8026EC0(allocSize);

    i = manager->capacity;
    if (i > 0) {
        void *zero = 0;
        arr = manager->array1;
        do {
            *arr = zero;
            arr++;
            i--;
        } while (i != 0);
    }

    return manager;
}

#if NON_MATCHING
/* sub_8008F20 initializes a fixed-slot object pool "manager" struct:
 *  +0x0: s32 activeCount (0)
 *  +0x4: s32 capacity (= count)
 *  +0x8: void **slotArray - `count` pointers, allocated via
 *        sub_8026EC0(count*4), zero-filled
 *  +0xc: u8 *nodeArray - `count` 0x14-byte nodes, allocated via
 *        sub_8026EC0(count*0x14)
 *  +0x10..0x40F: a 256-word (0x400-byte) table, zeroed
 *  +0x410..0x80F: a second 256-word (0x400-byte) table, zeroed
 *  +0x810: void *freeListArray - `count` 8-byte {node, next} pairs,
 *          allocated via sub_8026EC0(count*8)
 *  +0x814: void *freeListHead - set to `freeListArray` itself at the
 *          very end
 *
 * For each node index i (0..count-1): freeListArray[i].node points at
 * nodeArray[i]; nodeArray[i]'s fields 0/4/0xc and byte 0x10 are
 * zeroed; nodeArray[i]'s field 8 is set to point back at
 * freeListArray[i]; and freeListArray[i].next is chained to
 * freeListArray[i+1] (or NULL for the last entry) - building a
 * singly-linked free list of pool nodes through the wrapper array.
 * The two big 256-word tables (+0x10, +0x410) are very likely a pair
 * of spatial-partition/collision grids, consistent with this whole
 * region being part of docs/rom_map.md's still-only-partially-
 * understood AI/collision dispatch system - but that broader purpose
 * isn't needed to confirm every individual load/store here, which are
 * all confirmed correct.
 *
 * NOT YET BYTE-MATCHING: this compiler allocates the persistent
 * cross-loop values (item count, the +0x810/+0x814 field addresses,
 * the loop index reused from the zero-fill counter, the running
 * "next" byte offset) across r3/sb/sl/r4/r8 in a specific combination
 * this reconstruction doesn't reproduce - the isolated compile
 * produces the right *shape* (same branches, same use of `ip`/r8, and
 * even a matching high-register save/restore prologue/epilogue) but a
 * different concrete register assignment for several of the four
 * long-lived scalars. Parked rather than chase a many-register
 * allocation puzzle for a single function - see docs/matching.md,
 * "Parked, not matched: sub_8008F20". */
void *sub_8008F20(void *manager, s32 count)
{
    void *allocA, *allocB, *allocC;
    s32 n;

    *(s32 *)manager = 0;
    *(s32 *)((u8 *)manager + 4) = count;

    allocA = sub_8026EC0(count * 4);
    *(void **)((u8 *)manager + 8) = allocA;

    allocB = sub_8026EC0((count * 4 + count) * 4);
    *(void **)((u8 *)manager + 0xc) = allocB;

    allocC = sub_8026EC0(count * 8);
    *(void **)((u8 *)manager + 0x810) = allocC;

    n = *(s32 *)((u8 *)manager + 4);
    if (n > 0) {
        void *zero = 0;
        void **arr = *(void ***)((u8 *)manager + 8);
        do {
            *arr = zero;
            arr++;
            n--;
        } while (n != 0);
    }

    {
        s32 count2 = *(s32 *)((u8 *)manager + 4);
        void **field810 = (void **)((u8 *)manager + 0x810);
        void **field814 = (void **)((u8 *)manager + 0x814);
        u8 *p1 = (u8 *)manager + 0x10;
        u8 *p2 = (u8 *)manager + 0x410;
        s32 m = 0xFF;
        s32 zero2 = 0;
        do {
            *(s32 *)p1 = zero2;
            p1 += 4;
            *(s32 *)p2 = zero2;
            p2 += 4;
            m--;
        } while (m >= 0);

        {
            s32 i = 0;
            if (i < count2) {
                s32 nextOff = 8;
                s32 nodeOff = 0;
                do {
                    void *freeListArr = *field810;
                    s32 idxOff = i * 8;
                    void *entry = (u8 *)freeListArr + idxOff;
                    void *node = *(void **)((u8 *)manager + 0xc);
                    node = (u8 *)node + nodeOff;
                    *(void **)entry = node;

                    node = *(void **)((u8 *)manager + 0xc);
                    node = (u8 *)node + nodeOff;
                    *(s32 *)node = 0;
                    *(s32 *)((u8 *)node + 4) = 0;
                    *(s32 *)((u8 *)node + 0xc) = 0;
                    *((u8 *)node + 0x10) = 0;

                    node = *(void **)((u8 *)manager + 0xc);
                    node = (u8 *)node + nodeOff;
                    freeListArr = *field810;
                    entry = (u8 *)freeListArr + idxOff;
                    *(void **)((u8 *)node + 8) = entry;

                    if (i == *(s32 *)((u8 *)manager + 4) - 1) {
                        *(s32 *)((u8 *)entry + 4) = 0;
                    } else {
                        *(void **)((u8 *)entry + 4) = (u8 *)freeListArr + nextOff;
                    }

                    nextOff += 8;
                    nodeOff += 0x14;
                    i++;
                } while (i < *(s32 *)((u8 *)manager + 4));
            }
        }

        *field814 = *field810;
    }

    return manager;
}
#endif /* NON_MATCHING */

#if NON_MATCHING
/* Same pool-manager struct sub_8008F20 initializes and actor_part12.c
 * operates on - see that file for the full field writeup. */
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
 * NOT YET BYTE-MATCHING, but extremely close - every load, store, and
 * field offset is confirmed correct, and explicit register pins
 * (`obj`/`data`/`headField`/`newNode` to `r3`/`r5`/`r6`/`r2`, matching
 * a byte-for-byte-verified value/register-reuse chain through the
 * `fieldC` "recycled zero" idiom) reproduce the whole function except
 * one detail: the free-list-head field's address (`manager+0x814`) is
 * loop-invariant across the whole 255-bucket outer loop, and this
 * compiler correctly recognizes that and hoists the computation
 * outside the loop (computing it once, then just copying the cached
 * value into place per bucket) - but the ROM instead recomputes it
 * fresh every time a non-empty bucket is found. No portable C
 * construct tried (an inline-asm memory clobber included) discourages
 * this specific loop-invariant hoist. Parked on this single 2-byte
 * gap - see docs/matching.md, "Parked, not matched: `sub_8009150`". */
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
            register void **headField asm("r6") = &manager->freeListHead;

            for (;;) {
                register void *data asm("r5") = *(void **)node;

                if (data == obj) {
                    void *fieldC;

                    if (!((*((u8 *)data + 0xc) >> 4) & 1)) {
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
#endif /* NON_MATCHING */

#if NON_MATCHING
/* Resets a pool manager to empty: tears down every active object
 * (`slotArray[0..activeCount)`, firing each one's `table+0x50/0x54`
 * trampoline via `sub_803AD80` with constant arg `3` if non-`NULL`,
 * then clearing the slot), resets `activeCount` to 0, and rebuilds
 * both the grid (`gridHead`/`gridTail` zeroed) and the free list from
 * scratch over `nodeArray` - the exact same free-list-build loop
 * `sub_8008F20` performs during initialization.
 *
 * NOT YET BYTE-MATCHING: the active-object teardown loop (the first
 * half) is confirmed correct and matches in isolation, but the
 * free-list-rebuild loop (the second half) is a byte-for-byte copy of
 * `sub_8008F20`'s own tail and hits the exact same many-register
 * allocation gap documented there - the ROM keeps three persistent
 * high registers (`sb`/`sl`/`r8`) alive across the whole loop, while
 * this reconstruction's most faithful attempt still only needs two.
 * Parked for the same reason as `sub_8008F20` - see docs/matching.md,
 * "Parked, not matched: `sub_8009914`". */
void sub_8009914(struct pool_manager *manager)
{
    s32 i;

    for (i = 0; i < manager->activeCount; i++) {
        struct actor *part = manager->slotArray[i];

        if (part != 0) {
            u8 *rec = (u8 *)part->table + 0x50;
            s16 offset = *(s16 *)rec;
            void *addr = (u8 *)part + offset;
            void *fn = *(void **)(rec + 4);

            sub_803AD80(addr, (void *)3, fn);
        }
        manager->slotArray[i] = 0;
    }

    manager->activeCount = 0;

    {
        s32 capacity = manager->capacity;
        void **field810 = &manager->freeListArray;
        void **field814 = &manager->freeListHead;
        void **gridHead = manager->gridHead;
        void **gridTail = manager->gridTail;
        s32 m = 0xFF;
        void *zero = 0;

        do {
            *gridHead = zero;
            gridHead++;
            *gridTail = zero;
            gridTail++;
            m--;
        } while (m >= 0);

        {
            s32 i2 = 0;
            if (i2 < capacity) {
                s32 nextOff = 8;
                s32 nodeOff = 0;
                do {
                    void *freeListArr = *field810;
                    s32 idxOff = i2 * 8;
                    void *entry = (u8 *)freeListArr + idxOff;
                    void *node = (u8 *)manager->nodeArray + nodeOff;

                    *(void **)entry = node;

                    node = (u8 *)manager->nodeArray + nodeOff;
                    *(s32 *)node = 0;
                    *(s32 *)((u8 *)node + 4) = 0;
                    *(s32 *)((u8 *)node + 0xc) = 0;
                    *((u8 *)node + 0x10) = 0;

                    node = (u8 *)manager->nodeArray + nodeOff;
                    freeListArr = *field810;
                    entry = (u8 *)freeListArr + idxOff;
                    *(void **)((u8 *)node + 8) = entry;

                    if (i2 == manager->capacity - 1) {
                        *(s32 *)((u8 *)entry + 4) = 0;
                    } else {
                        *(void **)((u8 *)entry + 4) = (u8 *)freeListArr + nextOff;
                    }

                    nextOff += 8;
                    nodeOff += 0x14;
                    i2++;
                } while (i2 < manager->capacity);
            }
        }

        *field814 = *field810;
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
