#include "core.h"
#include "actor.h"
#include "actor_self.h"
#include <agb_syscall.h>
#include "crates.h"
#include "objects.h"
#include "memory.h"
#include "box_part.h"
#include "globals.h"

extern void *_call_via_r1(void *arg0, void *fn);
extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);

/* What the managers' lists hold: level objects with a gcc 2.x method
 * table at +0x18 (the same prefix as gobj_1a794.h's `struct gobj`, whose
 * header can't be included here - its externs clash with this file's
 * definitions). */
struct listed_obj_vtable {
    u8 unk_00[0x20];
    struct actor_method m20; // 0x20 - per-frame update
};

struct listed_obj {
    u8 unk_00[0x18];
    struct listed_obj_vtable *vtable; // 0x18
};

/* Calls the `m20` virtual method (via the `_call_via_r1` call thunk) of
 * every object in `manager->visible` (bounded by `visibleCount`). */
void DrawPartList(struct part_list *manager)
{
    s32 i;

    for (i = 0; i < manager->visibleCount; i++) {
        struct listed_obj *part = (struct listed_obj *)manager->visible[i];
        struct listed_obj_vtable *tbl = part->vtable;
        s16 offset = tbl->m20.thisOffset;
        void *addr = (u8 *)part + offset;
        void *fn = tbl->m20.fn;

        _call_via_r1(addr, fn);
    }
}

/* Searches `manager->items` (bounded by `capacity`) for an entry
 * equal to `target`; if found, compacts the array by shifting every
 * following entry down by one slot via the BIOS `CpuSet` wrapper
 * `CpuSet`, decrements `count`, and clears the now-unused
 * trailing slot. Same removal logic as `RemovePartListAt` below, but
 * locates the index by value instead of taking it directly as an
 * argument. */
void RemoveFromPartList(struct part_list *manager, void *target)
{
    s32 i = 0;
    s32 count = manager->capacity;
    void **base;

    if (i >= count) {
        goto done;
    }
    {
        void **p0 = (void **)manager->items;
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
        s32 control = (manager->count - i) & 0x1FFFFF;
        s32 newCount;

        control |= 0x4000000;
        CpuSet(src, dst, control);

        newCount = manager->count - 1;
        manager->count = newCount;
        manager->items[newCount] = 0;
    }
done:
    return;
}

/* Removes the entry at `index` from `manager->items`, compacting via
 * `CpuSet` the same way `RemoveFromPartList` does after its own
 * search. */
void RemovePartListAt(struct part_list *manager, s32 index)
{
    if (index < manager->capacity) {
        s32 off = index * 4;
        s32 srcOff = off + 4;
        u8 *base;
        void *src, *dst;
        s32 control;
        s32 newCount;

        base = (u8 *)manager->items;
        src = base + srcOff;
        dst = base + off;
        control = (manager->count - index) & 0x1FFFFF;
        control |= 0x4000000;
        CpuSet(src, dst, control);

        newCount = manager->count - 1;
        manager->count = newCount;
        manager->items[newCount] = 0;
    }
}

/* Appends `value` to `manager->items` if there's room (`count` <
 * `capacity`). */
void AddToPartList(struct part_list *manager, void *value)
{
    s32 count = manager->count;

    if (count < manager->capacity) {
        manager->items[count] = value;
        manager->count = count + 1;
    }
}

/* Tears down a manager: frees both of its arrays (`visible` and
 * `items`, each via `OperatorDeleteArray` if non-`NULL`), and, if bit 0 of
 * `flags` is set, frees the manager struct itself via
 * `OperatorDelete`. */
void DestroyPartList(struct part_list *manager, s32 flags)
{
    if (manager->visible != 0) {
        OperatorDeleteArray(manager->visible);
    }
    if (manager->items != 0) {
        OperatorDeleteArray(manager->items);
    }
    if (flags & 1) {
        OperatorDelete(manager);
    }
}

/* Initializes a manager: sets `count`/`visibleCount` to 0, `capacity` to
 * `count`, allocates two `count`-word arrays via `OperatorNewArray` for
 * `items`/`visible`, and zero-fills `items`. Returns `manager`. */
struct part_list *InitPartList(struct part_list *manager, s32 count)
{
    s32 i;
    void **arr;
    s32 allocSize;

    manager->count = 0;
    manager->visibleCount = 0;
    manager->capacity = count;

    allocSize = count * 4;
    manager->items = OperatorNewArray(allocSize);
    manager->visible = OperatorNewArray(allocSize);

    i = manager->capacity;
    if (i > 0) {
        void *zero = 0;
        arr = (void **)manager->items;
        do {
            *arr = zero;
            arr++;
            i--;
        } while (i != 0);
    }

    return manager;
}

