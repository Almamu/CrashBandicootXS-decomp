#include "core.h"
#include "actor.h"
#include "box_part.h"

extern void sub_8009008(void *manager, void *item);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void *sub_803AD7C(void *arg0, void *fn);
extern void *gUnknown_03001308;

/* A per-frame spatial-hash-grid maintenance pass over `manager`'s
 * `struct pool_manager` (`actor_part12.c`), scoped to the 3-bucket
 * window `[baseIdx, baseIdx+2]` around `baseIdx` (the same
 * `max(gUnknown_03001308`'s sub-object's own `x >> 8`, `0)` bucket
 * index `sub_800944C`/`sub_8009528` compute), plus the special "large
 * object" bucket 255 in a second pass - not a full 0-255 sweep like
 * those two sibling functions.
 *
 * For each node's object (`part`) in the windowed buckets:
 *  - If `part->flags` bit 4 ("large object") is set and this node has
 *    no bucket-255 secondary link yet (`node+0xc == 0`), lazily creates
 *    one - an inline copy of `sub_8009150`'s own body (pop a node off
 *    the free list, wrap `part` in it, splice it into bucket 255's
 *    head/tail list, cross-link the two nodes via `+0xc`).
 *  - Otherwise, if `part->flags` bit 0 is set (a "pending removal"
 *    flag), removes `part` from `manager`'s active-object array the
 *    same way `sub_8009A30` does (linear search, `sub_8009008` to
 *    unlink the grid node(s), `sub_803A94C`-based compaction), then
 *    fires a `part->table+0x50/0x54`-driven trampoline via
 *    `sub_803AD80` with constant arg `3` - the exact same "destroy"
 *    trampoline `sub_8009914`'s teardown loop fires.
 *  - Otherwise, tests `part` against a computed box (the tracked
 *    sub-object's position, offset by fixed constants `-0x6400`/
 *    `-0x3C00` in Q8 and sized `0x1B8`x`0x118` in Q8 - an "extended"
 *    region wider than the plain 240x160 screen box `sub_800944C`/
 *    `sub_8009528` use, meaning/purpose not yet confirmed) via a
 *    `part->table+0x40/0x44`-driven trampoline; on a hit, fires a
 *    `part->table+0x18/0x1c`-driven trampoline and marks the node
 *    (`node+0x10 = 1`) so the bucket-255 second pass knows to skip a
 *    node already handled via its primary bucket (clearing the mark
 *    instead), mirroring `sub_800944C`'s own primary/secondary-pass
 *    marking convention (there via `node+0x11`).
 *
 * Written as NAKED asm, not plain C: the semantics above are fully
 * confirmed (every branch, field offset, and call argument traced
 * against the ROM disassembly), but this function juggles more live
 * cross-branch state across three high registers (`r8`/`sb`/`sl`, each
 * reused for a different purpose in each of the three inner-loop
 * branches) than any C-level reconstruction attempted here reproduced
 * exactly - the same category of many-register allocation gap already
 * documented for `sub_8008F20`/`sub_8009914` in `actor_part11.c`.
 * Parked as a direct transcription of the ROM's own confirmed-correct
 * instructions instead. The NON_MATCHING draft below is now 7
 * halfwords off (see the note on it). */
#if NON_MATCHING
struct pool_node {
    struct box_part *data;
    struct pool_node *next;
    void *wrap;
    struct pool_node *link;
    u8 mark;
    u8 mark2;
};

struct pool_entry {
    struct pool_node *node;
    struct pool_entry *next;
};

struct pool_manager {
    s32 activeCount;
    s32 capacity;
    struct box_part **slotArray;
    void *nodeArray;
    struct pool_node *gridHead[256];
    struct pool_node *gridTail[256];
    void *freeListArray;
    struct pool_entry *freeListHead;
};

struct track_obj {
    u8 unused_00[0x10];
    s32 *pos;
};

