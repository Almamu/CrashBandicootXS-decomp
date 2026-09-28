#include "core.h"

/* GitHub issue #34/#40/#41, `UpdateGameFrame`-`MainLoop` cluster: the
 * second of the two raw functions `docs/matching/issue-34-game-loop-
 * 8022d50-80255d4.md` left for a follow-up pass (the first,
 * `sub_8022D50`, is `game_loop40.c`, now plain C).
 *
 * `self` is `*gUnknown_030012B4` (the same collision-bitmap base
 * `sub_8025944`/`sub_8025968`/`sub_802599C`, game_loop12.c, and
 * `sub_8025A0C`, game_loop13.c, already operate on).
 *
 * First half (fully understood, matches the ROM's own idiom one for
 * one): if `list` differs from `self`'s cached copy at `self+0`,
 * `self+8`/`self+0x208` (the first two of the three overlapping
 * collision-bitmap arrays `game_loop12.c`'s header comment documents)
 * are DMA3-zero-filled 64 bytes each (`DmaFill32(3, 0, dest, 64)`,
 * expanding to the exact same `REG_DMA3`-field-by-field store sequence
 * seen here); either way `self+8`->`self+0x108` and
 * `self+0x208`->`self+0x308` get unconditionally `CpuSet`-copied via
 * `sub_803A94C(src, dst, 0x04000040)` (the same idiom `sub_8022CA0`,
 * game_loop.c, already documents in the opposite direction), and
 * `self+4` is set from `posArg >> 8` (a Q8-to-int truncation). `list`
 * is then walked as a `{count:u16 @2, groups:ptr @4}` header over
 * `{count:u16 @2, items:ptr @4}` 8-byte group records, each holding
 * `{tableIdx:u16, p1:u16, p2:u16, p3:u16}` 8-byte item records; for
 * each item not already flagged in the `self+8` bit-grid
 * (`sub_8025968`), `sub_8025D28` (the table-indexed interworking-
 * trampoline dispatcher, game_loop14.c) fires with a running,
 * never-reset-per-group counter as its own `self` argument, indexing
 * `gUnknown_030012E4`'s table.
 *
 * Second half (skipped when `links` is NULL): each actor in
 * `gUnknown_0300130C` whose id is a link's `from` is chained
 * (`sub_8010714`/`sub_8010710`) to the actor with the link's `to` id,
 * following further links while `to` isn't spawned. Then each link whose
 * `from` actor doesn't exist resolves its `to` chain to a spawned actor
 * and moves that actor's neighbour chain (`sub_801070C`/`sub_8007398`)
 * up by its `+0x10` method's height.
 *
 * NAKED: plain C under old_agbcc is 153 halfwords off (732 bytes vs the
 * ROM's 704). Everything through the first link pass is byte-exact; in
 * the second pass the ROM walks each actor-list search with a
 * strength-reduced item pointer and a cached count, with no peeled first
 * iteration, while the reconstruction compiles the searches index-based
 * with the first iteration peeled.
 *
 * Later pass (#40 retry): the `#if NON_MATCHING` draft below is now
 * size-exact (704 bytes) under old_agbcc but ~219 halfwords off, mostly
 * block placement: the ROM lays the link-chasing do-while out with its
 * actor search first (entered via `b` to the loop test) and the
 * link-list scan after it, where this compile places the scan first;
 * the third pass's "found" flags also land in different registers.
 */
#if NON_MATCHING
struct lk_item
{
    u16 tableIdx;
    u16 p1;
    u16 p2;
    u16 p3;
};

struct lk_group
{
    u16 unk_00;
    u16 count;                      /* +0x02 */
    struct lk_item *items;          /* +0x04 */
};

struct lk_list
{
    u16 unk_00;
    u16 count;                      /* +0x02 */
    struct lk_group *groups;        /* +0x04 */
};

struct lk_self
{
    struct lk_list *list;           /* +0x000 */
    s32 pos;                        /* +0x004 */
    u8 bits0[0x100];                /* +0x008 */
    u8 bits0Copy[0x100];            /* +0x108 */
    u8 bits1[0x100];                /* +0x208 */
    u8 bits1Copy[0x100];            /* +0x308 */
};

struct lk_link
{
    s32 from;
    s32 to;
};

struct lk_links
{
    s32 count;
    struct lk_link links[0];
};

struct lk_point
{
    s32 x;
    s32 y;
};

