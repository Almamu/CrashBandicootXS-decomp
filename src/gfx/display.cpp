extern "C" {
#include "core.h"
#include "gfx.h"
#include "globals.h"
}

/* Continuation of the fade_to_black.cpp cluster - see docs/matching.md
 * for why this cluster needed splitting into this many pieces.
 * `gDispcnt` is a 2-byte packed mode/flags shadow copy of
 * `REG_DISPCNT`, committed to the real hardware register by
 * `CommitDispcnt`. The blend registers' commit, `CommitBlendRegs`,
 * ends the file (it started util/aabb.cpp until #767). */

/* `gDispcnt.bits`' field stores give the ROM's byte-wide
 * read-modify-writes; old_agbcc (Makefile) loads the mask before the
 * `ldrb`, as the ROM does. */

/* Sets `gDispcnt`'s low 3 bits (the DISPCNT background-mode
 * field) to `val & 7`, preserving the rest. */
void SetDispcntMode(s32 val)
{
    gDispcnt.bits.mode = val;
}

/* DISPCNT bit 11 (byte 1, bit 3) clear/set pair (part of the packed
 * DISPCNT-mode shadow's second byte). */
void HideBg3(void)
{
    gDispcnt.bits.bg3 = 0;
}

/* DISPCNT bit 10 (byte 1, bit 2) clear. */
void HideBg2(void)
{
    gDispcnt.bits.bg2 = 0;
}

/* DISPCNT bit 9 (byte 1, bit 1) clear. */
void HideBg1(void)
{
    gDispcnt.bits.bg1 = 0;
}

/* DISPCNT bit 8 (byte 1, bit 0) clear. */
void HideBg0(void)
{
    gDispcnt.bits.bg0 = 0;
}

/* DISPCNT bit 12 (byte 1, bit 4) clear. */
void HideObj(void)
{
    gDispcnt.bits.obj = 0;
}

/* DISPCNT bit 11 (byte 1, bit 3) set. */
void ShowBg3(void)
{
    gDispcnt.bits.bg3 = 1;
}

/* DISPCNT bit 10 (byte 1, bit 2) set. */
void ShowBg2(void)
{
    gDispcnt.bits.bg2 = 1;
}

/* DISPCNT bit 9 (byte 1, bit 1) set. */
void ShowBg1(void)
{
    gDispcnt.bits.bg1 = 1;
}

/* DISPCNT bit 8 (byte 1, bit 0) set. */
void ShowBg0(void)
{
    gDispcnt.bits.bg0 = 1;
}

/* DISPCNT bit 12 (byte 1, bit 4) set. */
void ShowObj(void)
{
    gDispcnt.bits.obj = 1;
}

/* DISPCNT bit 6 clear (DISPCNT's own top mode bit). */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void SetObjMapping2D(void)
{
    gDispcnt.bits.objMap1D = 0;
}

/* DISPCNT bit 6 set. */
void SetObjMapping1D(void)
{
    gDispcnt.bits.objMap1D = 1;
}

/* Commits the packed `gDispcnt` shadow (both bytes, as one
 * halfword) straight to `REG_DISPCNT`. */
void CommitDispcnt(void)
{
    *(vu16 *)REG_ADDR_DISPCNT = gDispcnt.raw;
}

/* Commits the `gBlendRegs` shadow to the real blend registers:
 * the word at `+0` covers both `REG_BLDCNT` and `REG_BLDALPHA` (a
 * single 32-bit write spanning the adjacent halfwords), and the low
 * 5 bits of the byte at `+4` become `REG_BLDY`. The mask is the ROM's
 * `(x << 27) >> 27` shift pair, which a plain `& 0x1f` would encode as
 * an AND with a constant instead. Built with old_agbcc, which derives
 * the BLDY address from BLDCNT's (`adds r2, #4`) as the ROM does; agbcc
 * loads it from a second literal. */
void CommitBlendRegs(void)
{
    u32 bldy;

    *(vu32 *)REG_ADDR_BLDCNT = gBlendRegs.blend.raw;
    bldy = gBlendRegs.bldy;
    REG_BLDY = (bldy << 27) >> 27;
}
