#include "core.h"

/* BG2 affine reference-point (REG_BG2X/REG_BG2Y) target fields for this
 * scroll/zoom effect subsystem - written by UpdateActorBgScroll (still raw),
 * read by CommitActorBgScroll (still raw). */
extern s32 gActorBgShake;
extern s32 gUnknown_030013D8;

void ShakeActorBg(s32 arg0)
{
    gActorBgShake = arg0;
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
