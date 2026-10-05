#include "gax_internal.h"

/* Steps a channel's instrument sequence (`instrument->seq`, one entry
 * per call while `cutDelay` keeps it armed): an entry can set a new
 * note, and with it a new wave row - resetting the sample position and,
 * if that row has a valid ping-pong sweep, arming `GaxChannelTickSweep`'s sweep
 * state - then applies its two effect commands (note slide, sequence
 * jump/loop, volume slide/set, sequence speed). Past the end of the
 * sequence it disarms itself (`cutDelay = 0`).
 *
 * Was NAKED ("r8/sb allocation ceiling"); written plainly - every
 * access through `self->instrument` directly, no cached local - it
 * matches outright, see docs/matching/gax-toolchain-retry.md. (Caching
 * `self->instrument` in a local drops the ROM's one `mov` between the
 * load and its callee-saved copy.) */
void GaxChannelStepInstrumentSeq(struct GaxChannelState *self)
{
    struct GaxInstrumentSeqEntry *e = &self->instrument->seq[self->seqPos];
    u32 i;

    if (self->seqPos >= self->instrument->seqLen) {
        self->cutDelay = 0;
        return;
    }
    self->seqPos++;
    if (e->note != 0) {
        self->note = (e->note - 2) << 5;
        self->field_21 = e->field_01;
        if (e->wave != 0) {
            self->row = e->wave - 1;
            self->samplePos = 0;
            self->field_11 = 1;
            self->vol17 = 0xff;
            self->sweepOn = 0;
            if (self->instrument->rows[self->row].field_00 != 0
                && self->instrument->rows[self->row].sweepMin < self->instrument->rows[self->row].sweepMax
                && self->instrument->rows[self->row].sweepLen > 0
                && self->instrument->rows[self->row].sweepRate != 0
                && self->instrument->rows[self->row].sweepStep > 0) {
                self->sweepOn = 1;
                self->sweepPos = self->instrument->rows[self->row].start;
                self->samplePos = self->sweepPos << 11;
                self->sweepTimer = self->instrument->rows[self->row].sweepRate;
                self->sweepDir = 1;
                if (self->sweepPos + self->instrument->rows[self->row].sweepLen
                    > self->instrument->rows[self->row].sweepMax)
                    self->sweepDir = -1;
            } else {
                self->samplePos = self->instrument->rows[self->row].start << 11;
            }
        }
    }
    self->volStep17 = 0;
    self->noteStep = 0;
    for (i = 0; i < 2; i++) {
        u32 cmd = e->fx[i] >> 8;
        u32 param = e->fx[i] & 0xff;

        switch (cmd) {
        case 1:
            self->noteStep = param;
            break;
        case 2:
            self->noteStep = -param;
            break;
        case 5:
            if (self->seqLoopCount == 0 || --self->seqLoopCount != 0)
                self->seqPos = param;
            break;
        case 6:
            if (self->seqLoopCount == 0)
                self->seqLoopCount = param ? param + 1 : 0;
            break;
        case 10:
            self->volStep17 = param;
            break;
        case 11:
            self->volStep17 = -param;
            break;
        case 12:
            self->vol17 = param;
            break;
        case 15:
            self->cutDelay = param;
            break;
        }
    }
}
