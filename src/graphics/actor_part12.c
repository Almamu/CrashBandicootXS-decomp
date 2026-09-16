#include "core.h"
#include "actor.h"

extern void sub_8009008(void *manager, void *item);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void *sub_8009AF0(void *manager, void *data, s32 bucket, s32 extra);
extern void sub_8009B3C(void *manager, void *obj);
extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *manager);

#if NON_MATCHING
struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

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
 * NOT YET BYTE-MATCHING: identical structural gap as `sub_8008D80` -
 * this compiler has no way to leave `boxH` untouched in its own
 * incoming stack slot while still building a 4-word AABB pointer that
 * includes it, the ABI stack-layout trick the ROM's own tighter local
 * frame relies on. Parked with the version that writes it explicitly,
 * matching `sub_8008D80`'s own parking rationale - see
 * `docs/matching.md`, "Parked, not matched: `sub_80099F0`". */
void sub_80099F0(void *manager, s32 boxX, s32 boxY, s32 boxW, s32 boxH, void *partArg, void *otherViewportArg)
{
    struct actor *part = partArg;
    struct actor *otherViewport = otherViewportArg;
    struct aabb box;
    s32 result;

    box.field_0 = boxX;
    box.field_4 = boxY;
    box.field_8 = boxW;
    box.field_c = boxH;

    result = sub_8009FF4(part, &box);
    if (result == 0) {
        return;
    }
    {
        u8 *rec = (u8 *)part->table + 0x68;
        s16 offset = *(s16 *)rec;
        void *addr = (u8 *)part + offset;
        u8 someByte = otherViewport->field_0A;
        register void *deadRead asm("r4") = *(void *volatile *)(rec + 4);
        (void)deadRead;

        sub_803AD88(addr, 1, someByte, 0);
    }
    otherViewport->flags |= 8;
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

/* Searches `manager`'s active-object array (`manager+8` base,
 * `manager+4` capacity bound for the search) for `target`; if found,
 * removes it from the collision grid and returns its pool node to the
 * free list via `sub_8009008`, then compacts the array (bounded this
 * time by the live count at `manager+0`) via the same CpuSet-based
 * shift used throughout this cluster, decrementing `manager+0`. */
void sub_8009A30(void *manager, void *target)
{
    s32 i = 0;
    s32 searchCount = *(s32 *)((u8 *)manager + 4);
    void **base;

    if (i >= searchCount) {
        goto done;
    }
    {
        void **p0 = *(void ***)((u8 *)manager + 8);
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

    if (i < *(s32 *)((u8 *)manager + 4)) {
        s32 off = i * 4;
        void *item = base[i];

        sub_8009008(manager, item);

        {
            s32 srcOff = off + 4;
            void **base2 = *(void ***)((u8 *)manager + 8);
            void *src = (u8 *)base2 + srcOff;
            void *dst = (u8 *)base2 + off;
            s32 control = (*(s32 *)manager - i) & 0x1FFFFF;
            s32 cnt;
            void **base3;

            control |= 0x4000000;
            sub_803A94C(src, dst, control);

            cnt = *(s32 *)manager;
            base3 = *(void ***)((u8 *)manager + 8);
            *(void **)((u8 *)base3 + cnt * 4 - 4) = 0;
            cnt -= 1;
            *(s32 *)manager = cnt;
        }
    }
done:
    return;
}

/* Removes the entry at `index` from `manager`'s active-object array
 * the same way `sub_8009A30` does after its own search - unlinks it
 * from the grid via `sub_8009008`, then compacts via `sub_803A94C`. */
void sub_8009AA0(void *manager, s32 index)
{
    if (index < *(s32 *)((u8 *)manager + 4)) {
        void **base = *(void ***)((u8 *)manager + 8);
        s32 off = index * 4;
        void *item = base[index];

        sub_8009008(manager, item);

        {
            s32 srcOff = off + 4;
            void **base2 = *(void ***)((u8 *)manager + 8);
            void *src = (u8 *)base2 + srcOff;
            void *dst = (u8 *)base2 + off;
            s32 control = (*(s32 *)manager - index) & 0x1FFFFF;
            s32 cnt;
            void **base3;

            control |= 0x4000000;
            sub_803A94C(src, dst, control);

            cnt = *(s32 *)manager;
            base3 = *(void ***)((u8 *)manager + 8);
            *(void **)((u8 *)base3 + cnt * 4 - 4) = 0;
            cnt -= 1;
            *(s32 *)manager = cnt;
        }
    }
}

/* Pops a node off `manager`'s free list (`manager+0x814` head,
 * unlinked via the wrapper entry's own `+4` "next" field), reuses it
 * to wrap `data`/`extra`, and inserts it into the spatial hash grid
 * bucket `bucket`: `manager+0x10` holds each bucket's head pointer
 * (set only the first time a bucket goes from empty), `manager+0x410`
 * holds each bucket's tail pointer (always updated, chaining the
 * previous tail's `+4` "next" field to the new node). Returns the
 * node. */
void *sub_8009AF0(void *manager, void *data, s32 bucket, s32 extra)
{
    void **headField = (void **)((u8 *)manager + 0x814);
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
        u8 *gridABase = (u8 *)manager + 0x10;
        void **gridASlot = (void **)(gridABase + off);
        if (*gridASlot == 0) {
            *gridASlot = node;
        }
        {
            u8 *gridBBase = (u8 *)manager + 0x410;
            void **gridBSlot = (void **)(gridBBase + off);
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
void sub_8009B3C(void *manager, void *obj)
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

/* Appends `obj` to `manager`'s active-object array (`manager+8` base,
 * `manager+0` count) if there's room below `manager+4`'s capacity,
 * inserting it into the collision grid via `sub_8009B3C` first. */
void sub_8009B70(void *manager, void *obj)
{
    if (*(s32 *)manager < *(s32 *)((u8 *)manager + 4)) {
        s32 idx;

        sub_8009B3C(manager, obj);

        idx = *(s32 *)manager;
        {
            void **base = *(void ***)((u8 *)manager + 8);
            base[idx] = obj;
        }
        *(s32 *)manager = idx + 1;
    }
}

/* Tears down a pool manager: frees the free-list array
 * (`manager+0x810`), the node array (`manager+0xc`), and the slot
 * array (`manager+8`), each via `sub_8026EB4` if non-`NULL`; resets
 * the capacity (`manager+4`) to 0; and, if a flags bit is set, frees
 * the manager itself via `sub_8026ED0`. */
void sub_8009B9C(void *manager, s32 flags)
{
    if (*(void **)((u8 *)manager + 0x810) != 0) {
        sub_8026EB4(*(void **)((u8 *)manager + 0x810));
    }
    if (*(void **)((u8 *)manager + 0xc) != 0) {
        sub_8026EB4(*(void **)((u8 *)manager + 0xc));
    }
    if (*(void **)((u8 *)manager + 8) != 0) {
        sub_8026EB4(*(void **)((u8 *)manager + 8));
    }
    *(s32 *)((u8 *)manager + 4) = 0;
    if (flags & 1) {
        sub_8026ED0(manager);
    }
}
asm(".align 2, 0");
