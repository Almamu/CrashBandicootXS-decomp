#include "core.h"

/* Sits right after sub_80011C0 (ROM 0x080011C0, in src/asset_util.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */

/* Returns the length of the next "word" starting at `s`: the number of
 * characters up to and including the first space, or up to (but not
 * including) the NUL terminator if no space is found first. Used by
 * the still-parked sub_8000EE4 (src/text_layout.c) to walk text one
 * token at a time. */
s32 sub_80011F4(u8 *s)
{
    s32 len = 0;
    u8 c;

    c = *s;
    if (c == 0) {
        goto done;
    }
    len = 1;
    if (c == ' ') {
        goto done;
    }
    for (;;) {
        s++;
        c = *s;
        if (c == 0) {
            goto done;
        }
        len++;
        if (c == ' ') {
            goto done;
        }
    }
done:
    return len;
}
