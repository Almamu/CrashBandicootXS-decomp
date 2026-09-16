#include "core.h"
#include "actor.h"

extern void *sub_803AD7C(void *arg0, void *fn);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *manager);
extern void *sub_8026EC0(u32 size);

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
asm(".align 2, 0");
