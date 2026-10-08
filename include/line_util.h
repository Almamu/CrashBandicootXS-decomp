#ifndef __LINE_UTIL_H__
#define __LINE_UTIL_H__

/* Bresenham-line state: set up by InitBresenhamLine (src/util/line.cpp) and
 * advanced one step at a time by StepBresenhamLine (src/util/line_step.cpp).
 * The two functions are declared in util.h. */
struct bresenham_line {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 err;         /* the decision term: > 0 steps the minor axis too */
    s32 errStraight; /* added to err on a major-axis-only step (2 * minor delta) */
    s32 errDiagonal; /* added on a diagonal step (2 * (minor - major delta)) */
    s32 sx;
    s32 sy;
    u8 flag; /* 1: x is the major (driving) axis */
};

#endif /* __LINE_UTIL_H__ */
