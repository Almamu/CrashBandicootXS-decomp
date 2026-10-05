#include "core.h"

/* A pair of "target minus current, halved toward zero" getters for this
 * BG2 affine scroll/zoom effect subsystem - gUnknown_030013FC/F8 are
 * target extents, gActorBgScrollY/D0 the current position (both
 * written by InitActorBgScroll/UpdateActorBgScroll, still raw). The `/2` is written
 * as the ROM's own round-toward-zero shift idiom
 * (`(x + (x>>31)) >> 1`, the ">>31" op arithmetic-shifting in the
 * value's sign bit as a 0/1 rounding nudge) rather than plain
 * division, matching this compiler's own signed-divide-by-2 codegen
 * either way - written explicitly since the standalone idiom is the
 * form seen used throughout this file's cluster. */
extern s32 gUnknown_030013FC;
extern s32 gActorBgScrollY;
extern s32 gUnknown_030013F8;
extern s32 gActorBgScrollX;

s32 sub_8029E98(void)
{
    return (gUnknown_030013FC / 2) - gActorBgScrollY;
}

s32 sub_8029EB4(void)
{
    return (gUnknown_030013F8 / 2) - gActorBgScrollX;
}
