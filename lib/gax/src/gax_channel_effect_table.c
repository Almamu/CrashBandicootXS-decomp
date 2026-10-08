#include "gax_internal.h"

/* Per-tick vibrato update of a channel's `vibratoOffset` (the pitch offset
 * added to the note when mixing) from a signed waveform table
 * (`gGaxVibratoTable`), gated on the bound instrument's `vibratoDepth`
 * being non-zero: `vibratoDelay` is a delay counter that ticks down once
 * per call, and once it expires `vibratoPhase` advances by the
 * instrument's `vibratoSpeed`, wrapped to 0-0x3f, before indexing the
 * table again. The result is scaled by `vibratoDepth` (>> 8, a standard
 * Q8 multiply-down). */
void GaxChannelTickVibrato(struct GaxChannelState *self)
{
    struct GaxChannelInstrument *inst = self->instrument;
    u32 result = inst->vibratoDepth;

    if (result != 0) {
        u8 *flagPtr = &self->vibratoDelay;

        if (*flagPtr == 0) {
            self->vibratoPhase = (self->vibratoPhase + inst->vibratoSpeed) & 0x3f;
        } else {
            *flagPtr -= 1;
        }
        {
            u8 *table = (u8 *)gGaxVibratoTable;
            u16 phase = self->vibratoPhase;
            u32 zero = 0;
            u8 *addr = table + phase;
            s8 tableVal = *(s8 *)(addr + zero);
            result = (tableVal * self->instrument->vibratoDepth) >> 8;
        }
    }
    self->vibratoOffset = result;
}
