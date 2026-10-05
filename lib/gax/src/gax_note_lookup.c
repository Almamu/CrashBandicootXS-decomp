#include "gax_internal.h"

/* Advances a channel's envelope position `*posp` by one tick and
 * returns the envelope's value there. Before interpolating it applies
 * the envelope's control points: while the note is held (`released ==
 * 0`) the position sticks at the sustain point; reaching the last
 * breakpoint pins the position there and, if that breakpoint's value is
 * 0 and no loop reaches it, arms the note-off state (`note = 0x8AD0`);
 * while held, reaching `loopEnd` jumps back to `loopStart`. The value
 * is the breakpoint's own value on an exact hit, otherwise the previous
 * breakpoint's value plus its Q8 slope times the distance past it.
 *
 * Was NAKED behind a register-pinned draft with a one-register
 * residual; written plainly against the envelope struct it matches
 * outright - see docs/matching/gax-toolchain-retry.md. */
u8 GaxEnvelopeTick(struct GaxChannelState *self, struct GaxEnvelope *env, u16 *posp)
{
    u16 pos = (*posp)++;
    s32 last;
    s32 i;

    if (env->sustain != 0xff && self->released == 0 && pos == env->points[env->sustain].pos)
        *posp = pos;

    last = env->count - 1;
    if (pos >= env->points[last].pos) {
        if (env->points[last].value == 0 && (env->loopEnd == 0xff || env->loopEnd < last)) {
            self->note = 0x8ad0;
            self->noteStep = 0;
            self->priority = 0x80000000;
        }
        *posp = pos;
    }

    if (self->released == 0 && env->loopStart != 0xff && env->loopEnd != 0xff
        && pos == env->points[env->loopEnd].pos)
        *posp = env->points[env->loopStart].pos;

    i = 0;
    while (env->points[i].pos < pos)
        i++;
    if (pos == env->points[i].pos)
        return env->points[i].value;
    {
        s32 slope = env->points[i].slope;

        i--;
        return ((pos - env->points[i].pos) * slope >> 8) + env->points[i].value;
    }
}
