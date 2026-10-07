#include "gax_internal.h"

/* Per-tick ping-pong sweep of a channel's sample start position: while
 * `sweepOn`, every `rows[row].sweepRate` ticks steps `sweepPos` by the
 * row's `sweepStep` in `sweepDir`'s direction, bouncing (and reversing
 * direction) off `sweepMax - sweepLen` / `sweepMin`, then shifts
 * `samplePos` (Q11) by the same amount.
 *
 * Was NAKED behind an 81%-matching register-pinned draft; written
 * plainly against the instrument-row struct it matches outright - see
 * docs/matching/archive/gax-toolchain-retry.md. */
void GaxChannelTickSweep(struct GaxChannelState *self)
{
    s32 old;
    struct GaxChannelInstrument *inst;

    if (self->sweepOn == 0)
        return;
    if (--self->sweepTimer != 0)
        return;

    old = self->sweepPos;
    inst = self->instrument;
    self->sweepTimer = inst->rows[self->row].sweepRate;
    if (self->sweepDir > 0) {
        self->sweepPos = old + inst->rows[self->row].sweepStep;
        if (self->sweepPos + inst->rows[self->row].sweepLen > inst->rows[self->row].sweepMax) {
            self->sweepPos -= inst->rows[self->row].sweepStep * 2;
            self->sweepDir = -1;
        }
    } else {
        self->sweepPos = old - inst->rows[self->row].sweepStep;
        if (self->sweepPos < inst->rows[self->row].sweepMin) {
            self->sweepPos += inst->rows[self->row].sweepStep * 2;
            self->sweepDir = 1;
        }
    }
    self->samplePos = self->samplePos - (old << 11) + (self->sweepPos << 11);
}
