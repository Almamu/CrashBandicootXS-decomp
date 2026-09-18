#include "core.h"

/* Continuation of the fade_screen_mode.c cluster, right after the
 * parked sub_8001524 (asm/code_3_1_8.s) - see docs/matching.md for
 * why this cluster needed splitting into this many pieces.
 * `gUnknown_03001288` is a 2-byte packed mode/flags shadow copy of
 * `REG_DISPCNT`, committed to the real hardware register by
 * `sub_8001614`. */

extern u8 gUnknown_03001288[2];

/* Sets `gUnknown_03001288`'s low 3 bits (the DISPCNT background-mode
 * field) to `val & 7`, preserving the rest.
 *
 * Written as NAKED asm, not plain C: a full C reconstruction (kept in
 * git history) got the logic right, but this compiler recognizes `-8`
 * as reachable from the already-loaded `7` mask via a single `SUB`
 * (`7 - 15 = -8`) and folds the ROM's fresh `movs r1,#8; rsbs r1,r1,#0`
 * pair into that shorter subtract, regardless of how the constant is
 * spelled (`-8`, `~7`) or how many intervening register-pinned
 * temporaries separate the two uses of r1 - an unavoidable
 * value-propagation optimization. Every instruction below is confirmed
 * byte-identical to the ROM - full NAKED transcription, like this
 * project's other hard-compiler-limitation cases (see
 * `src/util/printf_util.c`'s `sub_8000CBC` for the established
 * pattern), is more honest than continuing to chase this one constant
 * through plain C. */
NAKED void sub_8001524(s32 val)
{
    asm(
        "ldr r2, 1f\n\t"
        "mov r1, #7\n\t"
        "and r0, r1\n\t"
        "mov r1, #8\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "bx lr\n\t"
        ".align 2, 0\n\t"
    "1: .4byte gUnknown_03001288\n\t"
    );
}
asm(".align 2, 0");

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
