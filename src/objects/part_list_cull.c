#include "core.h"
#include "match.h"
#include "actor.h"
#include "vtable.h"
#include "objects.h"
#include "globals.h"
#include "level.h"
#include "bg_scroll_layer.h"
#include "box_part.h"

extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);
extern void *_call_via_r1(void *arg0, void *fn);

/* Same "extended screen box" filter shape as `UpdatePartList`'s own
 * `boxB` pass above (the plain 240x160 GBA screen region, in Q8, at
 * `gLevelLayers->layer0`'s scroll position) - iterates `manager`'s
 * `items`, copying each `part` whose method table slot 6
 * (SpriteObjOverlapsRect) passes into `visible`. */
void CullPartList(struct part_list *manager)
{
    s32 i;
    s32 box[4];
    MATCH_HOLD_REG(struct level_layers *, P, r0) = gLevelLayers;
    MATCH_HOLD_REG(struct bg_scroll_layer *, subObj, r0);
    s32 v0, v1;
    s32 v2, v3;

    subObj = P->layer0;
    v0 = subObj->x << 8;
    v1 = subObj->y << 8;
    box[0] = v0;
    box[1] = v1;
    v2 = 0xf0 << 8;
    v3 = 0xa0 << 8;
    box[2] = v2;
    box[3] = v3;

    manager->visibleCount = 0;

    for (i = 0; i < manager->count; i++) {
        MATCH_HOLD_REG(struct box_part **, arrBase, r1) = manager->items;
        MATCH_HOLD_REG(s32, idx, r0) = i * 4;
        MATCH_HOLD_REG(struct box_part **, slot, r0);
        struct box_part *part;

        slot = (struct box_part **)((u8 *)idx + (s32)arrBase);
        part = *slot;

        {
            MATCH_HOLD_REG(struct vtable_slot *, tbl, r1) = ((struct actor *)part)->table;
            MATCH_HOLD_REG(s32, offset, r0) = tbl[6].delta;
            MATCH_HOLD_REG(void *, addr, r0);
            MATCH_HOLD_REG(void *, fn, r2);

            addr = (u8 *)part + offset;
            fn = tbl[6].fn;

            if ((u8)_call_via_r2(addr, box, fn)) {
                s32 outCount = manager->visibleCount;
                struct box_part **outArr = manager->visible;

                outArr[outCount] = part;
                manager->visibleCount = outCount + 1;
            }
        }
    }
}

/* Calls the destructor (method table +0x50, with 3) of every entry of
 * `manager`'s `items` that isn't already `NULL`, then clears every slot
 * and resets `count` and `visibleCount` to 0 - a full teardown of both
 * of the list's arrays. */
void ClearPartList(struct part_list *manager)
{
    s32 i;

    for (i = 0; i < manager->count; i++) {
        s32 idx = i;
        struct box_part **arrBase = manager->items;
        struct box_part *part = arrBase[idx];

        if (part != 0) {
            MATCH_HOLD_REG(struct part_method *, rec, r1) = PART_METHOD(part, 0x50);
            MATCH_HOLD_REG(s32, offset, r0) = rec->thisOffset;
            MATCH_HOLD_REG(void *, addr, r0);
            MATCH_HOLD_REG(void *, fn, r2);

            addr = (u8 *)part + offset;
            fn = rec->fn;
            _call_via_r2(addr, (void *)3, fn);
        }
        arrBase = manager->items;
        arrBase[idx] = 0;
    }
    manager->count = 0;
    manager->visibleCount = 0;
}

/* Iterates `manager`'s `visible` parts: for each `part`, calls its
 * class-id method (method table +0x48) via `_call_via_r1` and skips
 * unless the result equals `arg1` (a caller-supplied selector); skips
 * unless `part->flags` bit 2 is set; then calls its collide method
 * (method table +0x08; return value discarded). */
void CollidePartsOfClass(struct part_list *manager, s32 arg1)
{
    s32 i;

    for (i = 0; i < manager->visibleCount; i++) {
        struct box_part **arr = manager->visible;
        struct box_part *part = arr[i];
        MATCH_HOLD_REG(struct part_method *, rec, r1) = PART_METHOD(part, 0x48);
        MATCH_HOLD_REG(s32, offset, r0) = rec->thisOffset;
        MATCH_HOLD_REG(void *, addr, r0);
        MATCH_HOLD_REG(void *, fn, r1);
        s32 result;

        addr = (u8 *)part + offset;
        fn = rec->fn;
        result = (s32)_call_via_r1(addr, fn);

        if (result != arg1) {
            continue;
        }
        {
            MATCH_HOLD_REG(u8, byte, r1) = part->flags;
            MATCH_HOLD_REG(s32, shifted, r0) = byte >> 2;
            MATCH_HOLD_REG(s32, mask, r1) = 1;
            MATCH_HOLD_REG(s32, test, r0);

            test = shifted & mask;
            if (!test) {
                continue;
            }
        }
        {
            /* Indexed from the table base: PART_METHOD(part, 0x08) adds
             * the 8 to the table register first. */
            MATCH_HOLD_REG(struct part_method *, tbl2, r1) = (struct part_method *)part->vtable;
            MATCH_HOLD_REG(s32, offset2, r0) = tbl2[1].thisOffset;
            MATCH_HOLD_REG(void *, addr2, r0);
            MATCH_HOLD_REG(void *, fn2, r1);

            addr2 = (u8 *)part + offset2;
            fn2 = tbl2[1].fn;
            _call_via_r1(addr2, fn2);
        }
    }
}
asm(".align 2, 0");
