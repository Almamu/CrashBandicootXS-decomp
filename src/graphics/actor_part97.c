#include "core.h"

extern s32 gCellAnimTime;
extern s32 gCellAnimSpeed;
extern s32 gCellAnimFrameStep;
extern s32 gCellAnimLength;
extern s32 gUnknown_030013A8;

extern void UploadCellAnimFrame(void);
extern void UpdateActorPaletteCycle(void);

/* Advances the console/text-plane's horizontal scroll accumulator by
 * `gCellAnimSpeed` (a Q8.8 per-frame velocity), wrapping it against
 * `gCellAnimLength`, and - whenever the whole-tile column actually
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
void AdvanceCellAnim(void)
{
    register s32 *b0ptr asm("r4") = &gCellAnimTime;
    register s32 prev asm("r1") = *b0ptr;
    s32 prevShifted = prev >> 8;
    register s32 velocity asm("r0") = gCellAnimSpeed;
    register s32 pos asm("r3") = prev + velocity;
    register s32 *bcPtr asm("r1");
    s32 delta;

    *b0ptr = pos;
    bcPtr = &gCellAnimFrameStep;
    delta = (pos >> 8) - prevShifted;
    *bcPtr = delta;

    if (pos >= gCellAnimLength) {
        register s32 wrapped asm("r0") = pos - gCellAnimLength;
        *b0ptr = wrapped;
    }

    if (delta != 0) {
        gUnknown_030013A8 += delta;
        UploadCellAnimFrame();
        UpdateActorPaletteCycle();
    }
}
