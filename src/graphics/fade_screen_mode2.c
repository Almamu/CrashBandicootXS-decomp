#include "core.h"

/* Continuation of the fade_screen_mode.c cluster, right after the
 * parked sub_8001524 (asm/code_3_1_8.s) - see docs/matching.md for
 * why this cluster needed splitting into this many pieces.
 * `gUnknown_03001288` is a 2-byte packed mode/flags shadow copy of
 * `REG_DISPCNT`, committed to the real hardware register by
 * `sub_8001614`. */

extern u8 gUnknown_03001288[2];

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
