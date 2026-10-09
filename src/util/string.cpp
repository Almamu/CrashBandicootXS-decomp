/* The libc-style string functions: FindSubstring (strstr), CountNonSpaceChars,
 * strcat, strncpy, CopyString, StringLength. FindSubstring moved here from
 * printf.cpp (#767; both default flags). */

extern "C" {
#include "core.h"
#include "util.h"
}

/* ASCII lowercase. Written with one result variable that the range
 * test also uses, so jump.c can't turn the if/else into a conditional
 * move (`r = c; if (...) r = c + 0x20;`): the ROM keeps the copy of `c`
 * in both arms and truncates after the join. */
static inline u32 ToLower(u32 c)
{
    u32 r = (u8)(c - 'A');

    if (r <= 25)
        r = c + 0x20;
    else
        r = c;
    return (u8)r;
}

/* strstr, with optional case-insensitive matching (`caseInsensitive`
 * nonzero lowercases both sides before comparing): returns a pointer
 * into `str` at the first occurrence of `pattern`, or `0` if there is
 * none or `pattern` is empty. It sits immediately after sprintf in the
 * ROM, so it starts this file (it was at the end of printf.cpp until #767).
 *
 * #662 round 3 replaced the opaque lowercase asm blocks and the register
 * pins: ToLower's shared result variable keeps both arms of its test
 * (see above); one variable `a` for the scanned haystack byte and the
 * pattern byte puts both in r3, as in the ROM; and copying the
 * parameters into locals, `needle` first, gives the ROM's prologue
 * order (`r7 = r2; ip = r1; r5 = r0`). */
u8 *FindSubstring(u8 *str, u8 *pattern, s32 caseInsensitive)
{
    u8 *needle = pattern;
    u8 *haystack = str;
    u32 first = *needle++;
    u32 a;
    u32 b;
    u8 *h;
    u8 *n;

    if (first == 0)
        return 0;
    if (caseInsensitive != 0)
        first = ToLower(first);
    for (;;) {
        a = *haystack++;
        if (caseInsensitive != 0)
            a = ToLower(a);
        if (a != first) {
            if (a == 0)
                return 0;
        } else {
            h = haystack;
            n = needle;
            do {
                a = *n++;
                if (a == 0)
                    goto found;
                b = *h++;
                if (caseInsensitive != 0) {
                    a = ToLower(a);
                    b = ToLower(b);
                }
            } while (a == b);
        }
    }
found:
    return haystack - 1;
}

/* Counts the non-space characters in a NUL-terminated string (spaces
 * are skipped, not counted; every other byte, including the
 * terminator's, is). */
s32 CountNonSpaceChars(u8 *s)
{
    s32 count = 0;
    u8 c;

    while ((c = *s) != 0) {
        s++;
        if (c != ' ') {
            count++;
        }
    }
    return count;
}

/* strcat: appends src to the end of dst (in place), NUL-terminating
 * the result. Like the ROM, it finds the end of dst via an index
 * (`dst[i]`) rather than walking a pointer; the pointer computed from
 * `dst + i` is then a *separate* variable (`q`, which takes `i`'s r2
 * once `i` is dead, rather than the sum being written back into `dst`'s
 * r3) used for the rest of the copy loop. */
void strcat(u8 *dst, u8 *src)
{
    s32 i = 0;

    if (dst[i] != 0) {
        do {
            i++;
        } while (dst[i] != 0);
    }
    {
        u8 *q = dst + i;

        while (*src != 0) {
            *q = *src;
            src++;
            q++;
        }
        *q = 0;
    }
}

/* strncpy: copies at most n bytes from src into dst, stopping early at
 * src's NUL terminator; NUL-terminates dst only if fewer than n bytes
 * were actually copied from src (real strncpy always pads dst to n
 * bytes - this doesn't). */
void strncpy(u8 *dst, u8 *src, s32 n)
{
    u8 c = *src;
    if (c != 0) {
        n--;
        if (n != -1) {
            do {
                *dst = c;
                src++;
                dst++;
                c = *src;
                if (c == 0) {
                    break;
                }
                n--;
            } while (n != -1);
        }
    }
    if (n != 0) {
        *dst = 0;
    }
}

/* strcpy and strlen below keep their ROM symbol names through asm
 * labels. Defined under those C names, their `u8 *` signatures conflict
 * with gcc's built-in strcpy/strlen and warn. The C names differ, the
 * symbols and code don't. */
void CopyString(u8 *dst, u8 *src) asm("strcpy");
s32 StringLength(u8 *s) asm("strlen");

/* strcpy. */
void CopyString(u8 *dst, u8 *src)
{
    u8 *p = dst;
    u8 c;
    while ((c = *src) != 0) {
        *p = c;
        src++;
        p++;
    }
    *p = 0;
}

/* strlen. Had no thumb_func_start label of its own in the original raw
 * asm/code_3_1_3.s (unlike every other function extracted so far) -
 * confirmed via a direct baserom.gba objdump that it's real code at
 * ROM 0x08000DF8, immediately after strcpy's own trailing pad NOP,
 * not data or padding; likely just never called via `bl` from anything
 * disassembled yet, so whatever tool originally split this file didn't
 * detect a boundary here. */
s32 StringLength(u8 *s)
{
    u8 *p = s;
    s32 i = 0;
    if (p[i] != 0) {
        do {
            i++;
        } while (p[i] != 0);
    }
    return i;
}
