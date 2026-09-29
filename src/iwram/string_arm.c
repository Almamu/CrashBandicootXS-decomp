#include "core.h"

/*
 * IWRAM 0x030000D4-0x0300024C (stored in ROM at 0x087E56B8): ARM copies
 * of the string helpers the Thumb code also has (src/util/string_util.c,
 * src/util/string_util2.c), part of the IWRAM image crt0 copies to
 * 0x03000000 at boot (see src/iwram/iwram_data.c and docs/data.md).
 *
 * Built as ARM code with agbcc_arm (Makefile ARM_OBJS). strlen_arm,
 * strcpy_arm and strcat_arm match as plain C. strncpy_arm and itoa_arm
 * are parked: the ROM was built by an ARM gcc whose output differs from
 * agbcc_arm's (gcc-2.9-arm-000512) in ways C can't reach - see each
 * function's comment and docs/matching/iwram-image.md.
 *
 * UNUSED - no caller anywhere in the ROM (checked: none of these five
 * addresses appears as a word in baserom.gba, and ARM code can only be
 * reached from Thumb through a pointer).
 */

/* strlen. UNUSED - see the file comment. */
s32 strlen_arm(u8 *s)
{
    s32 i = 0;

    while (s[i] != 0)
        i++;
    return i;
}

/* strcpy; the terminator is stored from the byte that ended the loop.
 * UNUSED - see the file comment. */
void strcpy_arm(u8 *dst, u8 *src)
{
    u8 c;

    while ((c = *src) != 0) {
        *dst++ = c;
        src++;
    }
    *dst = c;
}

#if NON_MATCHING
/* strncpy without the zero padding: copies at most n bytes, and
 * NUL-terminates only if fewer than n were copied. UNUSED - see the
 * file comment.
 *
 * Parked: agbcc_arm turns the `n == 0` early exit into a conditional
 * return (`bxeq lr`), while the ROM branches to its one `bx lr` at the
 * end, and it compares the final `n` against the immediate 0 where the
 * ROM compares it against the register it has just zeroed. Every C
 * phrasing tried (early return, one guarded block, an explicit zero
 * variable) gives the same conditional return: agbcc_arm emits one for
 * any leaf function with nothing to restore, and the ROM's compiler
 * never does (it never uses a conditional return anywhere in the IWRAM
 * image). */
void strncpy_arm(u8 *dst, u8 *src, s32 n)
{
    u8 c;

    if (n == 0)
        return;
    while ((c = *src) != 0) {
        *dst++ = c;
        src++;
        if (--n == 0)
            break;
    }
    if (n != 0)
        *dst = 0;
}
#else
NAKED void strncpy_arm(u8 *dst, u8 *src, s32 n)
{
    asm(".syntax unified\n"
        "\tcmp r2, #0\n"
        "\tbeq .L03000158\n"
        "\tldrb r3, [r1]\n"
        "\tcmp r3, #0\n"
        "\tbeq .L0300014C\n"
        ".L03000134:\n"
        "\tstrb r3, [r0], #1\n"
        "\tsubs r2, r2, #1\n"
        "\tbeq .L0300014C\n"
        "\tldrb r3, [r1, #1]!\n"
        "\tcmp r3, #0\n"
        "\tbne .L03000134\n"
        ".L0300014C:\n"
        "\tmov r3, #0\n"
        "\tcmp r2, r3\n"
        "\tstrbne r3, [r0]\n"
        ".L03000158:\n"
        "\tbx lr\n"
        ".syntax divided\n");
}
#endif

/* strcat. UNUSED - see the file comment. */
void strcat_arm(u8 *dst, u8 *src)
{
    u8 c;

    while (*dst != 0)
        dst++;
    while ((c = *src) != 0) {
        *dst++ = c;
        src++;
    }
    *dst = c;
}

