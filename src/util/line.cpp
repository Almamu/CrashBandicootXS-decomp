extern "C" {
#include "core.h"
#include "util.h"
}

/* Sits right after rand (ROM 0x08000E4C, in src/util/rand.cpp)
 * and before DrawWrappedText (still raw in asm/code_3_1_3.s). */

/* Bresenham-line setup: computes the deltas/signs/error terms for
 * walking a line from (x0,y0) to (x1,y1) one step at a time. */
void InitBresenhamLine(struct bresenham_line *l)
{
    s32 dx, dy;

    l->sx = 0;
    l->sy = 0;
    dx = l->x1 - l->x0;
    dy = l->y1 - l->y0;
    if (dx > 0) {
        l->sx = 1;
    } else if (dx < 0) {
        l->sx = -1;
        dx = -dx;
    }
    if (dy > 0) {
        l->sy = 1;
    } else if (dy < 0) {
        l->sy = -1;
        dy = -dy;
    }
    if (dy <= dx) {
        l->err = (dy << 1) - dx;
        l->errStraight = dy << 1;
        l->errDiagonal = (dy - dx) << 1;
        l->flag = 1;
    } else {
        l->err = (dx << 1) - dy;
        l->errStraight = dx << 1;
        l->errDiagonal = (dx - dy) << 1;
        l->flag = 0;
    }
}
