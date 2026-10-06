#include "core.h"
#include "match.h"
#include "iwram.h"

/*
 * IWRAM 0x030000D4-0x0300024C (stored in ROM at 0x087E56B8): ARM copies
 * of the string helpers the Thumb code also has (src/util/number_format.c,
 * src/util/string.c), part of the IWRAM image crt0 copies to
 * 0x03000000 at boot (see src/iwram/iwram_data.c and docs/data.md).
 *
 * Built as ARM code with agbcc_arm (Makefile ARM_OBJS). strlen_arm,
 * strcpy_arm, strncpy_arm and strcat_arm match as C. itoa_arm is parked:
 * the ROM was built by an ARM gcc whose prologue differs from agbcc_arm's
 * (gcc-2.9-arm-000512) in a way C can't reach - see its comment and
 * docs/matching/iwram-image.md.
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

/* strncpy without the zero padding: copies at most n bytes, and
 * NUL-terminates only if fewer than n were copied. UNUSED - see the
 * file comment.
 *
 * Two details reproduce the ROM's code:
 * - `c = 0; if (n != c) *dst = c;` makes the final test compare `n`
 *   with the register just zeroed (`mov r3, #0; cmp r2, r3`) instead of
 *   the immediate 0.
 * - The empty asm at the end emits no code. Without it agbcc_arm turns
 *   the `n == 0` branch to the final `bx lr` into a conditional return
 *   (`bxeq lr`): its jump pass rewrites any jump whose target is directly
 *   followed by a return (jump.c, "turn it into a RETURN insn").
 *   The asm sits between that label and the return, so the branch
 *   stays. The ROM's ARM compiler never emits a conditional return (see
 *   docs/matching/iwram-image.md). */
void strncpy_arm(u8 *dst, u8 *src, s32 n)
{
    u8 c;

    if (n != 0) {
        c = *src;
        if (c != 0) {
            do {
                *dst++ = c;
                if (--n == 0)
                    break;
                c = *++src;
            } while (c != 0);
        }
        c = 0;
        if (n != c)
            *dst = c;
    }
    MATCH_BARRIER();
}

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
 * Parked, and out of agbcc_arm's reach: the ROM saves r4-r6 with
 * `push {r4, r5, r6}` and returns with `pop {r4, r5, r6}; bx lr`,
 * without saving lr. agbcc_arm's arm_expand_prologue adds lr to every
 * register push ("If we have to push any regs, then we must push lr as
 * well"), so no C makes it push r4-r6 without lr. Other differences: the
 * ROM never uses lr (base in ip, buf/len/neg in r6/r5/r4), and its hex
 * test is `cmp r1, #10` + `addge`/`addlt`. agbcc_arm's fold-const.c
 * rewrites `digit < 10` to `digit <= 9` (`cmp r1, #9`), and combine turns
 * the `+ '0'` into `orr`. */
s32 itoa_arm(s32 value, u8 *buf, s32 base)
{
    MATCH_HOLD_REG(s32, num, r0);
    MATCH_HOLD_REG(s32, digit, r1);
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