#if NON_MATCHING
/* itoa: writes `value` in `base` to `buf` (upper-case hex digits, a
 * leading '-' for negative values), NUL-terminates it and returns its
 * length. Base 16 uses shifts; any other base divides with the BIOS Div
 * SWI (`swi 0x60000` in ARM state: r0 = quotient, r1 = remainder, r3
 * clobbered). UNUSED - see the file comment.
 *
 * Parked. The control flow and every instruction outside register
 * choice match, but the ROM keeps `base` in ip and `buf`/`len`/`neg` in
 * r6/r5/r4 and never touches lr, while agbcc_arm puts one of those
 * values in lr (it allocates lr before r4). A "lr" clobber on the SWI
 * keeps lr free but then the prologue saves lr, which the ROM doesn't.
 * The ROM's hex test is also `cmp r1, #10` + `addge`/`addlt`, where
 * agbcc_arm always canonicalizes `digit < 10` to `cmp r1, #9` +
 * `addgt`/`addle`. Both look like the same compiler difference as
 * strncpy_arm's. */
s32 itoa_arm(s32 value, u8 *buf, s32 base)
{
    register s32 num asm("r0");
    register s32 digit asm("r1");
    s32 len = 0;
    s32 neg;
    s32 i, j;

    num = value;
    if (num < 0) {
        neg = 1;
        num = -num;
    } else {
        neg = 0;
    }
    if (base != 16) {
        do {
            digit = base;
            asm("swi 0x60000" : "=r"(num), "=r"(digit) : "0"(num), "1"(digit) : "r3");
            buf[len] = digit + '0';
            len++;
        } while (num != 0);
    } else {
        do {
            digit = num & 15;
            num >>= 4;
            if (digit < 10)
                digit += '0';
            else
                digit += 'A' - 10;
            buf[len] = digit;
            len++;
        } while (num != 0);
    }
    if (neg) {
        buf[len] = '-';
        len++;
    }
    buf[len] = 0;
    i = 0;
    j = len - 1;
    do {
        u8 t = buf[i];
        buf[i] = buf[j];
        buf[j] = t;
        i++;
        j--;
    } while (i < j);
    return len;
}
#else
NAKED s32 itoa_arm(s32 value, u8 *buf, s32 base)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6}\n"
        "\tmov r5, #0\n"
        "\tcmp r0, #0\n"
        "\tmovge r4, #0\n"
        "\tmovlt r4, #1\n"
        "\trsblt r0, r0, #0\n"
        "\tmov r6, r1\n"
        "\tmov r12, r2\n"
        "\tcmp r2, #16\n"
        "\tbeq .L030001E0\n"
        ".L030001C0:\n"
        "\tmov r1, r12\n"
        "\tsvc 0x00060000\n"
        "\tadd r1, r1, #48\n"
        "\tstrb r1, [r6, r5]\n"
        "\tadd r5, r5, #1\n"
        "\tcmp r0, #0\n"
        "\tbne .L030001C0\n"
        "\tb .L03000204\n"
        ".L030001E0:\n"
        "\tand r1, r0, #15\n"
        "\tasr r0, r0, #4\n"
        "\tcmp r1, #10\n"
        "\taddge r1, r1, #55\n"
        "\taddlt r1, r1, #48\n"
        "\tstrb r1, [r6, r5]\n"
        "\tadd r5, r5, #1\n"
        "\tcmp r0, #0\n"
        "\tbne .L030001E0\n"
        ".L03000204:\n"
        "\tcmp r4, #0\n"
        "\tmovne r4, #45\n"
        "\tstrbne r4, [r6, r5]\n"
        "\taddne r5, r5, #1\n"
        "\tmovne r4, #0\n"
        "\tstrb r4, [r6, r5]\n"
        "\tsub r1, r5, #1\n"
        ".L03000220:\n"
        "\tldrb r3, [r6, r4]\n"
        "\tldrb r0, [r6, r1]\n"
        "\tstrb r3, [r6, r1]\n"
        "\tstrb r0, [r6, r4]\n"
        "\tadd r4, r4, #1\n"
        "\tsub r1, r1, #1\n"
        "\tcmp r4, r1\n"
        "\tblt .L03000220\n"
        "\tmov r0, r5\n"
        "\tpop {r4, r5, r6}\n"
        "\tbx lr\n"
        ".syntax divided\n");
}
#endif
