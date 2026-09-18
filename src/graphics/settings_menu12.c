#include "core.h"
#include "gba/defines.h"
#include "actor.h"
#include "pause_screen_results.h"

/* The composite pause/options screen's "apply display registers" step
 * for its own top-level object - see include/pause_screen_results.h for
 * the full reconciled struct (this function only touches field_c8/
 * field_cc/field_d0). Distinct from - and much larger than -
 * `struct sub_8006700_actor` (src/graphics/oam_count.c/settings_menu10.c),
 * which is the smaller per-widget object `src/graphics/oam_count.c`'s
 * already-matched `sub_8006714` uses for the same job at different
 * offsets. */

extern void sub_80006A8(void *arg0);
extern void sub_8006DC8(void *arg0);
extern void sub_8006AAC(void *arg0);
extern void FlushVramDmaQueue(void);
extern void *gUnknown_030012B8;
extern void *gUnknown_03001300;

/* `self->field_d0`'s read+store is deliberately routed through an
 * inline-asm-computed address pinned to `r0` rather than a plain
 * `self->field_d0` field access: with the latter, this compiler
 * recognizes `self` (r4) is dead after this point and folds the
 * address computation directly into r4 (saving a `mov`), one
 * instruction shorter than the ROM's fresh r0 computation - the ROM
 * never performs this particular reuse here (though it does for the
 * `field_c8`/`field_cc` accesses just above, which this reconstruction
 * gets for free from plain field access). */
void sub_8006250(struct pause_screen_results *self)
{
    sub_80006A8(self);
    sub_8006DC8(gUnknown_030012B8);
    sub_8006AAC(gUnknown_03001300);
    FlushVramDmaQueue();
    *(vu16 *)PLTT = 0;
    *(vu32 *)REG_ADDR_BLDCNT = self->field_c8;
    *(vu16 *)REG_ADDR_BLDY = (u32)(self->field_cc << 27) >> 27;
    {
        register u16 *p asm("r0");
        vu16 *dst = (vu16 *)REG_ADDR_DISPCNT;
        asm("add %0, %1, #0\n\tadd %0, %0, #0xd0" : "=r" (p) : "r" (self));
        *dst = *p;
    }
}
