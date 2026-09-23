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

/* Packs `arg1`'s low 2 bits into bits 2-3 of the same "self" scratch
 * buffer's byte +0x15 that `sub_801E8F8` writes bits 0-3 of (a second,
 * narrower bitfield update on the same byte - callers of this family
 * build up the same 0x10-byte scratch buffer field by field before
 * `LoadGraphicsPackage`).
 *
 * The two `& 3`/`neg`-mask constants land in the ROM's own registers
 * (both in r2, one right after the other - a fresh `mov r2,#0xd`
 * reload, not a reuse of the earlier `#3` value) once the second mask
 * is materialized via the same `mov #N; neg` opaque-asm idiom as
 * `UPDATE_ICON_FRAME_NIBBLE` (src/graphics/settings_menu6.c) instead of
 * a plain C `~0xc`/`-0xd`, which this compiler folds differently. */
void sub_801E950(u8 *self, u32 arg1)
{
    register s32 mask1 asm("r2");
    register u32 shifted asm("r1");
    register s32 mask2 asm("r2");
    register u8 byte asm("r3");

    mask1 = 3;
    shifted = arg1 & mask1;
    shifted = shifted << 2;
    asm volatile("mov %0, #0xd\n\tneg %0, %0" : "=r"(mask2));
    byte = self[0x15];
    mask2 &= byte;
    mask2 |= shifted;
    self[0x15] = mask2;
}
