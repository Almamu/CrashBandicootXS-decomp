#include "core.h"

/* Same palette-cycle cluster as actor_part41.c - see that file's header
 * comment and docs/matching/issue-50-actor-2a69c.md. Non-adjacent to
 * actor_part41.c since the parked `UpdateActorPaletteCycle` (actor_part42.c) sits
 * raw between them. */

extern s32 gActorPaletteCycleFrame;
extern s32 gActorPaletteCycleTarget;
extern s32 gActorPaletteCycleTimer;

extern s32 gActorPaletteCycleStartFrames[];
extern s32 gActorPaletteCycleTargetFrames[];

/* Seeds the palette-cycle cursor/bound pair from a per-category table
 * (`gActorPaletteCycleStartFrames`/`gActorPaletteCycleTargetFrames`, indexed by `idx`) and
 * resets the DMA-refresh counter. */
void SetActorPaletteCycle(s32 idx)
{
    gActorPaletteCycleFrame = gActorPaletteCycleStartFrames[idx];
    gActorPaletteCycleTarget = gActorPaletteCycleTargetFrames[idx];
    gActorPaletteCycleTimer = 0;
}

extern u8 gActorPaletteCycleEnabled;
extern void SaveActorPaletteCycle(void);

/* Arms/disarms the palette-cycle system (`gActorPaletteCycleEnabled`), resets
 * the cursor/bound/DMA-refresh counter, and saves that reset state back
 * via `SaveActorPaletteCycle`. */
void EnableActorPaletteCycle(u8 flag)
{
    gActorPaletteCycleEnabled = flag;
    gActorPaletteCycleFrame = 0;
    gActorPaletteCycleTarget = 0;
    gActorPaletteCycleTimer = 0;
    SaveActorPaletteCycle();
}

asm(".align 2, 0");
