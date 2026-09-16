#include "core.h"
#include "actor.h"

extern void *sub_803AD7C(void *arg0, void *fn);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *manager);
extern void *sub_8026EC0(u32 size);

/* Fires a `part->table+0x20/0x24`-driven trampoline via `sub_803AD7C`
 * for every entry in `manager`'s array (`manager+0x10` base,
 * `manager+8` count). */
void sub_8008DC0(void *manager)
{
    s32 i;

    for (i = 0; i < *(s32 *)((u8 *)manager + 8); i++) {
        void **arr = *(void ***)((u8 *)manager + 0x10);
        void *part = arr[i];
        u8 *tbl = *(u8 **)((u8 *)part + 0x18);
        s16 offset = *(s16 *)(tbl + 0x20);
        void *addr = (u8 *)part + offset;
        void *fn = *(void **)(tbl + 0x24);

        sub_803AD7C(addr, fn);
    }
}

/* Searches `manager`'s array (`manager+0xc` base, `manager+0` count)
 * for an entry equal to `target`; if found, compacts the array by
 * shifting every following entry down by one slot via the BIOS
 * `CpuSet` wrapper `sub_803A94C`, decrements `manager+4`'s count, and
 * clears the now-unused trailing slot. Same removal logic as
 * `sub_8008E50` below, but locates the index by value instead of
 * taking it directly as an argument. */
void sub_8008DEC(void *manager, void *target)
{
    s32 i = 0;
    s32 count = *(s32 *)manager;
    void **base;

    if (i >= count) {
        goto done;
    }
    {
        void **p0 = *(void ***)((u8 *)manager + 0xc);
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

    if (i < *(s32 *)manager) {
        s32 off = i * 4;
        s32 srcOff = off + 4;
        void *src = (u8 *)base + srcOff;
        void *dst = (u8 *)base + off;
        s32 control = (*(s32 *)((u8 *)manager + 4) - i) & 0x1FFFFF;
        s32 newCount;
        void **base2;

        control |= 0x4000000;
        sub_803A94C(src, dst, control);

        newCount = *(s32 *)((u8 *)manager + 4) - 1;
        *(s32 *)((u8 *)manager + 4) = newCount;
        base2 = *(void ***)((u8 *)manager + 0xc);
        base2[newCount] = 0;
    }
done:
    return;
}

/* Removes the entry at `index` from `manager`'s array (`manager+0xc`
 * base, `manager+0` count), compacting via `sub_803A94C` the same way
 * `sub_8008DEC` does after its own search. */
void sub_8008E50(void *manager, s32 index)
{
    if (index < *(s32 *)manager) {
        s32 off = index * 4;
        s32 srcOff = off + 4;
        u8 *base;
        void *src, *dst;
        s32 control;
        s32 newCount;
        void **base2;

        base = *(u8 **)((u8 *)manager + 0xc);
        src = base + srcOff;
        dst = base + off;
        control = (*(s32 *)((u8 *)manager + 4) - index) & 0x1FFFFF;
        control |= 0x4000000;
        sub_803A94C(src, dst, control);

        newCount = *(s32 *)((u8 *)manager + 4) - 1;
        *(s32 *)((u8 *)manager + 4) = newCount;
        base2 = *(void ***)((u8 *)manager + 0xc);
        base2[newCount] = 0;
    }
}

/* Appends `value` to `manager`'s array (`manager+0xc` base,
 * `manager+4` count) if there's room (`manager+4` count <
 * `manager+0` capacity). */
void sub_8008E94(void *manager, void *value)
{
    s32 count = *(s32 *)((u8 *)manager + 4);

    if (count < *(s32 *)manager) {
        void **arr = *(void ***)((u8 *)manager + 0xc);
        arr[count] = value;
        *(s32 *)((u8 *)manager + 4) = count + 1;
    }
}

/* Tears down a manager: frees both of its arrays (`manager+0x10` and
 * `manager+0xc`, each via `sub_8026EB4` if non-`NULL`), and, if bit 0
 * of `flags` is set, frees the manager struct itself via
 * `sub_8026ED0`. */
void sub_8008EB4(void *manager, s32 flags)
{
    if (*(void **)((u8 *)manager + 0x10) != 0) {
        sub_8026EB4(*(void **)((u8 *)manager + 0x10));
    }
    if (*(void **)((u8 *)manager + 0xc) != 0) {
        sub_8026EB4(*(void **)((u8 *)manager + 0xc));
    }
    if (flags & 1) {
        sub_8026ED0(manager);
    }
}

/* Initializes a manager: sets its count (`manager+4`) and second
 * count (`manager+8`) to 0, capacity (`manager+0`) to `count`,
 * allocates two `count`-word arrays via `sub_8026EC0` for
 * `manager+0xc` and `manager+0x10`, and zero-fills the first array.
 * Returns `manager`. */
void *sub_8008EE4(void *manager, s32 count)
{
    s32 i;
    void **arr;
    s32 allocSize;

    *(s32 *)((u8 *)manager + 4) = 0;
    *(s32 *)((u8 *)manager + 8) = 0;
    *(s32 *)manager = count;

    allocSize = count * 4;
    *(void **)((u8 *)manager + 0xc) = sub_8026EC0(allocSize);
    *(void **)((u8 *)manager + 0x10) = sub_8026EC0(allocSize);

    i = *(s32 *)manager;
    if (i > 0) {
        void *zero = 0;
        arr = *(void ***)((u8 *)manager + 0xc);
        do {
            *arr = zero;
            arr++;
            i--;
        } while (i != 0);
    }

    return manager;
}
asm(".align 2, 0");
