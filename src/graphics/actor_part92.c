#include "core.h"

/* BG2 affine reference-point (REG_BG2X/REG_BG2Y) target fields for this
 * scroll/zoom effect subsystem - written by sub_8029D8C (still raw),
 * read by sub_8029E50 (still raw). */
extern s32 gUnknown_030013D4;
extern s32 gUnknown_030013D8;

void sub_8029E28(s32 arg0)
{
    gUnknown_030013D4 = arg0;
}

void sub_8029E34(s32 arg0)
{
    gUnknown_030013D8 = arg0;
}

s32 sub_8029E40(void)
{
    return gUnknown_030013D8;
}

/* Genuine no-op stub - see nullsub_5 (actor_part106.c). */
void nullsub_6(void)
{
}

/* Being the last function in this translation unit, the trailing 2-byte
 * alignment pad needs an explicit file-scope `asm(".align 2, 0")` - this
 * compiler otherwise pads with a `mov r8, r8` no-op instead of the
 * ROM's zero bytes (see matching_decomp_alignment_fix memory /
 * docs/matching.md). */
asm(".align 2, 0");
