#include "core.h"
#include "match.h"
#include "gfx.h"
#include "globals.h"

/* Continuation of the fade_to_black.c cluster - see docs/matching.md
 * for why this cluster needed splitting into this many pieces.
 * `gDispcnt` is a 2-byte packed mode/flags shadow copy of
 * `REG_DISPCNT`, committed to the real hardware register by
 * `CommitDispcnt`. */

/* Sets `gDispcnt`'s low 3 bits (the DISPCNT background-mode
 * field) to `val & 7`, preserving the rest.
 *
 * The ROM materializes `-8` fresh via `movs r1,#8; rsbs r1,r1,#0`
 * rather than deriving it from the already-loaded `7` mask via a
 * cheaper `SUB` - but this compiler's value-propagation pass always
 * takes the cheaper `SUB` once `7` has been loaded anywhere nearby, no
 * matter how the `-8`/`~7` constant is spelled in C. The fix is to
 * never let the mask exist as a C-level constant at all: an inline-asm
 * block computes it via the exact two-instruction ROM sequence, opaque
 * to the optimizer, which reproduces the ROM's own choice instead of
 * outsmarting it. */
void SetDispcntMode(s32 val)
{
    MATCH_HOLD_REG(u8 *, addr, r2) = gDispcnt;
    MATCH_HOLD_REG(s32, lowBits, r0) = val & 7;
    s32 mask;

    asm("mov %0, #8\n\tneg %0, %0" : "=r"(mask));
    addr[0] = (mask & addr[0]) | lowBits;
}

/* `gDispcnt[1]` bit 3 clear/set pair (part of the packed
 * DISPCNT-mode shadow's second byte). */
void HideBg3(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = -9;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask & byte;
    addr[1] = result;
}

/* `gDispcnt[1]` bit 2 clear. */
void HideBg2(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = -5;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask & byte;
    addr[1] = result;
}

/* `gDispcnt[1]` bit 1 clear. */
void HideBg1(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = -3;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask & byte;
    addr[1] = result;
}

/* `gDispcnt[1]` bit 0 clear. */
void HideBg0(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = -2;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask & byte;
    addr[1] = result;
}

/* `gDispcnt[1]` bit 4 clear. */
void HideObj(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = -0x11;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask & byte;
    addr[1] = result;
}

/* `gDispcnt[1]` bit 3 set. */
void ShowBg3(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = 8;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask | byte;
    addr[1] = result;
}

/* `gDispcnt[1]` bit 2 set. */
void ShowBg2(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = 4;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask | byte;
    addr[1] = result;
}

/* `gDispcnt[1]` bit 1 set. */
void ShowBg1(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = 2;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask | byte;
    addr[1] = result;
}

/* `gDispcnt[1]` bit 0 set. */
void ShowBg0(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = 1;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask | byte;
    addr[1] = result;
}

/* `gDispcnt[1]` bit 4 set. */
void ShowObj(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = 0x10;
    MATCH_HOLD_REG(s32, byte, r2) = addr[1];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask | byte;
    addr[1] = result;
}

/* `gDispcnt[0]` bit 6 clear (DISPCNT's own top mode bit). */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void SetObjMapping2D(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = -0x41;
    MATCH_HOLD_REG(s32, byte, r2) = addr[0];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask & byte;
    addr[0] = result;
}

/* `gDispcnt[0]` bit 6 set. */
void SetObjMapping1D(void)
{
    MATCH_HOLD_REG(u8 *, addr, r1) = gDispcnt;
    MATCH_HOLD_REG(s32, mask, r0) = 0x40;
    MATCH_HOLD_REG(s32, byte, r2) = addr[0];
    MATCH_HOLD_REG(s32, result, r0);

    result = mask | byte;
    addr[0] = result;
}

/* Commits the packed `gDispcnt` shadow (both bytes, as one
 * halfword) straight to `REG_DISPCNT`. */
void CommitDispcnt(void)
{
    *(vu16 *)REG_ADDR_DISPCNT = *(u16 *)gDispcnt;
}
