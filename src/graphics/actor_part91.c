#include "core.h"

extern s32 gCellAnimFrameStep;
extern s32 gCellAnimSpeed;

/* Trivial getter for this scroll-effect subsystem's per-tick delta,
 * set by AdvanceCellAnim (still raw). */
s32 sub_8029B8C(void)
{
    return gCellAnimFrameStep;
}

/* `(v*15) << 2 >> 8` on gCellAnimSpeed (a Q8.8-ish speed/step value
 * set by SetCellAnimSpeed, still raw) - written as the ROM's own
 * shift-subtract-shift idiom (`(v<<4) - v`, i.e. `v*15`) rather than a
 * plain `* 15` to match its exact instruction sequence. */
s32 sub_8029B98(void)
{
    s32 v = gCellAnimSpeed;
    return ((v << 4) - v) << 2 >> 8;
}
