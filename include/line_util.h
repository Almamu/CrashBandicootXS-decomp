#ifndef __LINE_UTIL_H__
#define __LINE_UTIL_H__

/* Bresenham-line state: set up by InitBresenhamLine (src/util/line_util.c) and
 * advanced one step at a time by StepBresenhamLine (src/util/line_util2.c).
 * Field names beyond the four input coordinates are left as `field_N`
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

void InitBresenhamLine(struct bresenham_line *l);
void StepBresenhamLine(struct bresenham_line *line);

#endif /* __LINE_UTIL_H__ */
