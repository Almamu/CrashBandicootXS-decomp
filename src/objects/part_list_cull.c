#include "core.h"
#include "match.h"
#include "actor.h"
#include "vtable.h"
#include "objects.h"
#include "globals.h"

/* This file's `manager` is the same `dual_array_manager` struct
 * defined and used in `part_list.c` (capacity/count1/count2/
 * array1/array2), but is deliberately kept as raw offset casts here
 * instead of named struct field access: every function below pins
 * specific registers (`MATCH_HOLD_REG`) to reproduce exact
 * ROM instruction ordering, and this compiler's register allocation
 * for a struct-field access is sensitive to surrounding context in
 * ways that have already caused real regressions this session (see
 * docs/matching.md's "second tractable pocket" writeup and
 * docs/workflow.md step 7's carve-out for when to keep raw pointer
 * arithmetic). Converting these would need the same rebuild-verify
 * rigor as any other change here - not attempted opportunistically. */

extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);
extern void *_call_via_r1(void *arg0, void *fn);

/* Same "extended screen box" filter shape as `UpdatePartList`'s own
 * `boxB` pass above (the plain 240x160 GBA screen region, in Q8, at
 * the `gLevelLayers` sub-object's own position) - iterates
 * `manager`'s array (`manager+0xc` base, `manager+4` count),
 * filtering each `part` whose `table+0x30/0x34`-driven trampoline
 * passes into a second output array (`manager+0x10` base,
 * `manager+8` count). */
void CullPartList(void *manager)
{
    s32 i;
    s32 box[4];
    MATCH_HOLD_REG(void *, P, r0) = gLevelLayers;
    MATCH_HOLD_REG(void *, subObj, r0);
    s32 v0, v1;
    s32 v2, v3;

    subObj = *(void **)((u8 *)P + 0x10);
    v0 = *(s32 *)subObj << 8;
    v1 = *(s32 *)((u8 *)subObj + 4) << 8;
    box[0] = v0;
    box[1] = v1;
    v2 = 0xf0 << 8;
    v3 = 0xa0 << 8;
    box[2] = v2;
    box[3] = v3;

    *(s32 *)((u8 *)manager + 8) = 0;

    for (i = 0; i < *(s32 *)((u8 *)manager + 4); i++) {
        MATCH_HOLD_REG(void **, arrBase, r1) = *(void ***)((u8 *)manager + 0xc);
        MATCH_HOLD_REG(s32, idx, r0) = i * 4;
        MATCH_HOLD_REG(void **, slot, r0);
        void *part;

        slot = (void **)((u8 *)idx + (s32)arrBase);
        part = *slot;

        {
            MATCH_HOLD_REG(struct vtable_slot *, tbl, r1) = ((struct actor *)part)->table;
            MATCH_HOLD_REG(s32, offset, r0) = tbl[6].delta;
            MATCH_HOLD_REG(void *, addr, r0);
            MATCH_HOLD_REG(void *, fn, r2);

            addr = (u8 *)part + offset;
            fn = tbl[6].fn;

            if ((u8)_call_via_r2(addr, box, fn)) {
                s32 outCount = *(s32 *)((u8 *)manager + 8);
                void **outArr = *(void ***)((u8 *)manager + 0x10);

                outArr[outCount] = part;
                *(s32 *)((u8 *)manager + 8) = outCount + 1;
            }
        }
    }
}

/* Fires a `part->table+0x50/0x54`-driven trampoline (constant arg 3)
 * via `_call_via_r2` for every entry in `manager`'s array
 * (`manager+0xc` base, `manager+4` count) that isn't already `NULL`,
 * then clears every slot and resets both the count (`manager+4`) and
 * the second array's count (`manager+8`) to 0 - a full teardown of
 * both this manager's arrays. */
void ClearPartList(void *manager)
{
    s32 i;

    for (i = 0; i < *(s32 *)((u8 *)manager + 4); i++) {
        s32 idx = i;
        void **arrBase = *(void ***)((u8 *)manager + 0xc);
        void *part = arrBase[idx];

        if (part != 0) {
            MATCH_HOLD_REG(u8 *, rec, r1) = *(u8 **)((u8 *)part + 0x18) + 0x50;
            MATCH_HOLD_REG(s32, offset, r0) = *(s16 *)rec;
            MATCH_HOLD_REG(void *, addr, r0);
            MATCH_HOLD_REG(void *, fn, r2);

            addr = (u8 *)part + offset;
            fn = *(void **)(rec + 4);
            _call_via_r2(addr, (void *)3, fn);
        }
        arrBase = *(void ***)((u8 *)manager + 0xc);
        arrBase[idx] = 0;
    }
    *(s32 *)((u8 *)manager + 4) = 0;
    *(s32 *)((u8 *)manager + 8) = 0;
}

/* Iterates `manager`'s array (`manager+0x10` base, `manager+8`
 * count): for each `part`, fires a `table+0x48/0x4c`-driven
 * trampoline via `_call_via_r1` and skips unless the result equals
 * `arg1` (a caller-supplied selector); skips unless `part->flags`
 * bit 2 is set; then fires a *second*, unconditional `table+8/0xc`
 * trampoline (return value discarded). */
void CollidePartsOfClass(void *manager, s32 arg1)
{
    s32 i;

    for (i = 0; i < *(s32 *)((u8 *)manager + 8); i++) {
        void **arr = *(void ***)((u8 *)manager + 0x10);
        void *part = arr[i];
        MATCH_HOLD_REG(u8 *, rec, r1) = *(u8 **)((u8 *)part + 0x18) + 0x48;
        MATCH_HOLD_REG(s32, offset, r0) = *(s16 *)rec;
        MATCH_HOLD_REG(void *, addr, r0);
        MATCH_HOLD_REG(void *, fn, r1);
        s32 result;

        addr = (u8 *)part + offset;
        fn = *(void **)(rec + 4);
        result = (s32)_call_via_r1(addr, fn);

        if (result != arg1) {
            continue;
        }
        {
            MATCH_HOLD_REG(u8, byte, r1) = *((u8 *)part + 0xc);
            MATCH_HOLD_REG(s32, shifted, r0) = byte >> 2;
            MATCH_HOLD_REG(s32, mask, r1) = 1;
            MATCH_HOLD_REG(s32, test, r0);

            test = shifted & mask;
            if (!test) {
                continue;
            }
        }
        {
            MATCH_HOLD_REG(u8 *, tbl2, r1) = *(u8 **)((u8 *)part + 0x18);
            MATCH_HOLD_REG(s32, offset2, r0) = *(s16 *)(tbl2 + 8);
            MATCH_HOLD_REG(void *, addr2, r0);
            MATCH_HOLD_REG(void *, fn2, r1);

            addr2 = (u8 *)part + offset2;
            fn2 = *(void **)(tbl2 + 0xc);
            _call_via_r1(addr2, fn2);
        }
    }
}
asm(".align 2, 0");
