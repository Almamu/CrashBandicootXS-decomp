#include "core.h"

/* Trivial getter for this scroll-effect subsystem's accumulated X
 * offset, set by sub_8029B38 (still raw). */
extern s32 gUnknown_030013A8;

s32 sub_8029B2C(void)
{
    return gUnknown_030013A8;
}
