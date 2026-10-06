#include "gax_internal.h"
#include "match.h"

/* The FX channel type's init method: resets a sound-effect voice to its
 * defaults (no instrument, the `0x8AD0` "no note" sentinel, full vol15,
 * default volume (-1), lowest voice-steal priority) and picks the
 * resampler mode from the player's `fullResampler` flag (1, or 2 with
 * the full resampler). */
void GaxFxChannelInit(void *self)
{
    struct GaxChannelState *p = self;
    u32 zeroA = 0;
    MATCH_HOLD_REG(u32, zeroB, r1);
    u16 val;
    u8 b;

    p->samplePos = zeroA;
    p->row = (u8)zeroA;
    p->instrument = NULL;
    zeroB = 0;
    val = 0x8AD0;
    p->note = val;
    *(u8 *)&p->direction = 1; /* as an s8 store, gcc reuses this 1 for negOne */
    p->vol15 = 0xff;
    {
        s32 negOne = 1;
        negOne = -negOne;
        p->volume = negOne;
    }
    p->muted = zeroB;
    p->sweepOn = zeroB;
    p->isFirst = zeroB;
    p->pendingNote = zeroB;
    p->pendingInstrument = zeroB;
    p->priority = 0x80000000;
    b = gGaxPlayerState->fullResampler;
    {
        u32 v = 1;
        if (b != 0) {
            v = 2;
        }
        p->mixMode = v;
    }
}
