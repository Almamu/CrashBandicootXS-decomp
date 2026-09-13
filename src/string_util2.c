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
s32 sub_8000D68(u8 *s)
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
