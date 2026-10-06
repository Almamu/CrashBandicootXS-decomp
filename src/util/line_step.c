#include "core.h"
#include "util.h"

/* Sits right after DrawWrappedTextInBox (ROM 0x08001214, in src/text/text_box.c)
 * and before StepBrightnessFade (still raw in asm/code_3_1_7.s). Not adjacent
 * to InitBresenhamLine (src/util/line.c) in ROM address order - kept in its
 * own file purely because that file's object already links much
 * earlier; both share the struct definition from include/line_util.h. */

/* Advances a Bresenham line (set up by InitBresenhamLine) by one step: the
 * "driving" axis (x if `flag` is set, y otherwise) always advances by
 * its sign; the other axis advances only when the accumulated error
 * term (`err`) is positive, in which case it is corrected by
 * `errDiagonal` instead of `errStraight`. The error term is re-read from
 * `line` fresh inside each of the two `flag` branches (not hoisted
 * above the branch), matching the ROM's own two separate reloads. */
void StepBresenhamLine(struct bresenham_line *line)
{
    s32 err;

    if (line->flag != 0) {
        err = line->err;
        if (err <= 0) {
            line->err = err + line->errStraight;
        } else {
            line->err = err + line->errDiagonal;
            line->y0 += line->sy;
        }
        line->x0 += line->sx;
    } else {
        err = line->err;
        if (err <= 0) {
            line->err = err + line->errStraight;
        } else {
            line->err = err + line->errDiagonal;
            line->x0 += line->sx;
        }
        line->y0 += line->sy;
    }
}

/* Trailing padding (see matching_decomp_alignment_fix memory). */
asm(".align 2, 0");
