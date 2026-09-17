#include "core.h"
#include "memory.h"

/* Same "self" object family as actor_part39.c - see that file's header
 * comment and docs/matching/issue-50-actor-2a69c.md. Non-adjacent to
 * actor_part39.c since the parked `sub_802AA0C` (actor_part40.c) sits
 * raw between them. */

extern u8 gStaticData_087E4DF4[];

/* Trivial getter: `self+0x2c` (the constructor's one-shot byte flag). */
u8 sub_802AA4C(void *selfArg)
{
    return *((u8 *)selfArg + 0x2c);
}

/* Teardown: marks `self` "dead" (`+0x50 = gStaticData_087E4DF4`), unlinks
 * it from the circular `+0x48`(next)/`+0x4c`(prev) list, and frees it
 * when `flags & 1`. Same shape as `sub_802C19C`'s unlink sequence in
 * actor_part19.c. */
void sub_802AA54(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(u8 **)(self + 0x50) = gStaticData_087E4DF4;

    {
        u8 *prev = *(u8 **)(self + 0x4c);
        u8 *next = *(u8 **)(self + 0x48);
        *(u8 **)(prev + 0x48) = next;
    }
    {
        u8 *next = *(u8 **)(self + 0x48);
        u8 *prev = *(u8 **)(self + 0x4c);
        *(u8 **)(next + 0x4c) = prev;
    }

    if (flags & 1) {
        mem_free(self);
    }
}

extern s32 gUnknown_03000888;
extern void *gUnknown_03001428[];

/* Linear-searches `gUnknown_03001428`'s first `gUnknown_03000888`
 * entries for `self`, returning whether it's present. */
s32 sub_802AA80(void *selfArg)
{
    u8 *self = selfArg;
    register s32 i asm("r2") = 0;
    s32 count = gUnknown_03000888;

    if (i < count) {
        s32 n = count;
        register void **p asm("r1") = gUnknown_03001428;

        do {
            if (*p == self) {
                return 1;
            }
            p++;
            i++;
        } while (i < n);
    }
    return 0;
}

/* Appends `self` to `gUnknown_03001428` (capped at 15 entries), unless
 * it's `NULL`, the array is already full, or it's already present. */
void sub_802AAB4(void *selfArg)
{
    register u8 *self asm("r3") = selfArg;
    s32 count = gUnknown_03000888;

    if (count == 0xf || self == NULL) {
        return;
    }

    {
        s32 i = 0;

        if (i < count) {
            s32 n = count;
            void **p = gUnknown_03001428;

            do {
                if (*p == self) {
                    return;
                }
                p++;
                i++;
            } while (i < n);
        }
    }

    {
        s32 freshCount = gUnknown_03000888;

        gUnknown_03001428[freshCount] = self;
        gUnknown_03000888 = freshCount + 1;
    }
}

/* Clears `gUnknown_03001428`'s entry count. */
void sub_802AAFC(void)
{
    gUnknown_03000888 = 0;
}

extern u8 gUnknown_03001464;
extern s32 gUnknown_03001468;
extern s32 gUnknown_0300146C;
extern s32 gUnknown_03001470;
extern s32 gUnknown_03001474;
extern s32 gUnknown_03001478;

/* Loads the palette-cycle cursor/bound pair (`gUnknown_03001470`/
 * `gUnknown_03001474`, see `sub_802AB58` below) from their saved
 * counterparts (`gUnknown_03001468`/`gUnknown_0300146C`) and resets the
 * DMA-refresh counter `gUnknown_03001478`. */
void sub_802AB08(void)
{
    gUnknown_03001470 = gUnknown_03001468;
    gUnknown_03001474 = gUnknown_0300146C;
    gUnknown_03001478 = 0;
}

/* The inverse of `sub_802AB08`: saves the current cursor/bound pair back
 * into `gUnknown_03001468`/`gUnknown_0300146C`. */
void sub_802AB34(void)
{
    gUnknown_03001468 = gUnknown_03001470;
    gUnknown_0300146C = gUnknown_03001474;
}

asm(".align 2, 0");
