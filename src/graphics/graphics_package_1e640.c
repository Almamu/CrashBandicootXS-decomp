#include "core.h"

/* Reads back the packed-BG-control halfword (bits later written by
 * `sub_801E644`'s `self+0xc`/`self+0xd` pair) from the small graphics-
 * package "self" scratch buffer that `LoadGraphicsPackage`'s callers
 * build up before calling it (see `sub_80374D0` in
 * counter_selector_setup.c: `u8 buf[0x10]; sub_801E644(buf, ...);
 * LoadGraphicsPackage(buf, ...); REG_BG0CNT = sub_801E640(buf);` - the
 * buffer is treated as a raw byte scratch area by every caller, never a
 * named struct, so it stays `u8 *` here too). */
u16 sub_801E640(u8 *self)
{
    return *(u16 *)(self + 0xc);
}

#if NON_MATCHING
/* Writes the same "self" scratch buffer's first four fields: three raw
 * 32-bit values at +0x00/+0x04/+0x08 (`arg1`/`arg2`/`arg3` verbatim -
 * `sub_80374D0` calls this as `sub_801E644(buf, 2, 0x1e, 1, 3)`, so
 * these look like a level/category index, a BG control value, and a
 * priority/slot index, though the exact meaning isn't pinned down
 * beyond their offsets) plus two packed sub-byte fields: byte +0xc's
 * low nibble is `(arg5 & 3) | ((arg1 & 3) << 2)`, byte +0xd is
 * `arg2 & 0x1f`. Every instruction's operation is confirmed against
 * the ROM (isolated compile + manual diff) and every technique in
 * `docs/matching_decomp_register_pinning` (explicit local copies of
 * each mask constant, matching read/mask/store order statement-by-
 * statement) was tried; what resists matching is purely a handful of
 * gcc 2.9 register-allocation choices around the repeated `& 3`/`-0xd`
 * masks (the ROM keeps a dedicated register for the shared `3`
 * constant across both uses and reads the to-be-updated byte through
 * yet another dedicated register that this compiler collapses away in
 * every C phrasing tried), plus one extra callee-saved register (`r7`)
 * in the ROM's push/pop list that no reachable C shape reproduced
 * without introducing its own further mismatches elsewhere. Parked
 * rather than keep guessing - see
 * docs/matching/issue-30-graphics-loading.md. */
void sub_801E644(u8 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg5)
{
    s32 b;
    s32 mask;

    *(u16 *)(self + 0xc) = 0;
    b = self[0xc] & -4;
    b |= arg5 & 3;
    *(u32 *)(self + 0) = arg1;
    mask = -0xd;
    b &= mask;
    b |= (arg1 & 3) << 2;
    self[0xc] = b;

    b = 0x3f & self[0xd];
    *(u32 *)(self + 4) = arg2;
    b &= -0x20;
    b |= arg2 & 0x1f;
    self[0xd] = b;
    *(u32 *)(self + 8) = arg3;
}
#endif /* NON_MATCHING */
