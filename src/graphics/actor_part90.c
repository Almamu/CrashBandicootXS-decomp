#include "core.h"

/* Trivial getter for this scroll-effect subsystem's accumulated X
 * offset, set by AdvanceCellAnim (still raw). */
extern s32 gCellAnimDistance;

s32 GetCellAnimDistance(void)
{
    return gCellAnimDistance;
}