/* InitCrateList initializes a fixed-slot object pool "manager" struct:
 *  +0x0: s32 activeCount (0)
 *  +0x4: s32 capacity (= count)
 *  +0x8: void **slotArray - `count` pointers, allocated via
 *        OperatorNewArray(count*4), zero-filled
 *  +0xc: u8 *nodeArray - `count` 0x14-byte nodes, allocated via
 *        OperatorNewArray(count*0x14)
 *  +0x10..0x40F: a 256-word (0x400-byte) table, zeroed
 *  +0x410..0x80F: a second 256-word (0x400-byte) table, zeroed
 *  +0x810: void *freeListArray - `count` 8-byte {node, next} pairs,
 *          allocated via OperatorNewArray(count*8)
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
 * tail is the `PoolResetFreeList` inline (ResetCrateList ends with the same
 * code). Two source details carry the register assignment the old
 * notes blamed on the allocator: the grid clear is a plain indexed
 * `for` loop (gcc reverses it into the ROM's `i = 255 .. 0` countdown
 * with two post-increment pointers), and the free-list loop reads the
 * wrapper array through a `fl = freeList` copy taken inside the
 * `if (i < n)` guard, right before the `do` - that is the ROM's
 * `mov ip, sb` at the loop head. With `m->freeListArray` read directly
 * instead, the loop optimizer hoists `m + 0x810` above the grid clear,
 * which ties up r1 there and pushes the cached count out of r3. */
/* The zeroing stores go through `struct pool_init_node` (crates.h), the
 * untyped view of the node. */
static inline void PoolResetFreeList(struct pool_manager *m)
{
    s32 i;
    s32 n = m->capacity;
    struct pool_link **freeList = &m->freeListArray;
    struct pool_link **freeHead = &m->freeListHead;
    struct pool_link **fl;

    for (i = 0; i < 256; i++) {
        m->gridHead[i] = NULL;
        m->gridTail[i] = NULL;
    }
    i = 0;
    if (i < n) {
        fl = freeList;
        do {
            struct pool_link *link;
            struct pool_link *arr;

            (*fl)[i].node = &m->nodeArray[i];
            ((struct pool_init_node *)m->nodeArray)[i].data = NULL;
            ((struct pool_init_node *)m->nodeArray)[i].next = NULL;
            ((struct pool_init_node *)m->nodeArray)[i].link = NULL;
            ((struct pool_init_node *)m->nodeArray)[i].mark = 0;
            ((struct pool_init_node *)m->nodeArray)[i].wrap = link = &(arr = *fl)[i];
            if (i == m->capacity - 1)
                link->next = NULL;
            else
                link->next = &arr[i + 1];
            i++;
        } while (i < m->capacity);
    }
    *freeHead = *freeList;
}

struct pool_manager *InitCrateList(struct pool_manager *m, s32 count)
{
    m->activeCount = 0;
    m->capacity = count;
    m->slotArray = OperatorNewArray(count * 4);
    m->nodeArray = OperatorNewArray(m->capacity * sizeof(struct pool_node));
    {
        struct pool_link **p = &m->freeListArray;
        *p = OperatorNewArray(m->capacity * sizeof(struct pool_link));
    }
    {
        s32 j = m->capacity;
        if (j > 0) {
            void *zero = NULL;
            struct box_part **p = m->slotArray;
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

/* Same pool-manager struct InitCrateList initializes and crate_list.c
 * operates on - see that file for the full field writeup. Also used by
 * the now-matched `LinkCrateToActiveBucket` (`crate_grid_link.c`). */
/* ResetCrateList is reconstructed (semantics fully understood, and now
 * matched) as a NAKED transcription in its own translation unit,
 * `src/crates/crate_list_reset.c` - not appended here since its real
 * ROM address, 0x08009914, doesn't sit adjacent to this file's own
 * functions (it comes right after `CollidePlayerWithCrates`, `crate_player_collide.c`,
 * and right before `CollideCrateGridPartWithObject`/`crate_list.c`), per
 * docs/workflow.md step 4's "needs its own new .c file" case - the
 * same reason `CollideCrateGridPartWithPlayer` above got its own file
 * (`crate_grid_collide.c`), `CollideCrateGrid` got `crate_grid_collide.c`, the
 * now-matched `LinkCrateToActiveBucket` got `crate_grid_link.c`, and the now-matched
 * `DrawCrateList` got `crate_list_draw.c`. */

/* DrawCrateList is now matched as real C in its own translation unit,
 * `src/crates/crate_list_draw.c`; CollideCrateGrid is a NAKED transcription
 * in `src/crates/crate_grid_collide.c`; CollideCrateGridPartWithPlayer likewise in
 * `src/crates/crate_grid_collide.c` - none appended here since their real
 * ROM addresses don't sit adjacent to this file's own functions (they
 * come right after `LinkCrateToActiveBucket` above and right before `CollidePlayerWithCrates`,
 * `crate_player_collide.c`), per docs/workflow.md step 4's "needs its own new
 * .c file" case. */
asm(".align 2, 0");
