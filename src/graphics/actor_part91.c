#include "core.h"

extern s32 gUnknown_030013BC;
extern s32 gUnknown_030013B4;

/* Trivial getter for this scroll-effect subsystem's per-tick delta,
 * set by sub_8029B38 (still raw). */
s32 sub_8029B8C(void)
{
    return gUnknown_030013BC;
}

/* `(v*15) << 2 >> 8` on gUnknown_030013B4 (a Q8.8-ish speed/step value
 * set by sub_8029BAC, still raw) - written as the ROM's own
 * shift-subtract-shift idiom (`(v<<4) - v`, i.e. `v*15`) rather than a
 * plain `* 15` to match its exact instruction sequence. */
s32 sub_8029B98(void)
{
    s32 v = gUnknown_030013B4;
    return ((v << 4) - v) << 2 >> 8;
}
