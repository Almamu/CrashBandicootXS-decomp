#ifndef __AGB_SYSCALL_H__
#define __AGB_SYSCALL_H__

#include "gba/types.h"

/* The GBA BIOS SWI wrappers of lib/libagbsyscall (libagbsyscall.s), each a
 * bare `svc #N; bx lr`. The argument and return types are the ones the
 * game's callers use (they decide the callers' code, the wrappers don't
 * care). */

/* BgAffineSet's source record (the SDK's BgAffineSrcData): the texture
 * point (8.8 fixed point) shown at the screen point, the scale and the
 * angle. Its size, 0x14, is gcc's rounding of the struct to 4 bytes. */
struct bg_affine_src {
    s32 texX;
    s32 texY;
    s16 scrX;
    s16 scrY;
    s16 sx; // 8.8
    s16 sy;
    u16 angle;
};

/* ObjAffineSet's source record (the SDK's ObjAffineSrcData), 8 bytes
 * the same way. */
struct obj_affine_src {
    s16 sx; // 8.8
    s16 sy;
    u16 angle;
};

/* SWI 0xE: `count` BG affine parameter sets from `src` to `dst`. */
void BgAffineSet(void *src, void *dst, s32 count);

/* SWI 0xC: 32-byte-block copy/fill; `control` = word count | flags
 * (bit 24: fill from a fixed source word). */
void CpuFastSet(const void *src, void *dst, u32 control);

/* SWI 0xB: halfword/word copy/fill; `control` = unit count | flags
 * (bit 24: fixed source, bit 26: 32-bit units). */
void CpuSet(const void *src, void *dst, u32 control);

/* SWI 0x12/0x15: LZ77/run-length decompression into VRAM (16-bit
 * writes). src/system/asset.cpp declares them with `src` alone: its
 * callers leave `dst` in r1 from their own argument. */
void LZ77UnCompVram(const void *src, void *dst);
void RLUnCompVram(const void *src, void *dst);

/* SWI 0xF: `count` OBJ affine parameter sets from `src`, written to `dst`
 * every `offset` bytes. */
void ObjAffineSet(void *src, void *dst, s32 count, s32 offset);

/* SWI 8: integer square root. */
s32 Sqrt(s32 num);

/* SWI 5: halts until the next VBlank interrupt (r2 = 0 first). */
void VBlankIntrWait(void);

#endif /* __AGB_SYSCALL_H__ */
