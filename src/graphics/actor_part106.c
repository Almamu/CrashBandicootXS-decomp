#include "core.h"

/* Genuine no-op stub - part of this file's small tilemap/scroll-effect
 * accessor cluster (see actor_part105.c/91.c/92.c/93.c), left as-is per
 * docs/naming.md's `nullsub_N` convention. */
void nullsub_5(void)
{
}

extern s32 gCellAnimCols;
extern s32 gCellAnimRows;

/* `gCellAnimCols`/`gCellAnimRows` are a tile width/height pair
 * (set by InitCellAnim, still raw) for this same scroll-effect subsystem;
 * this computes their product doubled plus one - likely a tile-count-
 * to-byte-count-ish conversion for a buffer this subsystem allocates,
 * not traced further. */
s32 GetCellAnimFreeTile(void)
{
    return gCellAnimCols * gCellAnimRows * 2 + 1;
}
