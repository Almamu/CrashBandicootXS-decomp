#include "core.h"

/* Same palette-cycle cluster as actor_part41.c - see that file's header
 * comment and docs/matching/issue-50-actor-2a69c.md. Non-adjacent to
 * actor_part41.c since the parked `sub_802AB58` (actor_part42.c) sits
 * raw between them. */

extern s32 gUnknown_03001470;
extern s32 gUnknown_03001474;
extern s32 gUnknown_03001478;

extern s32 gStaticData_08178F60[];
extern s32 gStaticData_08178F70[];

/* Seeds the palette-cycle cursor/bound pair from a per-category table
 * (`gStaticData_08178F60`/`gStaticData_08178F70`, indexed by `idx`) and
 * resets the DMA-refresh counter. */
void sub_802ABC8(s32 idx)
{
    gUnknown_03001470 = gStaticData_08178F60[idx];
    gUnknown_03001474 = gStaticData_08178F70[idx];
    gUnknown_03001478 = 0;
}

extern u8 gUnknown_03001464;
extern void sub_802AB34(void);

/* Arms/disarms the palette-cycle system (`gUnknown_03001464`), resets
 * the cursor/bound/DMA-refresh counter, and saves that reset state back
 * via `sub_802AB34`. */
void sub_802ABFC(u8 flag)
{
    gUnknown_03001464 = flag;
    gUnknown_03001470 = 0;
    gUnknown_03001474 = 0;
    gUnknown_03001478 = 0;
    sub_802AB34();
}

asm(".align 2, 0");
