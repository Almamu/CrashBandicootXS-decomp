#include "core.h"

/* Genuine no-op stub - part of this file's small tilemap/scroll-effect
 * accessor cluster (see actor_part88.c/91.c/92.c/93.c), left as-is per
 * docs/naming.md's `nullsub_N` convention. */
void nullsub_5(void)
{
}

extern s32 gUnknown_03001398;
extern s32 gUnknown_0300139C;

/* `gUnknown_03001398`/`gUnknown_0300139C` are a tile width/height pair
 * (set by sub_8029890, still raw) for this same scroll-effect subsystem;
 * this computes their product doubled plus one - likely a tile-count-
 * to-byte-count-ish conversion for a buffer this subsystem allocates,
 * not traced further. */
s32 sub_8029AC4(void)
{
    return gUnknown_03001398 * gUnknown_0300139C * 2 + 1;
}
