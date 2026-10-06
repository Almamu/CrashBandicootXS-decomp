#include "gax_internal.h"

/* Per-tick vibrato update of a channel's `field_2e` (the pitch offset
 * added to the note when mixing) from a signed waveform table
 * (`gGaxVibratoTable`), gated on the bound instrument's `vibratoDepth`
 * being non-zero: `vibratoDelay` is a delay counter that ticks down once
 * per call, and once it expires `vibratoPhase` advances by the
 * instrument's `vibratoSpeed`, wrapped to 0-0x3f, before indexing the
 * table again. The result is scaled by `vibratoDepth` (>> 8, a standard
 * Q8 multiply-down). */
void GaxChannelTickVibrato(struct GaxChannelState *self)
{
    register struct GaxChannelState *p asm("r2") = self;
    struct GaxChannelInstrument *inst = p->instrument;
    u32 result = inst->vibratoDepth;

    if (result != 0) {
        u8 *flagPtr = &p->vibratoDelay;

        if (*flagPtr == 0) {
            p->vibratoPhase = (p->vibratoPhase + inst->vibratoSpeed) & 0x3f;
        } else {
            *flagPtr -= 1;
        }
        {
            u8 *table = (u8 *)gGaxVibratoTable;
            u16 phase = p->vibratoPhase;
            u32 zero = 0;
            u8 *addr = table + phase;
            s8 tableVal = *(s8 *)(addr + zero);
            result = (tableVal * p->instrument->vibratoDepth) >> 8;
        }
    }
    p->field_2e = result;
}
