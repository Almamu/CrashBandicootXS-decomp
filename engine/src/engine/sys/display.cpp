#include "engine/sys/display.hpp"

extern "C" {
#include "core.h"
#include "engine/gfx/gfx.h"
#include "globals.h"
}

using Engine::Sys::Display;

/* Continuation of the fade_to_black.cpp cluster - see docs/matching.md
 * for why this cluster needed splitting into this many pieces.
 * `dispcnt` is a 2-byte packed mode/flags shadow copy of
 * `REG_DISPCNT`, committed to the real hardware register by
 * `CommitDispcnt`. The blend registers' commit, `CommitBlendRegs`,
 * ends the file (it started util/aabb.cpp until #767). */

/* `dispcnt.bits`' field stores give the ROM's byte-wide
 * read-modify-writes; old_agbcc (Makefile) loads the mask before the
 * `ldrb`, as the ROM does. */

/* Sets `dispcnt`'s low 3 bits (the DISPCNT background-mode
 * field) to `val & 7`, preserving the rest. */
void Display::SetMode(s32 val)
{
    dispcnt.bits.mode = val;
}

/* DISPCNT bit 11 (byte 1, bit 3) clear/set pair (part of the packed
 * DISPCNT-mode shadow's second byte). */
void Display::HideBg3()
{
    dispcnt.bits.bg3 = 0;
}

/* DISPCNT bit 10 (byte 1, bit 2) clear. */
void Display::HideBg2()
{
    dispcnt.bits.bg2 = 0;
}

/* DISPCNT bit 9 (byte 1, bit 1) clear. */
void Display::HideBg1()
{
    dispcnt.bits.bg1 = 0;
}

/* DISPCNT bit 8 (byte 1, bit 0) clear. */
void Display::HideBg0()
{
    dispcnt.bits.bg0 = 0;
}

/* DISPCNT bit 12 (byte 1, bit 4) clear. */
void Display::HideObj()
{
    dispcnt.bits.obj = 0;
}

/* DISPCNT bit 11 (byte 1, bit 3) set. */
void Display::ShowBg3()
{
    dispcnt.bits.bg3 = 1;
}

/* DISPCNT bit 10 (byte 1, bit 2) set. */
void Display::ShowBg2()
{
    dispcnt.bits.bg2 = 1;
}

/* DISPCNT bit 9 (byte 1, bit 1) set. */
void Display::ShowBg1()
{
    dispcnt.bits.bg1 = 1;
}

/* DISPCNT bit 8 (byte 1, bit 0) set. */
void Display::ShowBg0()
{
    dispcnt.bits.bg0 = 1;
}

/* DISPCNT bit 12 (byte 1, bit 4) set. */
void Display::ShowObj()
{
    dispcnt.bits.obj = 1;
}

/* DISPCNT bit 6 clear (DISPCNT's own top mode bit). */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or engine/src/). */
void Display::SetObjMapping2D()
{
    dispcnt.bits.objMap1D = 0;
}

/* DISPCNT bit 6 set. */
void Display::SetObjMapping1D()
{
    dispcnt.bits.objMap1D = 1;
}

/* Commits the packed `dispcnt` shadow (both bytes, as one
 * halfword) straight to `REG_DISPCNT`. */
void Display::CommitDispcnt()
{
    *(vu16 *)REG_ADDR_DISPCNT = dispcnt.raw;
}

/* Commits the `blendRegs` shadow to the real blend registers:
 * the word at `+0` covers both `REG_BLDCNT` and `REG_BLDALPHA` (a
 * single 32-bit write spanning the adjacent halfwords), and the low
 * 5 bits of the byte at `+4` become `REG_BLDY`. The mask is the ROM's
 * `(x << 27) >> 27` shift pair, which a plain `& 0x1f` would encode as
 * an AND with a constant instead. Built with old_agbcc, which derives
 * the BLDY address from BLDCNT's (`adds r2, #4`) as the ROM does; agbcc
 * loads it from a second literal. */
void Display::CommitBlendRegs()
{
    /* `bldy` is needed: inlined, the instructions differ */
    u32 bldy;

    *(vu32 *)REG_ADDR_BLDCNT = blendRegs.blend.raw;
    bldy = blendRegs.bldy;
    REG_BLDY = (bldy << 27) >> 27;
}
