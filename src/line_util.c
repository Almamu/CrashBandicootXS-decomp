#include "core.h"

/* Sits right after sub_8000E4C (ROM 0x08000E4C, in src/rand_util.c)
 * and before sub_8000EE4 (still raw in asm/code_3_1_3.s). */

/* Bresenham-line setup: computes the deltas/signs/error terms for
 * walking a line from (x0,y0) to (x1,y1) one step at a time. Field
 * names beyond the four input coordinates are left as `field_N`
 * (offsets, not purposes) until a caller clarifies them. */
struct bresenham_line {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 sx;
    s32 sy;
    u8 flag;
};

void sub_8000E6C(struct bresenham_line *l)
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
        l->field_10 = (dy << 1) - dx;
        l->field_14 = dy << 1;
        l->field_18 = (dy - dx) << 1;
        l->flag = 1;
    } else {
        l->field_10 = (dx << 1) - dy;
        l->field_14 = dx << 1;
        l->field_18 = (dx - dy) << 1;
        l->flag = 0;
    }
}