/* `sub_8009A30`'s body, inlined. */
static inline void pool_remove(struct pool_manager *manager, struct box_part *target)
{
    s32 i = 0;
    s32 searchCount = manager->capacity;
    struct box_part **base;

    /* Emits no code. It keeps gcse's copy of `capacity` from landing
     * right after the load; cse2 would otherwise swap the two and put
     * the loaded value in the pre/post tests instead of the loop test. */
    asm("");

    if (i >= searchCount) {
        goto done;
    }
    {
        struct box_part **p0 = manager->slotArray;
        struct box_part *val = *p0;
        base = p0;
        if (val != target) {
            struct box_part **p = base;
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
        struct box_part *item = base[i];

        sub_8009008(manager, item);

        {
            s32 srcOff = off + 4;
            struct box_part **base2 = manager->slotArray;
            void *src = (u8 *)base2 + srcOff;
            void *dst = (u8 *)base2 + off;
            s32 control = (manager->activeCount - i) & 0x1FFFFF;
            s32 cnt;
            struct box_part **base3;

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

static inline void part_destroy(struct box_part *part)
{
    if (part != NULL) {
        struct part_method *m = PART_METHOD(part, 0x50);

        sub_803AD80((u8 *)part + m->thisOffset, (void *)3, m->fn);
    }
}

/* The statement expressions give each callee its own copy of the
 * pointer (the ROM's sb/ip and sl/sb pairs). */
#define PART_COPY(p) ({ struct box_part *_t = (p); _t; })

static inline void pool_destroy(struct pool_manager *manager, struct box_part *obj)
{
    pool_remove(manager, PART_COPY(obj));
    part_destroy(PART_COPY(obj));
}

/* 7 halfwords off under old_agbcc (was 215); agbcc is further off.
 * What's left: in the first loop's inlined search the ROM puts the
 * `capacity` load in r2 and the `slotArray` copy in r3, and this build
 * swaps them. Also `gridHeadBase[i]` comes out as `adds r0, r0, r2`
 * where the ROM has `adds r0, r2, r0`. Loop-shape changes (brief item
 * 9), extra-reference nudges, padding asm and do/while(0) depth changes
 * did not fix the swap without breaking the second loop. */
void sub_80091D4(struct pool_manager *manager)
{
    s32 box[4];
    s32 *pos;
    s32 base;
    s32 i;
    s32 next;
    struct pool_node *node;
    struct pool_node **gridHeadBase;
    struct pool_node **gridHead255;
    s32 v2, v3, v0, v1;

    v2 = 0x1b800;
    v3 = 0x11800;
    box[2] = v2;
    box[3] = v3;
    pos = ((struct track_obj *)gUnknown_03001308)->pos;
    v0 = (pos[0] << 8) - 0x6400;
    v1 = (pos[1] << 8) - 0x3c00;
    box[0] = v0;
    box[1] = v1;
    base = pos[0];
    base >>= 8;
    if (base < 0)
        base = 0;

    i = base + 2;
    gridHeadBase = manager->gridHead;
    gridHead255 = &manager->gridHead[255];
    do {
        node = gridHeadBase[i];
        next = i - 1;
        while (node != NULL) {
            /* Reading `node->data` twice gives the ROM's load into r2
             * and the copy into r5. */
            s32 large = (node->data->flags >> 4) & 1;
            struct box_part *part = node->data;

            if (large && node->link == NULL) {
                struct pool_entry *entry = manager->freeListHead;
                struct pool_node *newNode = entry->node;

                manager->freeListHead = entry->next;
                entry->next = NULL;
                newNode->data = part;
                newNode->next = NULL;
                newNode->link = node;
                newNode->mark = 0;
                newNode->mark2 = 0;
                if (*gridHead255 == NULL)
                    *gridHead255 = newNode;
                if (manager->gridTail[255] != NULL)
                    manager->gridTail[255]->next = newNode;
                manager->gridTail[255] = newNode;
                node->link = newNode;
            } else if (part->flags & 1) {
                pool_destroy(manager, PART_COPY(part));
            } else {
                struct part_method *m = PART_METHOD(part, 0x40);

                if ((u8)sub_803AD80((u8 *)part + m->thisOffset, box, m->fn)) {
                    struct box_part *p2 = node->data;
                    struct part_method *m2 = PART_METHOD(p2, 0x18);

                    sub_803AD7C((u8 *)p2 + m2->thisOffset, m2->fn);
                    node->mark = 1;
                }
            }
            node = node->next;
        }
        i = next;
    } while (i >= base);

    for (node = *gridHead255; node != NULL; node = node->next) {
        struct box_part *part = node->data;

        if (part->flags & 1) {
            pool_destroy(manager, PART_COPY(part));
        } else if (node->link->mark == 0) {
            struct part_method *m = PART_METHOD(part, 0x18);

            sub_803AD7C((u8 *)part + m->thisOffset, m->fn);
        } else {
            node->link->mark = 0;
        }
    }
}
#else
NAKED void sub_80091D4(void *manager)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x1c\n\t"
        "add r7, r0, #0\n\t"
        "mov r0, #0xdc\n\t"
        "lsl r0, r0, #9\n\t"
        "mov r1, #0x8c\n\t"
        "lsl r1, r1, #9\n\t"
        "str r0, [sp, #8]\n\t"
        "str r1, [sp, #0xc]\n\t"
        "ldr r0, 6f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r2, [r0, #0x10]\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r1, r1, #8\n\t"
        "ldr r0, 7f\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [r2, #4]\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r3, 8f\n\t"
        "add r0, r0, r3\n\t"
        "str r1, [sp]\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r2, [r2]\n\t"
        "asr r2, r2, #8\n\t"
        "str r2, [sp, #0x10]\n\t"
        "cmp r2, #0\n\t"
        "bge 1f\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #0x10]\n\t"
    "1:\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "add r1, #2\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0x10\n\t"
        "str r2, [sp, #0x18]\n\t"
        "ldr r3, 9f\n\t"
        "add r3, r3, r7\n\t"
        "mov sl, r3\n\t"
    "2:\n\t"
        "lsl r0, r1, #2\n\t"
        "ldr r2, [sp, #0x18]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r0, [r0]\n\t"
        "mov r8, r0\n\t"
        "sub r1, #1\n\t"
        "str r1, [sp, #0x14]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "b 19f\n\t"
    "3:\n\t"
        "mov r3, r8\n\t"
        "ldr r2, [r3]\n\t"
        "ldrb r0, [r2, #0xc]\n\t"
        "lsr r1, r0, #4\n\t"
        "mov r0, #1\n\t"
        "and r1, r0\n\t"
        "add r5, r2, #0\n\t"
        "cmp r1, #0\n\t"
        "beq 12f\n\t"
        "ldr r4, [r3, #0xc]\n\t"
        "cmp r4, #0\n\t"
        "bne 12f\n\t"
        "ldr r1, 10f\n\t"
        "add r2, r7, r1\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r3, [r1]\n\t"
        "ldr r0, [r1, #4]\n\t"
        "str r0, [r2]\n\t"
        "str r4, [r1, #4]\n\t"
        "str r5, [r3]\n\t"
        "str r4, [r3, #4]\n\t"
        "mov r2, r8\n\t"
        "str r2, [r3, #0xc]\n\t"
        "strb r4, [r3, #0x10]\n\t"
        "strb r4, [r3, #0x11]\n\t"
        "mov r1, sl\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "str r3, [r1]\n\t"
    "4:\n\t"
        "ldr r2, 11f\n\t"
        "add r1, r7, r2\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "str r3, [r0, #4]\n\t"
    "5:\n\t"
        "str r3, [r1]\n\t"
        "mov r0, r8\n\t"
        "str r3, [r0, #0xc]\n\t"
        "b 18f\n\t"
        ".align 2, 0\n"
    "6: .4byte gUnknown_03001308\n"
    "7: .4byte 0xFFFF9C00\n"
    "8: .4byte 0xFFFFC400\n"
    "9: .4byte 0x0000040C\n"
    "10: .4byte 0x00000814\n"
    "11: .4byte 0x0000080C\n"
    "12:\n\t"
        "mov r4, #1\n\t"
        "add r0, r4, #0\n\t"
        "ldrb r1, [r5, #0xc]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 17f\n\t"
        "mov sb, r5\n\t"
        "mov ip, r5\n\t"
        "mov r6, #0\n\t"
        "ldr r2, [r7, #4]\n\t"
        "add r4, r2, #0\n\t"
        "cmp r6, r4\n\t"
        "bge 15f\n\t"
        "ldr r0, [r7, #8]\n\t"
        "ldr r1, [r0]\n\t"
        "add r3, r0, #0\n\t"
        "cmp r1, r5\n\t"
        "beq 14f\n\t"
        "add r1, r3, #0\n\t"
    "13:\n\t"
        "add r1, #4\n\t"
        "add r6, #1\n\t"
        "cmp r6, r2\n\t"
        "bge 15f\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r0, ip\n\t"
        "bne 13b\n\t"
    "14:\n\t"
        "cmp r6, r4\n\t"
        "bge 15f\n\t"
        "lsl r4, r6, #2\n\t"
        "add r0, r4, r3\n\t"
        "ldr r1, [r0]\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_8009008\n\t"
        "add r0, r4, #4\n\t"
        "ldr r1, [r7, #8]\n\t"
        "add r0, r1, r0\n\t"
        "add r1, r1, r4\n\t"
        "ldr r2, [r7]\n\t"
        "sub r2, r2, r6\n\t"
        "ldr r3, 16f\n\t"
        "and r2, r3\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #0x13\n\t"
        "orr r2, r3\n\t"
        "bl sub_803A94C\n\t"
        "ldr r2, [r7]\n\t"
        "ldr r1, [r7, #8]\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r0, r1\n\t"
        "sub r0, #4\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0]\n\t"
        "sub r2, #1\n\t"
        "str r2, [r7]\n\t"
    "15:\n\t"
        "mov r2, sb\n\t"
        "cmp r2, #0\n\t"
        "beq 18f\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x50\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, sb\n\t"
        "ldr r2, [r1, #4]\n\t"
        "mov r1, #3\n\t"
        "bl sub_803AD80\n\t"
        "b 18f\n\t"
        ".align 2, 0\n"
    "16: .4byte 0x001FFFFF\n"
    "17:\n\t"
        "ldr r1, [r5, #0x18]\n\t"
        "add r1, #0x40\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r2, [r1, #4]\n\t"
        "mov r1, sp\n\t"
        "bl sub_803AD80\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 18f\n\t"
        "mov r3, r8\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r2, [r0, #0x18]\n\t"
        "mov r3, #0x18\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r2, #0x1c]\n\t"
        "bl sub_803AD7C\n\t"
        "mov r0, r8\n\t"
        "strb r4, [r0, #0x10]\n\t"
    "18:\n\t"
        "mov r1, r8\n\t"
        "ldr r1, [r1, #4]\n\t"
        "mov r8, r1\n\t"
        "cmp r1, #0\n\t"
        "beq 19f\n\t"
        "b 3b\n\t"
    "19:\n\t"
        "ldr r1, [sp, #0x14]\n\t"
        "ldr r2, [sp, #0x10]\n\t"
        "cmp r1, r2\n\t"
        "blt 20f\n\t"
        "b 2b\n\t"
    "20:\n\t"
        "mov r3, sl\n\t"
        "ldr r3, [r3]\n\t"
        "mov r8, r3\n\t"
        "cmp r3, #0\n\t"
        "beq 29f\n\t"
    "21:\n\t"
        "mov r0, r8\n\t"
        "ldr r2, [r0]\n\t"
        "mov r3, #1\n\t"
        "ldrb r1, [r2, #0xc]\n\t"
        "and r3, r1\n\t"
        "cmp r3, #0\n\t"
        "beq 26f\n\t"
        "mov sl, r2\n\t"
        "mov sb, r2\n\t"
        "mov r5, #0\n\t"
        "ldr r6, [r7, #4]\n\t"
        "add r4, r6, #0\n\t"
        "cmp r5, r4\n\t"
        "bge 24f\n\t"
        "ldr r0, [r7, #8]\n\t"
        "ldr r1, [r0]\n\t"
        "add r3, r0, #0\n\t"
        "cmp r1, r2\n\t"
        "beq 23f\n\t"
        "add r1, r3, #0\n\t"
    "22:\n\t"
        "add r1, #4\n\t"
        "add r5, #1\n\t"
        "cmp r5, r6\n\t"
        "bge 24f\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r0, sb\n\t"
        "bne 22b\n\t"
    "23:\n\t"
        "cmp r5, r4\n\t"
        "bge 24f\n\t"
        "lsl r4, r5, #2\n\t"
        "add r0, r4, r3\n\t"
        "ldr r1, [r0]\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_8009008\n\t"
        "add r0, r4, #4\n\t"
        "ldr r1, [r7, #8]\n\t"
        "add r0, r1, r0\n\t"
        "add r1, r1, r4\n\t"
        "ldr r2, [r7]\n\t"
        "sub r2, r2, r5\n\t"
        "ldr r3, 25f\n\t"
        "and r2, r3\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #0x13\n\t"
        "orr r2, r3\n\t"
        "bl sub_803A94C\n\t"
        "ldr r2, [r7]\n\t"
        "ldr r1, [r7, #8]\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r0, r1\n\t"
        "sub r0, #4\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0]\n\t"
        "sub r2, #1\n\t"
        "str r2, [r7]\n\t"
    "24:\n\t"
        "mov r2, sl\n\t"
        "cmp r2, #0\n\t"
        "beq 28f\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x50\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, sl\n\t"
        "ldr r2, [r1, #4]\n\t"
        "mov r1, #3\n\t"
        "bl sub_803AD80\n\t"
        "b 28f\n\t"
        ".align 2, 0\n"
    "25: .4byte 0x001FFFFF\n"
    "26:\n\t"
        "mov r0, r8\n\t"
        "ldr r1, [r0, #0xc]\n\t"
        "ldrb r0, [r1, #0x10]\n\t"
        "cmp r0, #0\n\t"
        "bne 27f\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "mov r3, #0x18\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r1, [r1, #0x1c]\n\t"
        "bl sub_803AD7C\n\t"
        "b 28f\n\t"
    "27:\n\t"
        "strb r3, [r1, #0x10]\n\t"
    "28:\n\t"
        "mov r0, r8\n\t"
        "ldr r0, [r0, #4]\n\t"
        "mov r8, r0\n\t"
        "cmp r0, #0\n\t"
        "bne 21b\n\t"
    "29:\n\t"
        "add sp, #0x1c\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
#endif
asm(".align 2, 0");
