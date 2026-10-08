extern "C" {
#include "core.h"
#include "gfx.h"
#include "globals.h"
}

/* Continuation of the fade_to_black.cpp cluster - see docs/matching.md
 * for why this cluster needed splitting into this many pieces.
 * `gDispcnt` is a 2-byte packed mode/flags shadow copy of
 * `REG_DISPCNT`, committed to the real hardware register by
 * `CommitDispcnt`. */

/* `gDispcnt` viewed as its REG_DISPCNT bitfields. The field stores give
 * the ROM's byte-wide read-modify-writes; old_agbcc (Makefile) loads the
 * mask before the `ldrb`, as the ROM does. */
#define DISPCNT_BITS ((struct dispcnt_bits *)gDispcnt)

/* Sets `gDispcnt`'s low 3 bits (the DISPCNT background-mode
 * field) to `val & 7`, preserving the rest. */
void SetDispcntMode(s32 val)
{
    DISPCNT_BITS->mode = val;
}

/* `gDispcnt[1]` bit 3 clear/set pair (part of the packed
 * DISPCNT-mode shadow's second byte). */
void HideBg3(void)
{
    DISPCNT_BITS->bg3 = 0;
}

/* `gDispcnt[1]` bit 2 clear. */
void HideBg2(void)
{
    DISPCNT_BITS->bg2 = 0;
}

/* `gDispcnt[1]` bit 1 clear. */
void HideBg1(void)
{
    DISPCNT_BITS->bg1 = 0;
}

/* `gDispcnt[1]` bit 0 clear. */
void HideBg0(void)
{
    DISPCNT_BITS->bg0 = 0;
}

/* `gDispcnt[1]` bit 4 clear. */
void HideObj(void)
{
    DISPCNT_BITS->obj = 0;
}

/* `gDispcnt[1]` bit 3 set. */
void ShowBg3(void)
{
    DISPCNT_BITS->bg3 = 1;
}

/* `gDispcnt[1]` bit 2 set. */
void ShowBg2(void)
{
    DISPCNT_BITS->bg2 = 1;
}

/* `gDispcnt[1]` bit 1 set. */
void ShowBg1(void)
{
    DISPCNT_BITS->bg1 = 1;
}

/* `gDispcnt[1]` bit 0 set. */
void ShowBg0(void)
{
    DISPCNT_BITS->bg0 = 1;
}

/* `gDispcnt[1]` bit 4 set. */
void ShowObj(void)
{
    DISPCNT_BITS->obj = 1;
}

/* `gDispcnt[0]` bit 6 clear (DISPCNT's own top mode bit). */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void SetObjMapping2D(void)
{
    DISPCNT_BITS->objMap1D = 0;
}

/* `gDispcnt[0]` bit 6 set. */
void SetObjMapping1D(void)
{
    DISPCNT_BITS->objMap1D = 1;
}

/* Commits the packed `gDispcnt` shadow (both bytes, as one
 * halfword) straight to `REG_DISPCNT`. */
void CommitDispcnt(void)
{
    *(vu16 *)REG_ADDR_DISPCNT = *(u16 *)gDispcnt;
}
