#include "core.h"
#include "line_util.h"

/* Sits right after sub_8001214 (ROM 0x08001214, in src/util/word_util.c)
 * and before sub_80012AC (still raw in asm/code_3_1_7.s). Not adjacent
 * to sub_8000E6C (src/util/line_util.c) in ROM address order - kept in its
 * own file purely because that file's object already links much
 * earlier; both share the struct definition from include/line_util.h. */

/* Advances a Bresenham line (set up by sub_8000E6C) by one step: the
 * "driving" axis (x if `flag` is set, y otherwise) always advances by
 * its sign; the other axis advances only when the accumulated error
 * term is positive, in which case the error term is corrected by
 * `field_18` instead of `field_14`. The error term is re-read from
 * `line` fresh inside each of the two `flag` branches (not hoisted
 * above the branch), matching the ROM's own two separate reloads. */
void sub_8001254(struct bresenham_line *line)
{
    s32 err;

    if (line->flag != 0) {
        err = line->field_10;
        if (err <= 0) {
            line->field_10 = err + line->field_14;
        } else {
            line->field_10 = err + line->field_18;
            line->y0 += line->sy;
        }
        line->x0 += line->sx;
    } else {
        err = line->field_10;
        if (err <= 0) {
            line->field_10 = err + line->field_14;
        } else {
            line->field_10 = err + line->field_18;
            line->x0 += line->sx;
        }
        line->y0 += line->sy;
    }
}

/* Trailing padding (see matching_decomp_alignment_fix memory). */
asm(".align 2, 0");
