#include "core.h"

/* Continuation of the fade_screen_mode.c cluster - see docs/matching.md
 * for why this cluster needed splitting into this many pieces.
 * `gUnknown_03001288` is a 2-byte packed mode/flags shadow copy of
 * `REG_DISPCNT`, committed to the real hardware register by
 * `sub_8001614`. */

extern u8 gUnknown_03001288[2];

/* Sets `gUnknown_03001288`'s low 3 bits (the DISPCNT background-mode
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
void sub_8001524(s32 val)
{
    register u8 *addr asm("r2") = gUnknown_03001288;
    register s32 lowBits asm("r0") = val & 7;
    s32 mask;

    asm("mov %0, #8\n\tneg %0, %0" : "=r"(mask));
    addr[0] = (mask & addr[0]) | lowBits;
}

/* `gUnknown_03001288[1]` bit 3 clear/set pair (part of the packed
 * DISPCNT-mode shadow's second byte). */
void sub_800153C(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = -9;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask & byte;
    addr[1] = result;
}

/* `gUnknown_03001288[1]` bit 2 clear. */
void sub_8001550(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = -5;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask & byte;
    addr[1] = result;
}

/* `gUnknown_03001288[1]` bit 1 clear. */
void sub_8001564(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = -3;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask & byte;
    addr[1] = result;
}

/* `gUnknown_03001288[1]` bit 0 clear. */
void sub_8001578(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = -2;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask & byte;
    addr[1] = result;
}

/* `gUnknown_03001288[1]` bit 4 clear. */
void sub_800158C(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = -0x11;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask & byte;
    addr[1] = result;
}

/* `gUnknown_03001288[1]` bit 3 set. */
void sub_80015A0(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = 8;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask | byte;
    addr[1] = result;
}

/* `gUnknown_03001288[1]` bit 2 set. */
void sub_80015B0(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = 4;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask | byte;
    addr[1] = result;
}

/* `gUnknown_03001288[1]` bit 1 set. */
void sub_80015C0(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = 2;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask | byte;
    addr[1] = result;
}

/* `gUnknown_03001288[1]` bit 0 set. */
void sub_80015D0(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = 1;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask | byte;
    addr[1] = result;
}

/* `gUnknown_03001288[1]` bit 4 set. */
void sub_80015E0(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = 0x10;
    register s32 byte asm("r2") = addr[1];
    register s32 result asm("r0");

    result = mask | byte;
    addr[1] = result;
}

/* `gUnknown_03001288[0]` bit 6 clear (DISPCNT's own top mode bit). */
void sub_80015F0(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = -0x41;
    register s32 byte asm("r2") = addr[0];
    register s32 result asm("r0");

    result = mask & byte;
    addr[0] = result;
}

/* `gUnknown_03001288[0]` bit 6 set. */
void sub_8001604(void)
{
    register u8 *addr asm("r1") = gUnknown_03001288;
    register s32 mask asm("r0") = 0x40;
    register s32 byte asm("r2") = addr[0];
    register s32 result asm("r0");

    result = mask | byte;
    addr[0] = result;
}

/* Commits the packed `gUnknown_03001288` shadow (both bytes, as one
 * halfword) straight to `REG_DISPCNT`. */
void sub_8001614(void)
{
    *(vu16 *)REG_ADDR_DISPCNT = *(u16 *)gUnknown_03001288;
}
