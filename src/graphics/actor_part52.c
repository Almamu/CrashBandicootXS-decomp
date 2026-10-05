#include "core.h"
#include "memory.h"

/* Same "self" object family as wumpa.c - see that file's header
 * comment and docs/matching/issue-50-actor-2a69c.md. Non-adjacent to
 * wumpa.c since the parked `sub_802AA0C` (actor_part40.c) sits
 * raw between them. */

extern u8 gActorVtable[];

/* Trivial getter: `self+0x2c` (the constructor's one-shot byte flag). */
u8 IsActorVisible(void *selfArg)
{
    return *((u8 *)selfArg + 0x2c);
}

/* Teardown: marks `self` "dead" (`+0x50 = gActorVtable`), unlinks
 * it from the circular `+0x48`(prev)/`+0x4c`(next) list, and frees it
 * when `flags & 1`. Same shape as `DestroyPolarPlayer`'s unlink sequence in
 * actor_part19.c. */
void DestroyActor(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(u8 **)(self + 0x50) = gActorVtable;

    {
        u8 *next = *(u8 **)(self + 0x4c);
        u8 *prev = *(u8 **)(self + 0x48);
        *(u8 **)(next + 0x48) = prev;
    }
    {
        u8 *prev = *(u8 **)(self + 0x48);
        u8 *next = *(u8 **)(self + 0x4c);
        *(u8 **)(prev + 0x4c) = next;
    }

    if (flags & 1) {
        mem_free(self);
    }
}

extern s32 gCollectedSpawnCount;
extern void *gCollectedSpawns[];

/* Linear-searches `gCollectedSpawns`'s first `gCollectedSpawnCount`
 * entries for `self`, returning whether it's present. */
s32 IsSpawnCollected(void *selfArg)
{
    u8 *self = selfArg;
    register s32 i asm("r2") = 0;
    s32 count = gCollectedSpawnCount;

    if (i < count) {
        s32 n = count;
        register void **p asm("r1") = gCollectedSpawns;

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

/* Appends `self` to `gCollectedSpawns` (capped at 15 entries), unless
 * it's `NULL`, the array is already full, or it's already present. */
void MarkSpawnCollected(void *selfArg)
{
    register u8 *self asm("r3") = selfArg;
    s32 count = gCollectedSpawnCount;

    if (count == 0xf || self == NULL) {
        return;
    }

    {
        s32 i = 0;

        if (i < count) {
            s32 n = count;
            void **p = gCollectedSpawns;

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
        s32 freshCount = gCollectedSpawnCount;

        gCollectedSpawns[freshCount] = self;
        gCollectedSpawnCount = freshCount + 1;
    }
}

/* Clears `gCollectedSpawns`'s entry count. */
void ClearCollectedSpawns(void)
{
    gCollectedSpawnCount = 0;
}

extern u8 gActorPaletteCycleEnabled;
extern s32 gSavedActorPaletteCycleFrame;
extern s32 gSavedActorPaletteCycleTarget;
extern s32 gActorPaletteCycleFrame;
extern s32 gActorPaletteCycleTarget;
extern s32 gActorPaletteCycleTimer;

/* Loads the palette-cycle cursor/bound pair (`gActorPaletteCycleFrame`/
 * `gActorPaletteCycleTarget`, see `UpdateActorPaletteCycle` below) from their saved
 * counterparts (`gSavedActorPaletteCycleFrame`/`gSavedActorPaletteCycleTarget`) and resets the
 * DMA-refresh counter `gActorPaletteCycleTimer`. */
void RestoreActorPaletteCycle(void)
{
    gActorPaletteCycleFrame = gSavedActorPaletteCycleFrame;
    gActorPaletteCycleTarget = gSavedActorPaletteCycleTarget;
    gActorPaletteCycleTimer = 0;
}

/* The inverse of `RestoreActorPaletteCycle`: saves the current cursor/bound pair back
 * into `gSavedActorPaletteCycleFrame`/`gSavedActorPaletteCycleTarget`. */
void SaveActorPaletteCycle(void)
{
    gSavedActorPaletteCycleFrame = gActorPaletteCycleFrame;
    gSavedActorPaletteCycleTarget = gActorPaletteCycleTarget;
}

asm(".align 2, 0");
