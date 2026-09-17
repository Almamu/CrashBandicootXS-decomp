#include "core.h"
#include "gba/gba.h"

/* Fills one 4bpp VRAM tile (`0x06017800`) with a solid color index via a
 * word-sized DMA3 transfer from a stack scratch halfword: `arg1`'s low
 * nibble is replicated into all four nibbles of a 16-bit pattern, and
 * also (rounded down to a multiple of 0x10) packed into the low 4 bits
 * of the "self" scratch buffer's byte +0x15 (alongside its own +0x1c
 * 32-bit field, set verbatim to `arg1`) - the same graphics-package
 * scratch buffer `sub_801E640`/`sub_801E644` write, extended here with
 * two more fields. */
void sub_801E8F8(u8 *selfArg, s32 arg1)
{
    register u8 *self asm("r4") = selfArg;
    register s32 val asm("r3");
    register s32 aligned asm("r2");
    register u32 mask asm("r1");
    register u32 acc asm("r0");
    u16 buf;
    u32 dadVal;
    vu32 *dma;

    val = arg1;
    *(s32 *)(self + 0x1c) = val;
    aligned = val;
    if (val < 0) {
        aligned += 0xf;
    }
    aligned >>= 4;
    aligned <<= 4;

    mask = 0xf;
    acc = mask;
    asm("" : "+r"(acc));
    acc &= self[0x15];
    acc |= aligned;
    self[0x15] = acc;

    mask &= val;
    acc = mask << 4;
    aligned = mask << 8;
    acc |= aligned;
    aligned = mask << 0xc;
    acc |= aligned;
    mask |= acc;

    dadVal = 0x06017800;
    buf = mask;
    dma = (vu32 *)REG_ADDR_DMA3SAD;
    dma[0] = (u32)&buf;
    dma[1] = dadVal;
    dma[2] = 0x81000400;
    dma[2];
}

#if NON_MATCHING
/* Packs `arg1`'s low 2 bits into bits 2-3 of the same "self" scratch
 * buffer's byte +0x15 that `sub_801E8F8` writes bits 0-3 of (a second,
 * narrower bitfield update on the same byte - callers of this family
 * build up the same 0x10-byte scratch buffer field by field before
 * `LoadGraphicsPackage`). Fully matches in shape and every instruction
 * except one: the ROM rematerializes the `-0xd` mask via
 * `sub r2,r2,#0x10` off the register that still holds the earlier `#3`
 * constant, while every C phrasing tried here (a fresh `-0xd` literal,
 * `~0xc`, a `register`-pinned intermediate, an `asm("":"+r"(...))`
 * compiler barrier between the two constant loads) makes this compiler
 * reload `#0xd` fresh instead, one instruction short of the ROM's
 * count. Parked rather than keep guessing - see
 * docs/matching/issue-30-graphics-loading.md. */
void sub_801E950(u8 *self, u32 arg1)
{
    register s32 shifted asm("r1");
    register s32 b asm("r2");

    shifted = arg1 & 3;
    shifted <<= 2;
    b = -0xd;
    b &= self[0x15];
    b |= shifted;
    self[0x15] = b;
}
#endif /* NON_MATCHING */
