#include "core.h"
#include "gba/io_reg.h"

extern s32 gUnknown_030013D0;
extern s32 gUnknown_030013F4;
extern s32 gUnknown_030013CC;
extern s32 gUnknown_030013D4;

/* Commits the BG0/BG1 scroll accumulators to the actual hardware
 * scroll registers, then clears the per-axis bias (gUnknown_030013D4)
 * for the next frame. `x`/`yShift` are register-pinned (both reused
 * verbatim for the BG1 writes, matching the ROM's own register reuse),
 * and `dest`/`vofsDest` are materialized as explicit pointer locals
 * ahead of each store so this compiler loads the destination register's
 * address before the source value it's about to write - the ROM's own
 * instruction order - rather than the reverse order a plain
 * `REG_BG0HOFS = gUnknown_030013D0 >> 8;`/`REG_BG0VOFS = ...` compiles
 * to (see docs/workflow.md step 3). */
void sub_8029E50(void)
{
    register s32 x asm("r2");
    register s32 yShift asm("r1");
    vu16 *dest = &REG_BG0HOFS;
    vu16 *vofsDest;

    x = gUnknown_030013D0 >> 8;
    *dest = x;
    vofsDest = &REG_BG0VOFS;
    yShift = gUnknown_030013CC >> 8;
    *vofsDest = gUnknown_030013F4 + yShift;
    REG_BG1HOFS = x;
    REG_BG1VOFS = yShift;

    gUnknown_030013D4 = 0;
}
