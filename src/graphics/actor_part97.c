#include "core.h"

extern s32 gUnknown_030013B0;
extern s32 gUnknown_030013B4;
extern s32 gUnknown_030013BC;
extern s32 gUnknown_030013AC;
extern s32 gUnknown_030013A8;

extern void sub_80297C8(void);
extern void sub_802AB58(void);

/* Advances the console/text-plane's horizontal scroll accumulator by
 * `gUnknown_030013B4` (a Q8.8 per-frame velocity), wrapping it against
 * `gUnknown_030013AC`, and - whenever the whole-tile column actually
 * changed - shifts the visible-column counter and re-triggers the
 * pending-cell DMA/palette-cursor pair. `prev`/`velocity`/`pos` are
 * register-pinned to `r1`/`r0`/`r3` - this compiler otherwise reuses
 * `prev`'s own register in place for the sum (since `prev` is dead
 * after computing it), while the ROM keeps the updated position in a
 * genuinely separate register from the old value. `bcPtr` is
 * materialized (and pinned back to `r1`, reusing `prev`'s now-dead
 * register) *before* the shift/subtract that becomes `delta` - the ROM
 * loads the store destination's address ahead of computing the value,
 * not right before the store. `wrapped` is likewise pinned to a fresh
 * register rather than reusing `pos`'s own - same "ROM keeps the new
 * value in a genuinely separate register" pattern as `pos` itself (see
 * docs/workflow.md step 3 for this whole family of fixes). */
void sub_8029B38(void)
{
    register s32 *b0ptr asm("r4") = &gUnknown_030013B0;
    register s32 prev asm("r1") = *b0ptr;
    s32 prevShifted = prev >> 8;
    register s32 velocity asm("r0") = gUnknown_030013B4;
    register s32 pos asm("r3") = prev + velocity;
    register s32 *bcPtr asm("r1");
    s32 delta;

    *b0ptr = pos;
    bcPtr = &gUnknown_030013BC;
    delta = (pos >> 8) - prevShifted;
    *bcPtr = delta;

    if (pos >= gUnknown_030013AC) {
        register s32 wrapped asm("r0") = pos - gUnknown_030013AC;
        *b0ptr = wrapped;
    }

    if (delta != 0) {
        gUnknown_030013A8 += delta;
        sub_80297C8();
        sub_802AB58();
    }
}
