#include "core.h"

/* Writes the graphics-package "self" scratch buffer's +0x00/+0x04 pair
 * verbatim (a second, narrower constructor alongside `sub_801E644`'s
 * five-argument one - same buffer, different subset of fields). */
void sub_801E964(u8 *self, u32 arg1, u32 arg2)
{
    *(u32 *)(self + 0) = arg1;
    *(u32 *)(self + 4) = arg2;
}

/* Clears bits 4-9 (masked via -0x11/-0x21/0x3f, i.e. a combined
 * ~0x10 & ~0x20 & 0x3f = 0xf) of the same scratch buffer's byte +0x11,
 * and clears bit 2 (mask -0xd, i.e. ~4) of byte +0x15 - two independent
 * "reset before rebuild" bitfield clears on the same buffer
 * `sub_801E8F8`/`sub_801E950` write. */
void sub_801E96C(u8 *self)
{
    register s32 mask asm("r3") = -0xd;
    register s32 b asm("r1");

    b = mask;
    asm("" : "+r"(b));
    b &= self[0x11];
    b &= -0x11;
    b &= -0x21;
    b &= 0x3f;
    self[0x11] = b;
    mask &= self[0x15];
    self[0x15] = mask;
}
/* This object is the last thing linked before the still-raw
 * asm/code_3_2_17_1e990.s continuation, which starts at a 4-byte-aligned
 * ROM address (0x0801E990) two bytes past sub_801E96C's own end
 * (0x0801E98E) - the ROM pads that gap with zero bytes (a real
 * `.align 2, 0` in the original assembly), not this compiler's default
 * Thumb NOP-fill (`0x46C0`) for an implicit end-of-object alignment. See
 * the matching_decomp_alignment_fix technique. */
asm(".align 2, 0");
