#include "core.h"
#include "gba/io_reg.h"

extern s32 gActorBgScrollX;
extern s32 gActorBg0VOffset;
extern s32 gActorBgScrollY;
extern s32 gActorBgShake;

/* Commits the BG0/BG1 scroll accumulators to the actual hardware
 * scroll registers, then clears the per-axis bias (gActorBgShake)
 * for the next frame. `x`/`yShift` are register-pinned (both reused
 * verbatim for the BG1 writes, matching the ROM's own register reuse),
 * and `dest`/`vofsDest` are materialized as explicit pointer locals
 * ahead of each store so this compiler loads the destination register's
 * address before the source value it's about to write - the ROM's own
 * instruction order - rather than the reverse order a plain
 * `REG_BG0HOFS = gActorBgScrollX >> 8;`/`REG_BG0VOFS = ...` compiles
 * to (see docs/workflow.md step 3). */
void CommitActorBgScroll(void)
{
    register s32 x asm("r2");
    register s32 yShift asm("r1");
    vu16 *dest = &REG_BG0HOFS;
    vu16 *vofsDest;

    x = gActorBgScrollX >> 8;
    *dest = x;
    vofsDest = &REG_BG0VOFS;
    yShift = gActorBgScrollY >> 8;
    *vofsDest = gActorBg0VOffset + yShift;
    REG_BG1HOFS = x;
    REG_BG1VOFS = yShift;

    gActorBgShake = 0;
}
