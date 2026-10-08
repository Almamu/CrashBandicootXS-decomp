#include "gax_internal.h"

/* The FX channel type's init method: resets a sound-effect voice to its
 * defaults (no instrument, the `0x8AD0` "no note" sentinel, full vol15,
 * default volume (-1), lowest voice-steal priority) and picks the
 * resampler mode from the player's `fullResampler` flag (1, or 2 with
 * the full resampler). */
void GaxFxChannelInit(void *self)
{
    struct GaxChannelState *p = self;

    p->samplePos = 0;
    p->row = 0;
    p->instrument = NULL;
    p->note = 0x8AD0;
    p->direction = 1;
    p->vol15 = 0xff;
    p->volume = -1;
    p->muted = 0;
    p->sweepOn = 0;
    p->isFirst = 0;
    p->pendingNote = 0;
    p->pendingInstrument = 0;
    p->priority = 0x80000000;
    {
        u8 b = gGaxPlayerState->fullResampler;
        u32 v = 1;

        if (b != 0)
            v = 2;
        p->mixMode = v;
    }
}
