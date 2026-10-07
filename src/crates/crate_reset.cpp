#include "crate.hpp"

extern "C" {
#include "globals.h"
#include "player.h"
}

/* The general line stepper and Crate::Reset (#664, include/crate.hpp). */

/* A Bresenham line from (pos, count) to (a, b), from the end with the
 * lower count: returns `count` at the step where `pos` passes `limit`, or
 * -1 if it never does. The four cases are the octants: x or y the major
 * axis, `pos` increasing or decreasing; each is one of crate.hpp's
 * steppers, inlined. */
s32 FindLineCrossing(s32 pos, s32 count, s32 a, s32 b, s32 limit)
{
    s32 dx, dy;
    s32 tmp;

    if (count > b) {
        tmp = count;
        count = b;
        b = tmp;
        tmp = pos;
        pos = a;
        a = tmp;
    }
    dx = a - pos;
    dy = b - count;
    if (dx > 0) {
        if (dx > dy)
            return FindLineCrossingXMajor(pos, count, dx, dy, 1, limit);
        return FindLineCrossingYMajor(pos, count, dx, dy, 1, limit);
    }
    dx = -dx;
    if (dx > dy)
        return FindLineCrossingXMajor(pos, count, dx, dy, -1, limit);
    return FindLineCrossingYMajor(pos, count, dx, dy, -1, limit);
}

/* Resets the crate's state: the contact and vulnerable flags set, the
 * state and busy bits and the player's `busy` latch cleared, the fall,
 * the per-kind fields and the stack links zeroed, and no time-trial
 * kind. */
void Crate::Reset()
{
    EnableContact();
    f.b.vulnerable = 1;
    state &= CRATE_STATE_MASK;
    gPlayer->busy = 0;
    state &= CRATE_STATE_BUSY;
    fallDistance = 0;
    fallSpeed = 0;
    solidKind = 0;
    timer = 0;
    paramA = 0;
    paramB = 0;
    touched = 0;
    trialKind = -1;
    above = 0;
    below = 0;
}
