#include "core.h"
#include "actor.h"
#include "actor_self.h"

extern void *sub_803AD7C(void *arg0, void *fn);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *manager);
extern void *sub_8026EC0(u32 size);
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void *gUnknown_03001308;

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

/* What the managers' lists hold: level objects with a gcc 2.x method
 * table at +0x18 (the same prefix as gobj_1a794.h's `struct gobj`, whose
 * header can't be included here - its externs clash with this file's
 * definitions). */
struct listed_obj_vtable {
    u8 unk_00[0x20];
    struct actor_method m20;    // 0x20 - per-frame update
};

struct listed_obj {
    u8 unk_00[0x18];
    struct listed_obj_vtable *vtable; // 0x18
};

/* Calls the `m20` virtual method (via the `sub_803AD7C` call thunk) of
 * every object in `manager->array2` (bounded by `count2`). */
void sub_8008DC0(struct dual_array_manager *manager)
{
    s32 i;

    for (i = 0; i < manager->count2; i++) {
        struct listed_obj *part = manager->array2[i];
        struct listed_obj_vtable *tbl = part->vtable;
        s16 offset = tbl->m20.thisOffset;
        void *addr = (u8 *)part + offset;
        void *fn = tbl->m20.fn;

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
 * Real C (issue #9-#11 NAKED retry; matches under both compilers). The
 * tail is the `PoolResetFreeList` inline (sub_8009914 ends with the same
 * code). Two source details carry the register assignment the old
 * notes blamed on the allocator: the grid clear is a plain indexed
 * `for` loop (gcc reverses it into the ROM's `i = 255 .. 0` countdown
 * with two post-increment pointers), and the free-list loop reads the
 * wrapper array through a `fl = freeList` copy taken inside the
 * `if (i < n)` guard, right before the `do` - that is the ROM's
 * `mov ip, sb` at the loop head. With `m->freeListArray` read directly
 * instead, the loop optimizer hoists `m + 0x810` above the grid clear,
 * which ties up r1 there and pushes the cached count out of r3. */
struct pool_init_link;

struct pool_init_node {
    void *unk_00;
    void *unk_04;
    struct pool_init_link *link;  // 0x08
    void *unk_0C;
    u8 unk_10;
};

struct pool_init_link {
    struct pool_init_node *node;
    struct pool_init_link *next;
};

struct pool_init {
    s32 activeCount;
    s32 capacity;
    void **slotArray;
    struct pool_init_node *nodeArray;
    void *gridHead[256];
    void *gridTail[256];
    struct pool_init_link *freeListArray;
    struct pool_init_link *freeListHead;
};

static inline void PoolResetFreeList(struct pool_init *m)
{
    s32 i;
    s32 n = m->capacity;
    struct pool_init_link **freeList = &m->freeListArray;
    struct pool_init_link **freeHead = &m->freeListHead;
    struct pool_init_link **fl;

    for (i = 0; i < 256; i++) {
        m->gridHead[i] = NULL;
        m->gridTail[i] = NULL;
    }
    i = 0;
    if (i < n) {
        fl = freeList;
        do {
            struct pool_init_link *link;
            struct pool_init_link *arr;

            (*fl)[i].node = &m->nodeArray[i];
            m->nodeArray[i].unk_00 = NULL;
            m->nodeArray[i].unk_04 = NULL;
            m->nodeArray[i].unk_0C = NULL;
            m->nodeArray[i].unk_10 = 0;
            m->nodeArray[i].link = link = &(arr = *fl)[i];
            if (i == m->capacity - 1)
                link->next = NULL;
            else
                link->next = &arr[i + 1];
            i++;
        } while (i < m->capacity);
    }
    *freeHead = *freeList;
}

struct pool_init *sub_8008F20(struct pool_init *m, s32 count)
{
    m->activeCount = 0;
    m->capacity = count;
    m->slotArray = sub_8026EC0(count * 4);
    m->nodeArray = sub_8026EC0(m->capacity * sizeof(struct pool_init_node));
    {
        struct pool_init_link **p = &m->freeListArray;
        *p = sub_8026EC0(m->capacity * sizeof(struct pool_init_link));
    }
    {
        s32 j = m->capacity;
        if (j > 0) {
            void *zero = NULL;
            void **p = m->slotArray;
            do {
                *p = zero;
                p++;
                j--;
            } while (j != 0);
        }
    }
    PoolResetFreeList(m);
    return m;
}
asm(".align 2, 0");

/* Same pool-manager struct sub_8008F20 initializes and actor_part12.c
 * operates on - see that file for the full field writeup. Also used by
 * the now-matched `sub_8009150` (`actor_part11g.c`). */
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

/* sub_8009914 is reconstructed (semantics fully understood, and now
 * matched) as a NAKED transcription in its own translation unit,
 * `src/graphics/actor_part11i.c` - not appended here since its real
 * ROM address, 0x08009914, doesn't sit adjacent to this file's own
 * functions (it comes right after `sub_8009868`, `actor_part11d.c`,
 * and right before `sub_80099F0`/`actor_part12.c`), per
 * docs/workflow.md step 4's "needs its own new .c file" case - the
 * same reason `sub_80096C0` above got its own file
 * (`actor_part11e.c`), `sub_8009528` got `actor_part11f.c`, the
 * now-matched `sub_8009150` got `actor_part11g.c`, and the now-matched
 * `sub_800944C` got `actor_part11h.c`. */

/* sub_800944C is now matched as real C in its own translation unit,
 * `src/graphics/actor_part11h.c`; sub_8009528 is a NAKED transcription
 * in `src/graphics/actor_part11f.c`; sub_80096C0 likewise in
 * `src/graphics/actor_part11e.c` - none appended here since their real
 * ROM addresses don't sit adjacent to this file's own functions (they
 * come right after `sub_8009150` above and right before `sub_8009868`,
 * `actor_part11d.c`), per docs/workflow.md step 4's "needs its own new
 * .c file" case. */
asm(".align 2, 0");
