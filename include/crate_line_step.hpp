/* A Bresenham line's single-octant steppers, for include/crate.hpp, with
 * no include guard: crate.hpp includes them as inline functions
 * (CRATE_LINE_STEP `inline`), which FindLineCrossing (crate_reset.cpp)
 * inlines, one per octant; crate.cpp includes them at its end with
 * CRATE_LINE_STEP empty, for the ROM's out-of-line copies (which have no
 * caller). */

/* A Bresenham line step with y the major axis: walks `dy` steps,
 * counting `count`; each time the error term is not negative, `y` steps
 * by `yStep` and the walk returns `count` once `y` reaches `bound`. -1 if
 * it never does. */
CRATE_LINE_STEP s32 FindLineCrossingYMajor(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound)
{
    s32 twoDx = dx * 2;
    s32 diff = twoDx - dy * 2;
    s32 err = twoDx - dy;
    s32 n = dy - 1;

    if (n != -1) {
        do {
            if (err >= 0) {
                y += yStep;
                if (y >= bound)
                    return count;
                err += diff;
            } else {
                err += twoDx;
            }
            count++;
            n--;
        } while (n != -1);
    }
    return -1;
}

/* The same with x the major axis: walks `dx` steps, `y` stepping each
 * time and `count` only when the error term is not negative. */
CRATE_LINE_STEP s32 FindLineCrossingXMajor(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound)
{
    s32 twoDy = dy * 2;
    s32 diff = twoDy - dx * 2;
    s32 err = twoDy - dx;
    s32 n = dx - 1;

    if (n != -1) {
        do {
            if (err >= 0) {
                count++;
                err += diff;
            } else {
                err += twoDy;
            }
            y += yStep;
            if (y >= bound)
                return count;
            n--;
        } while (n != -1);
    }
    return -1;
}
