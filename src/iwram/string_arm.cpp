extern "C" {
#include "core.h"
#include "iwram.h"
}

/*
 * IWRAM 0x030000D4-0x03000198 (stored in ROM at 0x087E56B8): ARM copies
 * of the string helpers the Thumb code also has (src/util/string.cpp),
 * part of the IWRAM image crt0 copies to 0x03000000 at boot (see
 * src/iwram/iwram_data.cpp and docs/data.md). The ARM itoa that follows
 * them, itoa_arm, is assembly (asm/itoa_arm.s): its ROM code isn't gcc
 * output (#662).
 *
 * Built as ARM code (Makefile ARM_OBJS) from C++ with agbcp_arm_patched
 * and -mno-cond-return (docs/cplusplus.md, "The IWRAM ARM code").
 * strlen_arm, strcpy_arm and strcat_arm come out the same under stock
 * agbcc_arm's flags. strncpy_arm needs -mno-cond-return: the ROM's ARM
 * code makes no conditional returns. See its comment and
 * docs/matching/iwram-image.md.
 *
 * UNUSED - no caller anywhere in the ROM (checked: none of these five
 * addresses, itoa_arm's included, appears as a word in baserom.gba, and
 * ARM code can only be reached from Thumb through a pointer).
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
 * `c = 0; if (n != c) *dst = c;` makes the final test compare `n` with
 * the register just zeroed (`mov r3, #0; cmp r2, r3`) instead of the
 * immediate 0.
 *
 * The `n == 0` branch to the final `bx lr` stays a branch only under
 * -mno-cond-return (Makefile). Stock agbcc_arm's jump pass turns any
 * jump to a label followed by the return into a conditional return
 * (`bxeq lr`), gated only by arm.c's use_return_insn, which holds for
 * every frameless leaf with nothing saved, so no C shape avoids it; the
 * ROM's later ARM gcc never emits one. #662 rounds 3-4 needed an empty
 * asm barrier between that label and the return
 * (docs/matching/iwram-image.md, "Ninth step"). */
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
