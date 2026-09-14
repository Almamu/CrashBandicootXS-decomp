#include "core.h"

/* Sits right after sub_8000CBC (ROM 0x08000CBC), which is not yet
 * byte-matching and is still parked in asm/code_3_1_2.s - kept in its
 * own translation unit (rather than string_util.c, thematically the
 * better fit) purely so ldscript.txt can place this one function's
 * object between code_3_1_2.o and code_3_1_3.o and preserve ROM address
 * order. */

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

/* Trailing padding (see matching_decomp_alignment_fix memory). */
asm(".align 2, 0");

/* strcat: appends src to the end of dst (in place), NUL-terminating
 * the result. `p`/`i` are pinned to r3/r2 to match the ROM, which
 * finds the end of dst via an index (`p[i]`) rather than walking a
 * pointer; the pointer computed from `p + i` is then a *separate*
 * variable (`q`, also pinned to r2 - the ROM lets `i`'s register go
 * dead and reuses it, rather than writing the sum back into `p`'s r3)
 * used for the rest of the copy loop. All plain scratch here (leaf
 * function, no calls). */
void strcat(u8 *dst, u8 *src)
{
    register u8 *p asm("r3");
    register s32 i asm("r2");
    i = 0;
    p = dst;
    if (p[i] != 0) {
        do {
            i++;
        } while (p[i] != 0);
    }
    {
        register u8 *q asm("r2");
        q = p + i;
        while (*src != 0) {
            *q = *src;
            src++;
            q++;
        }
        *q = 0;
    }
}

/* Trailing padding (see matching_decomp_alignment_fix memory). */
asm(".align 2, 0");

/* strncpy: copies at most n bytes from src into dst, stopping early at
 * src's NUL terminator; NUL-terminates dst only if fewer than n bytes
 * were actually copied from src (real strncpy always pads dst to n
 * bytes - this doesn't). */
void sub_8000DAC(u8 *dst, u8 *src, s32 n)
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

/* strcpy. */
void strcpy(u8 *dst, u8 *src)
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

/* Trailing padding (see matching_decomp_alignment_fix memory). */
asm(".align 2, 0");

/* strlen. Had no thumb_func_start label of its own in the original raw
 * asm/code_3_1_3.s (unlike every other function extracted so far) -
 * confirmed via a direct baserom.gba objdump that it's real code at
 * ROM 0x08000DF8, immediately after strcpy's own trailing pad NOP,
 * not data or padding; likely just never called via `bl` from anything
 * disassembled yet, so whatever tool originally split this file didn't
 * detect a boundary here. */
s32 strlen(u8 *s)
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