struct lk_method
{
    s16 delta;
    u8 unk_02[2];
    void *fn;
};

struct lk_vtable
{
    u8 unk_00[0x10];
    struct lk_method height;        /* +0x10 */
};

struct lk_actor
{
    struct lk_point pos;            /* +0x00 */
    u16 id;                         /* +0x08 */
    u8 unk_0A[0xE];
    struct lk_vtable *vtable;       /* +0x18 */
};

struct lk_actor_list
{
    s32 count;
    u8 unk_04[4];
    struct lk_actor **items;        /* +0x08 */
};

extern void *gUnknown_030012E4;
extern struct lk_actor_list *gUnknown_0300130C;
extern void sub_803A94C(void *src, void *dst, s32 control);
extern u8 sub_8025968(struct lk_self *self, s32 n);
extern void sub_8025D28(void *table, s32 n, struct lk_item *item);
extern void sub_8010714(struct lk_actor *a, struct lk_actor *b);
extern void sub_8010710(struct lk_actor *a, struct lk_actor *b);
extern struct lk_actor *sub_801070C(struct lk_actor *a);
extern void sub_8007398(struct lk_actor *a, struct lk_point pos);
extern u8 *sub_803AD7C(void *self, void *fn);

void sub_80255D4(struct lk_self *self, struct lk_list *list, struct lk_links *links, s32 posArg)
{
    s32 counter;
    s32 g;
    s32 n;
    struct lk_link *lk;

    if (list != self->list)
    {
        self->list = list;
        DmaFill32(3, 0, self->bits0, 64);
        DmaFill32(3, 0, self->bits1, 64);
    }
    sub_803A94C(self->bits0, self->bits0Copy, 0x04000040);
    sub_803A94C(self->bits1, self->bits1Copy, 0x04000040);
    self->pos = posArg >> 8;

    counter = 0;
    for (g = self->list->count - 1; g >= 0; g--)
    {
        struct lk_group *group = &self->list->groups[g];
        s32 k;

        for (k = 0; k < group->count; k++)
        {
            if (!sub_8025968(self, counter))
                sub_8025D28(gUnknown_030012E4, counter, &group->items[k]);
            counter++;
        }
    }

    if (links == NULL)
        return;
    n = links->count;
    lk = links->links;

    {
        s32 i;

        for (i = gUnknown_0300130C->count - 1; i >= 0; i--)
        {
            struct lk_actor *actor = gUnknown_0300130C->items[i];
            u16 id = actor->id;
            s32 j;

            for (j = 0; j < n; j++)
            {
                if (id == lk[j].from)
                {
                    s32 done = 0;
                    s32 to = lk[j].to;

                    do
                    {
                        s32 k;
                        s32 missing;
                        s32 m;

                        for (k = gUnknown_0300130C->count - 1; k >= 0; k--)
                        {
                            struct lk_actor *other = gUnknown_0300130C->items[k];

                            if (to == other->id)
                            {
                                sub_8010714(actor, other);
                                sub_8010710(other, actor);
                                done = 1;
                                break;
                            }
                        }
                        if (done)
                            break;
                        missing = 1;
                        for (m = 0; m < n; m++)
                        {
                            if (to == lk[m].from)
                            {
                                to = lk[m].to;
                                missing = 0;
                                break;
                            }
                        }
                        if (missing)
                            done = 1;
                    } while (!done);
                    break;
                }
            }
        }
    }

    {
        s32 j;

        for (j = 0; j < n; j++)
        {
            u16 from = lk[j].from;
            struct lk_actor *actor;
            s32 to;
            s32 found;

            {
                s32 k;
                s32 cnt = gUnknown_0300130C->count;
                struct lk_actor **items;

                found = 0;
                if (0 < cnt)
                {
                    items = gUnknown_0300130C->items;
                    for (k = 0; k < cnt; k++, items++)
                    {
                        if ((*items)->id == from)
                        {
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (found)
                continue;

            to = (u16)lk[j].to;
            actor = NULL;
            for (;;)
            {
                s32 missing = 1;
                s32 next = 0;
                s32 m;

                for (m = 0; m < n; m++)
                {
                    if (to == lk[m].from)
                    {
                        s32 k;

                        missing = 0;
                        next = (u16)lk[m].to;
                        for (k = 0; k < gUnknown_0300130C->count; k++)
                        {
                            actor = gUnknown_0300130C->items[k];
                            if (actor->id == to)
                                goto move;
                        }
                        break;
                    }
                }
                if (missing)
                {
                    s32 k;

                    for (k = 0; k < gUnknown_0300130C->count; k++)
                    {
                        struct lk_actor *a = gUnknown_0300130C->items[k];

                        if (a->id == to)
                        {
                            actor = a;
                            goto move;
                        }
                    }
                    break;
                }
                to = next;
            }
            continue;
        move:
            if (actor != NULL)
            {
                struct lk_method *hm = &actor->vtable->height;
                s32 lift = (sub_803AD7C((u8 *)actor + hm->delta, hm->fn)[5] + 1) << 8;

                do
                {
                    struct lk_point p;

                    p.x = actor->pos.x;
                    p.y = actor->pos.y + lift;
                    sub_8007398(actor, p);
                    actor = sub_801070C(actor);
                } while (actor != NULL);
            }
        }
    }
}
#else
NAKED void sub_80255D4(void *self, void *list, void *links, s32 posArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x14\n\t"
        "add r6, r0, #0\n\t"
        "mov sb, r2\n\t"
        "add r5, r3, #0\n\t"
        "mov r3, #0\n\t"
        "ldr r0, [r6]\n\t"
        "cmp r1, r0\n\t"
        "beq 1f\n\t"
        "str r1, [r6]\n\t"
        "str r3, [sp]\n\t"
        "ldr r0, 90f\n\t"
        "mov r1, sp\n\t"
        "str r1, [r0]\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #8\n\t"
        "str r1, [r0, #4]\n\t"
        "ldr r2, 91f\n\t"
        "str r2, [r0, #8]\n\t"
        "ldr r1, [r0, #8]\n\t"
        "str r3, [sp]\n\t"
        "mov r3, sp\n\t"
        "str r3, [r0]\n\t"
        "mov r4, #0x82\n\t"
        "lsl r4, r4, #2\n\t"
        "add r1, r6, r4\n\t"
        "str r1, [r0, #4]\n\t"
        "str r2, [r0, #8]\n\t"
        "ldr r0, [r0, #8]\n\t"
    "1:\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #8\n\t"
        "mov r2, #0x84\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r6, r2\n\t"
        "ldr r4, 92f\n\t"
        "add r2, r4, #0\n\t"
        "bl sub_803A94C\n\t"
        "mov r3, #0x82\n\t"
        "lsl r3, r3, #2\n\t"
        "add r0, r6, r3\n\t"
        "mov r2, #0xc2\n\t"
        "lsl r2, r2, #2\n\t"
        "add r1, r6, r2\n\t"
        "add r2, r4, #0\n\t"
        "bl sub_803A94C\n\t"
        "asr r0, r5, #8\n\t"
        "str r0, [r6, #4]\n\t"
        "mov r7, #0\n\t"
        "ldr r0, [r6]\n\t"
        "ldrh r2, [r0, #2]\n\t"
        "sub r2, #1\n\t"
        "cmp r2, #0\n\t"
        "blt 2f\n\t"
    "3:\n\t"
        "ldr r0, [r6]\n\t"
        "lsl r1, r2, #3\n\t"
        "ldr r0, [r0, #4]\n\t"
        "add r5, r0, r1\n\t"
        "mov r4, #0\n\t"
        "sub r2, #1\n\t"
        "mov r8, r2\n\t"
        "ldrh r3, [r5, #2]\n\t"
        "cmp r4, r3\n\t"
        "bge 5f\n\t"
    "4:\n\t"
        "add r0, r6, #0\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_8025968\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 6f\n\t"
        "ldr r0, 93f\n\t"
        "ldr r0, [r0]\n\t"
        "lsl r1, r4, #3\n\t"
        "ldr r2, [r5, #4]\n\t"
        "add r2, r2, r1\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_8025D28\n\t"
    "6:\n\t"
        "add r7, #1\n\t"
        "add r4, #1\n\t"
        "ldrh r0, [r5, #2]\n\t"
        "cmp r4, r0\n\t"
        "blt 4b\n\t"
    "5:\n\t"
        "mov r2, r8\n\t"
        "cmp r2, #0\n\t"
        "bge 3b\n\t"
    "2:\n\t"
        "mov r1, sb\n\t"
        "cmp r1, #0\n\t"
        "bne 7f\n\t"
        "b 30f\n\t"
    "7:\n\t"
        "add r1, #4\n\t"
        "mov sb, r1\n\t"
        "sub r1, #4\n\t"
        "ldm r1!, {r2}\n\t"
        "mov r8, r2\n\t"
        "ldr r0, 94f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "sub r2, r0, #1\n\t"
        "cmp r2, #0\n\t"
        "blt 15f\n\t"
    "8:\n\t"
        "ldr r0, 94f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #8]\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r7, [r0]\n\t"
        "ldrh r4, [r7, #8]\n\t"
        "mov r3, #0\n\t"
        "sub r2, #1\n\t"
        "str r2, [sp, #0x10]\n\t"
        "cmp r3, r8\n\t"
        "bge 13f\n\t"
        "mov r1, sb\n\t"
    "9:\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r4, r0\n\t"
        "bne 12f\n\t"
        "mov r6, #0\n\t"
        "ldr r5, [r1, #4]\n\t"
        "ldr r3, 94f\n\t"
        "mov sl, r3\n\t"
    "10:\n\t"
        "ldr r0, 94f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "sub r2, r0, #1\n\t"
        "b 17f\n\t"
        ".align 2, 0\n"
    "90: .4byte 0x040000D4\n"
    "91: .4byte 0x85000010\n"
    "92: .4byte 0x04000040\n"
    "93: .4byte gUnknown_030012E4\n"
    "94: .4byte gUnknown_0300130C\n"
    "16:\n\t"
        "sub r2, #1\n\t"
    "17:\n\t"
        "cmp r2, #0\n\t"
        "blt 20f\n\t"
        "mov r4, sl\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r1, [r0, #8]\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r4, [r0]\n\t"
        "ldrh r0, [r4, #8]\n\t"
        "cmp r5, r0\n\t"
        "bne 16b\n\t"
        "add r0, r7, #0\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8010714\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_8010710\n\t"
        "mov r6, #1\n\t"
    "20:\n\t"
        "cmp r6, #0\n\t"
        "bne 13f\n\t"
        "mov r3, #1\n\t"
        "mov r2, #0\n\t"
        "cmp r6, r8\n\t"
        "bge 21f\n\t"
        "mov r1, sb\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r5, r0\n\t"
        "bne 22f\n\t"
        "ldr r5, [r1, #4]\n\t"
        "b 23f\n\t"
    "22:\n\t"
        "add r2, #1\n\t"
        "cmp r2, r8\n\t"
        "bge 21f\n\t"
        "lsl r0, r2, #3\n\t"
        "mov r4, sb\n\t"
        "add r1, r0, r4\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r5, r0\n\t"
        "bne 22b\n\t"
        "ldr r5, [r1, #4]\n\t"
        "mov r3, #0\n\t"
    "21:\n\t"
        "cmp r3, #0\n\t"
        "beq 23f\n\t"
        "mov r6, #1\n\t"
    "23:\n\t"
        "cmp r6, #0\n\t"
        "beq 10b\n\t"
        "b 13f\n\t"
    "12:\n\t"
        "add r1, #8\n\t"
        "add r3, #1\n\t"
        "cmp r3, r8\n\t"
        "blt 9b\n\t"
    "13:\n\t"
        "ldr r2, [sp, #0x10]\n\t"
        "cmp r2, #0\n\t"
        "bge 8b\n\t"
    "15:\n\t"
        "mov r3, #0\n\t"
        "cmp r3, r8\n\t"
        "blt 25f\n\t"
        "b 30f\n\t"
    "25:\n\t"
        "lsl r0, r3, #3\n\t"
        "mov r2, sb\n\t"
        "add r1, r0, r2\n\t"
        "ldrh r5, [r1]\n\t"
        "mov r7, #0\n\t"
        "mov r4, #0\n\t"
        "ldr r1, 95f\n\t"
        "ldr r1, [r1]\n\t"
        "ldr r2, [r1]\n\t"
        "add r6, r0, #0\n\t"
        "add r3, #1\n\t"
        "str r3, [sp, #0xc]\n\t"
        "cmp r7, r2\n\t"
        "bge 27f\n\t"
        "ldr r1, [r1, #8]\n\t"
    "26:\n\t"
        "ldr r0, [r1]\n\t"
        "ldrh r0, [r0, #8]\n\t"
        "cmp r0, r5\n\t"
        "beq 33f\n\t"
        "add r1, #4\n\t"
        "add r4, #1\n\t"
        "cmp r4, r2\n\t"
        "blt 26b\n\t"
    "27:\n\t"
        "cmp r7, #0\n\t"
        "bne 33f\n\t"
        "mov r3, sb\n\t"
        "add r0, r6, r3\n\t"
        "ldrh r5, [r0, #4]\n\t"
        "mov r6, #0\n\t"
        "ldr r4, 95f\n\t"
        "mov sl, r4\n\t"
        "b 29f\n\t"
        ".align 2, 0\n"
    "95: .4byte gUnknown_0300130C\n"
    "28:\n\t"
        "mov r5, ip\n\t"
    "29:\n\t"
        "mov r7, #1\n\t"
        "mov r0, #0\n\t"
        "mov ip, r0\n\t"
        "mov r2, #0\n\t"
        "mov r1, r8\n\t"
        "cmp r1, #0\n\t"
        "ble 31f\n\t"
        "mov r1, sb\n\t"
    "32:\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r5, r0\n\t"
        "bne 34f\n\t"
        "mov r7, #0\n\t"
        "ldrh r1, [r1, #4]\n\t"
        "mov ip, r1\n\t"
        "mov r3, #0\n\t"
        "ldr r2, 96f\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r7, r0\n\t"
        "bge 31f\n\t"
        "mov r4, sl\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r0, #8]\n\t"
    "35:\n\t"
        "ldr r6, [r0]\n\t"
        "ldrh r1, [r6, #8]\n\t"
        "cmp r1, r5\n\t"
        "beq 40f\n\t"
        "add r0, #4\n\t"
        "add r3, #1\n\t"
        "cmp r3, r2\n\t"
        "blt 35b\n\t"
        "b 31f\n\t"
        ".align 2, 0\n"
    "96: .4byte gUnknown_0300130C\n"
    "34:\n\t"
        "add r1, #8\n\t"
        "add r2, #1\n\t"
        "cmp r2, r8\n\t"
        "blt 32b\n\t"
    "31:\n\t"
        "mov r0, #0\n\t"
        "cmp r0, #0\n\t"
        "bne 40f\n\t"
        "cmp r7, #0\n\t"
        "beq 38f\n\t"
        "mov r4, #0\n\t"
        "ldr r1, 97f\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r4, r0\n\t"
        "bge 38f\n\t"
        "mov r2, sl\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r3, [r0]\n\t"
        "ldr r2, [r0, #8]\n\t"
    "36:\n\t"
        "ldr r1, [r2]\n\t"
        "ldrh r0, [r1, #8]\n\t"
        "cmp r0, r5\n\t"
        "bne 37f\n\t"
        "add r6, r1, #0\n\t"
        "b 40f\n\t"
        ".align 2, 0\n"
    "97: .4byte gUnknown_0300130C\n"
    "37:\n\t"
        "add r2, #4\n\t"
        "add r4, #1\n\t"
        "cmp r4, r3\n\t"
        "blt 36b\n\t"
    "38:\n\t"
        "mov r3, #0\n\t"
        "cmp r3, #0\n\t"
        "bne 40f\n\t"
        "cmp r7, #0\n\t"
        "beq 28b\n\t"
        "b 33f\n\t"
    "40:\n\t"
        "cmp r6, #0\n\t"
        "beq 33f\n\t"
        "ldr r1, [r6, #0x18]\n\t"
        "mov r4, #0x10\n\t"
        "ldrsh r0, [r1, r4]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r1, [r1, #0x14]\n\t"
        "bl sub_803AD7C\n\t"
        "ldrb r0, [r0, #5]\n\t"
        "add r0, #1\n\t"
        "lsl r5, r0, #8\n\t"
        "add r4, sp, #4\n\t"
    "41:\n\t"
        "ldr r0, [r6]\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r2, [r6, #4]\n\t"
        "add r2, r2, r5\n\t"
        "str r2, [r4, #4]\n\t"
        "ldr r1, [sp, #4]\n\t"
        "add r0, r6, #0\n\t"
        "bl sub_8007398\n\t"
        "add r0, r6, #0\n\t"
        "bl sub_801070C\n\t"
        "add r6, r0, #0\n\t"
        "cmp r6, #0\n\t"
        "bne 41b\n\t"
    "33:\n\t"
        "ldr r3, [sp, #0xc]\n\t"
        "cmp r3, r8\n\t"
        "bge 30f\n\t"
        "b 25b\n\t"
    "30:\n\t"
        "add sp, #0x14\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}
#endif
